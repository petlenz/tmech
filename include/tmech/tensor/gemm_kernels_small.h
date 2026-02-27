/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_KERNELS_SMALL_H
#define GEMM_KERNELS_SMALL_H

#ifdef TMECH_HAS_XSIMD

namespace detail {

// Fully unrolled micro-kernels for small GEMM/GEMV sizes.
// Single-register path: dimension fits in one SIMD register (dim <= W).
// Multi-register path: dimension spans multiple registers (W < dim <= MAX_SMALL).
// Uses std::index_sequence fold expressions for row unrolling.
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

    // Tier selection flags
    static constexpr bool is_small_gemm = (ColumnsRHS > 1)
                                        && (ColumnsRHS <= MAX_SMALL)
                                        && (ColumnsLHS <= MAX_SMALL)
                                        && (RowsLHS <= MAX_SMALL)
                                        && (W > 0);
    static constexpr bool is_small_gemv = (ColumnsRHS == 1)
                                        && (ColumnsLHS <= MAX_SMALL)
                                        && (W > 0);

    // Multi-register layout (guarded against W==0)
    static constexpr size_type NW_V   = (W > 0) ? (ColumnsLHS / W) : 0;
    static constexpr size_type TAIL_V = (W > 0) ? (ColumnsLHS % W) : ColumnsLHS;
    static constexpr size_type NW_M   = (W > 0) ? (ColumnsRHS / W) : 0;
    static constexpr size_type TAIL_M = (W > 0) ? (ColumnsRHS % W) : ColumnsRHS;

    // ================================================================
    // GEMV — fully unrolled matrix-vector product (ColumnsRHS == 1)
    // ================================================================

    // --- Single-register GEMV (ColumnsLHS <= W) ---

    template <std::size_t... Is>
    static inline void gemv_small_simd(LHS const* __lhs, RHS const* __rhs,
                                       RESULT* __result,
                                       std::index_sequence<Is...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        auto rhs_vec = batch_type::load_unaligned(__rhs);
        ((void)(__result[Is] = xsimd::reduce_add(
            batch_type::load_unaligned(__lhs + Is * ColumnsLHS) * rhs_vec)), ...);
    }

    template <std::size_t I, std::size_t... Ks>
    static constexpr inline RESULT gemv_dot_row(LHS const* __lhs, RHS const* __rhs,
                                                std::index_sequence<Ks...>) noexcept {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + Ks]) *
                 static_cast<RESULT>(__rhs[Ks])) + ...);
    }

    template <std::size_t... Is>
    static constexpr inline void gemv_small_scalar(LHS const* __lhs, RHS const* __rhs,
                                                   RESULT* __result,
                                                   std::index_sequence<Is...>) noexcept {
        ((__result[Is] = gemv_dot_row<Is>(__lhs, __rhs,
                                          std::make_index_sequence<ColumnsLHS>{})), ...);
    }

    // --- Multi-register GEMV (ColumnsLHS > W) ---

    template <std::size_t... Ws>
    static inline void preload_rhs_v(xsimd::batch<RESULT>* rhs_b, RHS const* __rhs,
                                     std::index_sequence<Ws...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        ((rhs_b[Ws] = batch_type::load_unaligned(__rhs + Ws * W)), ...);
    }

    template <std::size_t I, std::size_t... Ws>
    static inline auto gemv_dot_chunks(LHS const* __lhs,
                                       xsimd::batch<RESULT> const* rhs_b,
                                       std::index_sequence<Ws...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        return ((batch_type::load_unaligned(__lhs + I * ColumnsLHS + Ws * W)
                 * rhs_b[Ws]) + ...);
    }

    template <std::size_t I, std::size_t... Ts>
    static constexpr inline RESULT gemv_dot_tail(LHS const* __lhs, RHS const* __rhs,
                                                 std::index_sequence<Ts...>) noexcept {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + NW_V * W + Ts]) *
                 static_cast<RESULT>(__rhs[NW_V * W + Ts])) + ...);
    }

    template <std::size_t... Is>
    static inline void gemv_small_multi(LHS const* __lhs, RHS const* __rhs,
                                        RESULT* __result,
                                        std::index_sequence<Is...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        batch_type rhs_b[NW_V > 0 ? NW_V : 1];
        preload_rhs_v(rhs_b, __rhs, std::make_index_sequence<NW_V>{});
        if constexpr (TAIL_V > 0) {
            ((__result[Is] = xsimd::reduce_add(
                gemv_dot_chunks<Is>(__lhs, rhs_b, std::make_index_sequence<NW_V>{}))
                + gemv_dot_tail<Is>(__lhs, __rhs,
                    std::make_index_sequence<TAIL_V>{})), ...);
        } else {
            ((__result[Is] = xsimd::reduce_add(
                gemv_dot_chunks<Is>(__lhs, rhs_b,
                    std::make_index_sequence<NW_V>{}))), ...);
        }
    }

    // --- GEMV dispatch ---

    static inline void gemv_small(LHS const* __lhs, RHS const* __rhs,
                                  RESULT* __result) noexcept {
        if constexpr (ColumnsLHS > W) {
            gemv_small_multi(__lhs, __rhs, __result,
                             std::make_index_sequence<RowsLHS>{});
        } else if constexpr (ColumnsLHS == W) {
            gemv_small_simd(__lhs, __rhs, __result,
                            std::make_index_sequence<RowsLHS>{});
        } else {
            gemv_small_scalar(__lhs, __rhs, __result,
                              std::make_index_sequence<RowsLHS>{});
        }
    }

    // ================================================================
    // GEMM — fully unrolled matrix-matrix product (ColumnsRHS > 1)
    // ================================================================

    // --- Single-register GEMM (ColumnsRHS <= W) ---

    template <std::size_t... Ks>
    static inline auto fma_row(xsimd::batch<RESULT> const* rhs_rows,
                               LHS const* lhs_row,
                               std::index_sequence<Ks...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        return ((batch_type(static_cast<RESULT>(lhs_row[Ks])) * rhs_rows[Ks]) + ...);
    }

    template <std::size_t... Ks>
    static inline void preload_rhs(xsimd::batch<RESULT>* rhs_rows,
                                   RHS const* __rhs,
                                   std::index_sequence<Ks...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        ((rhs_rows[Ks] = batch_type::load_unaligned(__rhs + Ks * ColumnsRHS)), ...);
    }

    template <std::size_t... Is>
    static inline void gemm_small_simd(LHS const* __lhs, RHS const* __rhs,
                                       RESULT* __result,
                                       std::index_sequence<Is...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        batch_type rhs_rows[ColumnsLHS];
        preload_rhs(rhs_rows, __rhs, std::make_index_sequence<ColumnsLHS>{});
        (fma_row(rhs_rows, __lhs + Is * ColumnsLHS,
                 std::make_index_sequence<ColumnsLHS>{})
            .store_unaligned(__result + Is * ColumnsRHS), ...);
    }

    template <std::size_t I, std::size_t J, std::size_t... Ks>
    static constexpr inline RESULT dot_ij(LHS const* __lhs, RHS const* __rhs,
                                          std::index_sequence<Ks...>) noexcept {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + Ks]) *
                 static_cast<RESULT>(__rhs[Ks * ColumnsRHS + J])) + ...);
    }

    template <std::size_t I, std::size_t... Js>
    static constexpr inline void gemm_row_scalar(LHS const* __lhs, RHS const* __rhs,
                                                 RESULT* __result,
                                                 std::index_sequence<Js...>) noexcept {
        ((__result[I * ColumnsRHS + Js] =
            dot_ij<I, Js>(__lhs, __rhs, std::make_index_sequence<ColumnsLHS>{})), ...);
    }

    template <std::size_t... Is>
    static constexpr inline void gemm_small_scalar(LHS const* __lhs, RHS const* __rhs,
                                                   RESULT* __result,
                                                   std::index_sequence<Is...>) noexcept {
        (gemm_row_scalar<Is>(__lhs, __rhs, __result,
                             std::make_index_sequence<ColumnsRHS>{}), ...);
    }

    // --- Multi-register GEMM (ColumnsRHS > W) ---

    template <std::size_t I>
    static inline void gemm_row_multi(LHS const* __lhs, RHS const* __rhs,
                                      RESULT* __result) noexcept {
        using batch_type = xsimd::batch<RESULT>;

        // SIMD accumulators for full output chunks
        batch_type acc[NW_M > 0 ? NW_M : 1];
        if constexpr (NW_M > 0) {
            for (size_type j = 0; j < NW_M; ++j)
                acc[j] = batch_type(RESULT(0));
        }

        // Scalar tail accumulators
        RESULT tail[TAIL_M > 0 ? TAIL_M : 1];
        if constexpr (TAIL_M > 0) {
            for (size_type j = 0; j < TAIL_M; ++j)
                tail[j] = RESULT(0);
        }

        // Accumulate over K (constexpr bound — compiler fully unrolls)
        for (size_type k = 0; k < ColumnsLHS; ++k) {
            auto a_val = static_cast<RESULT>(__lhs[I * ColumnsLHS + k]);
            if constexpr (NW_M > 0) {
                auto a_bc = batch_type(a_val);
                for (size_type j = 0; j < NW_M; ++j)
                    acc[j] += a_bc * batch_type::load_unaligned(
                        __rhs + k * ColumnsRHS + j * W);
            }
            if constexpr (TAIL_M > 0) {
                for (size_type j = 0; j < TAIL_M; ++j)
                    tail[j] += a_val * static_cast<RESULT>(
                        __rhs[k * ColumnsRHS + NW_M * W + j]);
            }
        }

        // Store results
        RESULT* res_row = __result + I * ColumnsRHS;
        if constexpr (NW_M > 0) {
            for (size_type j = 0; j < NW_M; ++j)
                acc[j].store_unaligned(res_row + j * W);
        }
        if constexpr (TAIL_M > 0) {
            for (size_type j = 0; j < TAIL_M; ++j)
                res_row[NW_M * W + j] = tail[j];
        }
    }

    template <std::size_t... Is>
    static inline void gemm_small_multi(LHS const* __lhs, RHS const* __rhs,
                                        RESULT* __result,
                                        std::index_sequence<Is...>) noexcept {
        (gemm_row_multi<Is>(__lhs, __rhs, __result), ...);
    }

    // --- GEMM dispatch ---

    static inline void gemm_small(LHS const* __lhs, RHS const* __rhs,
                                  RESULT* __result) noexcept {
        if constexpr (ColumnsRHS > W) {
            gemm_small_multi(__lhs, __rhs, __result,
                             std::make_index_sequence<RowsLHS>{});
        } else if constexpr (ColumnsRHS == W) {
            gemm_small_simd(__lhs, __rhs, __result,
                            std::make_index_sequence<RowsLHS>{});
        } else {
            gemm_small_scalar(__lhs, __rhs, __result,
                              std::make_index_sequence<RowsLHS>{});
        }
    }
};

} // namespace detail

#endif // TMECH_HAS_XSIMD

#endif // GEMM_KERNELS_SMALL_H
