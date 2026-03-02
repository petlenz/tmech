/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_KERNELS_SMALL_H
#define GEMM_KERNELS_SMALL_H

// -------------------- restrict helper --------------------
#if !defined(TMECH_RESTRICT)
  #if defined(__GNUC__) || defined(__clang__)
    #define TMECH_RESTRICT __restrict__
  #else
    #define TMECH_RESTRICT
  #endif
#endif

// -------------------- force-inline helper --------------------
#if !defined(TMECH_INLINE)
  #if defined(__GNUC__) || defined(__clang__)
    #define TMECH_INLINE inline __attribute__((always_inline))
  #elif defined(_MSC_VER)
    #define TMECH_INLINE __forceinline
  #else
    #define TMECH_INLINE inline
  #endif
#endif

#ifdef TMECH_HAS_XSIMD

#include <array>
#include <utility> // index_sequence

namespace detail {

// Fully unrolled micro-kernels for small GEMM/GEMV sizes.
// Single-register path: dimension fits in one SIMD register (dim <= W).
// Multi-register path: dimension spans multiple registers (W < dim <= MAX_SMALL).
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
struct gemm_kernels_small
{
private:
    using size_type = std::size_t;

    static constexpr std::size_t safe_simd_width() {
        if constexpr (has_xsimd_batch_v<RESULT>) {
            return xsimd::batch<RESULT>::size;
        }
        return 0;
    }

public:
    static constexpr std::size_t W = safe_simd_width();
    static constexpr std::size_t MAX_SMALL = 9;

    static constexpr bool is_small_gemm = (ColumnsRHS > 1)
                                        && (ColumnsRHS <= MAX_SMALL)
                                        && (ColumnsLHS <= MAX_SMALL)
                                        && (RowsLHS <= MAX_SMALL)
                                        && (W > 0);
    static constexpr bool is_small_gemv = (ColumnsRHS == 1)
                                        && (ColumnsLHS <= MAX_SMALL)
                                        && (W > 0);

    static constexpr size_type NW_V   = (W > 0) ? (ColumnsLHS / W) : 0;
    static constexpr size_type TAIL_V = (W > 0) ? (ColumnsLHS % W) : ColumnsLHS;
    static constexpr size_type NW_M   = (W > 0) ? (ColumnsRHS / W) : 0;
    static constexpr size_type TAIL_M = (W > 0) ? (ColumnsRHS % W) : ColumnsRHS;

    // ================================================================
    // GEMV
    // ================================================================

    template <std::size_t... Is>
    static inline void gemv_small_simd(LHS const* TMECH_RESTRICT __lhs,
                                       RHS const* TMECH_RESTRICT __rhs,
                                       RESULT* TMECH_RESTRICT __result,
                                       std::index_sequence<Is...>) noexcept
    {
        using batch_type = xsimd::batch<RESULT>;
        auto rhs_vec = batch_type::load_unaligned(__rhs);
        ((void)(__result[Is] = xsimd::reduce_add(
            batch_type::load_unaligned(__lhs + Is * ColumnsLHS) * rhs_vec)), ...);
    }

    template <std::size_t I, std::size_t... Ks>
    static constexpr TMECH_INLINE RESULT gemv_dot_row(LHS const* __lhs, RHS const* __rhs,
                                                std::index_sequence<Ks...>) noexcept
    {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + Ks]) *
                 static_cast<RESULT>(__rhs[Ks])) + ...);
    }

    template <std::size_t... Is>
    static constexpr inline void gemv_small_scalar(LHS const* __lhs, RHS const* __rhs,
                                                   RESULT* __result,
                                                   std::index_sequence<Is...>) noexcept
    {
        ((__result[Is] = gemv_dot_row<Is>(__lhs, __rhs,
                                          std::make_index_sequence<ColumnsLHS>{})), ...);
    }

    template <std::size_t... Ws>
    static TMECH_INLINE void preload_rhs_v(xsimd::batch<RESULT>* rhs_b, RHS const* __rhs,
                                     std::index_sequence<Ws...>) noexcept
    {
        using batch_type = xsimd::batch<RESULT>;
        ((rhs_b[Ws] = batch_type::load_unaligned(__rhs + Ws * W)), ...);
    }

    template <std::size_t I, std::size_t... Ws>
    static TMECH_INLINE auto gemv_dot_chunks(LHS const* __lhs,
                                       xsimd::batch<RESULT> const* rhs_b,
                                       std::index_sequence<Ws...>) noexcept
    {
        using batch_type = xsimd::batch<RESULT>;
        // Sum of products across full chunks (vector sum)
        return ((batch_type::load_unaligned(__lhs + I * ColumnsLHS + Ws * W) * rhs_b[Ws]) + ...);
    }

    template <std::size_t I, std::size_t... Ts>
    static constexpr TMECH_INLINE RESULT gemv_dot_tail(LHS const* __lhs, RHS const* __rhs,
                                                 std::index_sequence<Ts...>) noexcept
    {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + NW_V * W + Ts]) *
                 static_cast<RESULT>(__rhs[NW_V * W + Ts])) + ...);
    }

    template <std::size_t... Is>
    static inline void gemv_small_multi(LHS const* TMECH_RESTRICT __lhs,
                                        RHS const* TMECH_RESTRICT __rhs,
                                        RESULT* TMECH_RESTRICT __result,
                                        std::index_sequence<Is...>) noexcept
    {
        using batch_type = xsimd::batch<RESULT>;
        constexpr std::size_t Align = xsimd::default_arch::alignment();

        alignas(Align) batch_type rhs_b[NW_V > 0 ? NW_V : 1];
        preload_rhs_v(rhs_b, __rhs, std::make_index_sequence<NW_V>{});

        if constexpr (TAIL_V > 0) {
            ((__result[Is] =
                xsimd::reduce_add(gemv_dot_chunks<Is>(__lhs, rhs_b, std::make_index_sequence<NW_V>{}))
              + gemv_dot_tail<Is>(__lhs, __rhs, std::make_index_sequence<TAIL_V>{})), ...);
        } else {
            ((__result[Is] =
                xsimd::reduce_add(gemv_dot_chunks<Is>(__lhs, rhs_b, std::make_index_sequence<NW_V>{}))), ...);
        }
    }

    static inline void gemv_small(LHS const* TMECH_RESTRICT __lhs,
                                        RHS const* TMECH_RESTRICT __rhs,
                                        RESULT* TMECH_RESTRICT __result) noexcept
    {
        if constexpr (ColumnsLHS > W) {
            gemv_small_multi(__lhs, __rhs, __result, std::make_index_sequence<RowsLHS>{});
        } else if constexpr (ColumnsLHS == W) {
            gemv_small_simd(__lhs, __rhs, __result, std::make_index_sequence<RowsLHS>{});
        } else {
            gemv_small_scalar(__lhs, __rhs, __result, std::make_index_sequence<RowsLHS>{});
        }
    }

    // ================================================================
    // GEMM
    // ================================================================

    // -------- Single-register GEMM (ColumnsRHS <= W) --------
    // Use explicit FMA chain instead of "sum of products" expression.

    template <std::size_t... Ks>
    static TMECH_INLINE xsimd::batch<RESULT> fma_row_fused(xsimd::batch<RESULT> const* rhs_rows,
                                                     LHS const* lhs_row,
                                                     std::index_sequence<Ks...>) noexcept
    {
        using batch = xsimd::batch<RESULT>;
        batch acc;
        // fold that builds: acc = fma(broadcast(a_k), rhs_rows[k], acc) ...
        ((acc = xsimd::fma(batch(static_cast<RESULT>(lhs_row[Ks])), rhs_rows[Ks], acc)), ...);
        return acc;
    }

    template <std::size_t... Ks>
    static TMECH_INLINE void preload_rhs_single(xsimd::batch<RESULT>* rhs_rows,
                                          RHS const* __rhs,
                                          std::index_sequence<Ks...>) noexcept
    {
        using batch = xsimd::batch<RESULT>;
        ((rhs_rows[Ks] = batch::load_unaligned(__rhs + Ks * ColumnsRHS)), ...);
    }

    template <std::size_t... Is>
    static inline void gemm_small_simd(LHS const* TMECH_RESTRICT __lhs,
                                       RHS const* TMECH_RESTRICT __rhs,
                                       RESULT* TMECH_RESTRICT __result,
                                       std::index_sequence<Is...>) noexcept
    {
        using batch = xsimd::batch<RESULT>;
        constexpr std::size_t Align = xsimd::default_arch::alignment();

        alignas(Align) batch rhs_rows[ColumnsLHS];
        preload_rhs_single(rhs_rows, __rhs, std::make_index_sequence<ColumnsLHS>{});

        (fma_row_fused(rhs_rows, __lhs + Is * ColumnsLHS, std::make_index_sequence<ColumnsLHS>{})
            .store_unaligned(__result + Is * ColumnsRHS), ...);
    }

    // -------- Scalar GEMM (ColumnsRHS < W) --------

    template <std::size_t I, std::size_t J, std::size_t... Ks>
    static constexpr TMECH_INLINE RESULT dot_ij(LHS const* __lhs, RHS const* __rhs,
                                          std::index_sequence<Ks...>) noexcept
    {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + Ks]) *
                 static_cast<RESULT>(__rhs[Ks * ColumnsRHS + J])) + ...);
    }

    template <std::size_t I, std::size_t... Js>
    static constexpr TMECH_INLINE void gemm_row_scalar(LHS const* __lhs, RHS const* __rhs,
                                                 RESULT* __result,
                                                 std::index_sequence<Js...>) noexcept
    {
        ((__result[I * ColumnsRHS + Js] =
            dot_ij<I, Js>(__lhs, __rhs, std::make_index_sequence<ColumnsLHS>{})), ...);
    }

    template <std::size_t... Is>
    static constexpr inline void gemm_small_scalar(LHS const* __lhs, RHS const* __rhs,
                                                   RESULT* __result,
                                                   std::index_sequence<Is...>) noexcept
    {
        (gemm_row_scalar<Is>(__lhs, __rhs, __result, std::make_index_sequence<ColumnsRHS>{}), ...);
    }

    // -------- Multi-register GEMM (ColumnsRHS > W) --------
    // Key optimization: process multiple rows at once to reuse RHS loads.

    static constexpr std::size_t ROW_BLOCK =
        (RowsLHS >= 4) ? 4 : (RowsLHS >= 2 ? 2 : 1);

    template <std::size_t R>
    struct row_blocker {
        // R is 1, 2, or 4
        template <std::size_t I0>
        static inline void run(LHS const* TMECH_RESTRICT __lhs,
                                     RHS const* TMECH_RESTRICT __rhs,
                                     RESULT* TMECH_RESTRICT __result) noexcept
        {
            using batch = xsimd::batch<RESULT>;
            constexpr std::size_t Align = xsimd::default_arch::alignment();

            alignas(Align) batch acc[R][NW_M > 0 ? NW_M : 1];
            RESULT tail[R][TAIL_M > 0 ? TAIL_M : 1];

            // zero
            for (std::size_t r = 0; r < R; ++r) {
                for (std::size_t j = 0; j < NW_M; ++j) acc[r][j] = batch(RESULT(0));
                if constexpr (TAIL_M > 0) {
                    for (std::size_t t = 0; t < TAIL_M; ++t) tail[r][t] = RESULT(0);
                }
            }

            // K loop (small)
            for (std::size_t k = 0; k < ColumnsLHS; ++k) {
                // load RHS chunks once per k
                alignas(Align) batch bvec[NW_M > 0 ? NW_M : 1];
                for (std::size_t j = 0; j < NW_M; ++j) {
                    bvec[j] = batch::load_unaligned(__rhs + k * ColumnsRHS + j * W);
                }

                // for each row in block
                for (std::size_t r = 0; r < R; ++r) {
                    const std::size_t I = I0 + r;
                    auto a_val = static_cast<RESULT>(__lhs[I * ColumnsLHS + k]);
                    batch a_bc(a_val);

                    for (std::size_t j = 0; j < NW_M; ++j) {
                        acc[r][j] = xsimd::fma(a_bc, bvec[j], acc[r][j]);
                    }

                    if constexpr (TAIL_M > 0) {
                        for (std::size_t t = 0; t < TAIL_M; ++t) {
                            tail[r][t] += a_val * static_cast<RESULT>(
                                __rhs[k * ColumnsRHS + NW_M * W + t]);
                        }
                    }
                }
            }

            // store
            for (std::size_t r = 0; r < R; ++r) {
                const std::size_t I = I0 + r;
                RESULT* out = __result + I * ColumnsRHS;

                for (std::size_t j = 0; j < NW_M; ++j) {
                    acc[r][j].store_unaligned(out + j * W);
                }
                if constexpr (TAIL_M > 0) {
                    for (std::size_t t = 0; t < TAIL_M; ++t) {
                        out[NW_M * W + t] = tail[r][t];
                    }
                }
            }
        }
    };

    template <std::size_t I>
    static TMECH_INLINE void gemm_row_multi_1(LHS const* TMECH_RESTRICT __lhs,
                                        RHS const* TMECH_RESTRICT __rhs,
                                        RESULT* TMECH_RESTRICT __result) noexcept
    {
        row_blocker<1>::template run<I>(__lhs, __rhs, __result);
    }

    static TMECH_INLINE void gemm_small_multi(LHS const* TMECH_RESTRICT __lhs,
                                               RHS const* TMECH_RESTRICT __rhs,
                                               RESULT* TMECH_RESTRICT __result) noexcept
    {
        // Compile-time explicit row dispatch (RowsLHS <= 9)
        if constexpr (RowsLHS == 1) {
            row_blocker<1>::template run<0>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 2) {
            row_blocker<2>::template run<0>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 3) {
            row_blocker<2>::template run<0>(__lhs, __rhs, __result);
            row_blocker<1>::template run<2>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 4) {
            row_blocker<4>::template run<0>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 5) {
            row_blocker<4>::template run<0>(__lhs, __rhs, __result);
            row_blocker<1>::template run<4>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 6) {
            row_blocker<4>::template run<0>(__lhs, __rhs, __result);
            row_blocker<2>::template run<4>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 7) {
            row_blocker<4>::template run<0>(__lhs, __rhs, __result);
            row_blocker<2>::template run<4>(__lhs, __rhs, __result);
            row_blocker<1>::template run<6>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 8) {
            row_blocker<4>::template run<0>(__lhs, __rhs, __result);
            row_blocker<4>::template run<4>(__lhs, __rhs, __result);
        } else if constexpr (RowsLHS == 9) {
            row_blocker<4>::template run<0>(__lhs, __rhs, __result);
            row_blocker<4>::template run<4>(__lhs, __rhs, __result);
            row_blocker<1>::template run<8>(__lhs, __rhs, __result);
        }
    }

    // --- GEMM dispatch ---
    static inline void gemm_small(LHS const* TMECH_RESTRICT __lhs,
                                        RHS const* TMECH_RESTRICT __rhs,
                                        RESULT* TMECH_RESTRICT __result) noexcept
    {
        if constexpr (ColumnsRHS > W) {
            gemm_small_multi(__lhs, __rhs, __result);
        } else if constexpr (ColumnsRHS == W) {
            gemm_small_simd(__lhs, __rhs, __result, std::make_index_sequence<RowsLHS>{});
        } else {
            gemm_small_scalar(__lhs, __rhs, __result, std::make_index_sequence<RowsLHS>{});
        }
    }
};

} // namespace detail

#endif // TMECH_HAS_XSIMD
#endif // GEMM_KERNELS_SMALL_H