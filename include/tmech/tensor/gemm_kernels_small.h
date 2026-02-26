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
// Condition: ColumnsRHS <= W (GEMM) or ColumnsLHS <= W (GEMV),
// where W is the SIMD register width in elements.
// Uses std::index_sequence fold expressions — zero runtime loops.
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
struct gemm_kernels_small
{
private:
    static constexpr std::size_t safe_simd_width() {
        if constexpr (has_xsimd_batch_v<RESULT>) {
            return xsimd::batch<RESULT>::size;
        }
        return 0;
    }

public:
    static constexpr std::size_t W = safe_simd_width();

    // Tier selection flags
    static constexpr bool is_small_gemm = (ColumnsRHS > 1) && (ColumnsRHS <= W);
    static constexpr bool is_small_gemv = (ColumnsRHS == 1) && (ColumnsLHS <= W);

    // ---- gemm_small: fully unrolled GEMM for ColumnsRHS <= W ----

    // Fold-expression FMA chain for one output row
    template <std::size_t... Ks>
    static inline auto fma_row(xsimd::batch<RESULT> const* rhs_rows,
                               LHS const* lhs_row,
                               std::index_sequence<Ks...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        return ((batch_type(static_cast<RESULT>(lhs_row[Ks])) * rhs_rows[Ks]) + ...);
    }

    // Pre-load all K RHS rows into SIMD registers
    template <std::size_t... Ks>
    static inline void preload_rhs(xsimd::batch<RESULT>* rhs_rows,
                                   RHS const* __rhs,
                                   std::index_sequence<Ks...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        ((rhs_rows[Ks] = batch_type::load_unaligned(__rhs + Ks * ColumnsRHS)), ...);
    }

    // SIMD path: one vector per output row (ColumnsRHS == W)
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

    // Scalar fold for one element C[I][J] = sum_k A[I][k] * B[k][J]
    template <std::size_t I, std::size_t J, std::size_t... Ks>
    static constexpr inline RESULT dot_ij(LHS const* __lhs, RHS const* __rhs,
                                          std::index_sequence<Ks...>) noexcept {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + Ks]) *
                 static_cast<RESULT>(__rhs[Ks * ColumnsRHS + J])) + ...);
    }

    // Scalar fold: one row
    template <std::size_t I, std::size_t... Js>
    static constexpr inline void gemm_row_scalar(LHS const* __lhs, RHS const* __rhs,
                                                 RESULT* __result,
                                                 std::index_sequence<Js...>) noexcept {
        ((__result[I * ColumnsRHS + Js] =
            dot_ij<I, Js>(__lhs, __rhs, std::make_index_sequence<ColumnsLHS>{})), ...);
    }

    // Scalar fold: all rows (ColumnsRHS < W)
    template <std::size_t... Is>
    static constexpr inline void gemm_small_scalar(LHS const* __lhs, RHS const* __rhs,
                                                   RESULT* __result,
                                                   std::index_sequence<Is...>) noexcept {
        (gemm_row_scalar<Is>(__lhs, __rhs, __result,
                             std::make_index_sequence<ColumnsRHS>{}), ...);
    }

    static inline void gemm_small(LHS const* __lhs, RHS const* __rhs,
                                  RESULT* __result) noexcept {
        if constexpr (ColumnsRHS == W) {
            gemm_small_simd(__lhs, __rhs, __result,
                            std::make_index_sequence<RowsLHS>{});
        } else {
            gemm_small_scalar(__lhs, __rhs, __result,
                              std::make_index_sequence<RowsLHS>{});
        }
    }

    // ---- gemv_small: fully unrolled GEMV for ColumnsLHS <= W ----

    // SIMD: load, multiply, reduce for each row (ColumnsLHS == W)
    template <std::size_t... Is>
    static inline void gemv_small_simd(LHS const* __lhs, RHS const* __rhs,
                                       RESULT* __result,
                                       std::index_sequence<Is...>) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        auto rhs_vec = batch_type::load_unaligned(__rhs);
        ((void)(__result[Is] = xsimd::reduce_add(
            batch_type::load_unaligned(__lhs + Is * ColumnsLHS) * rhs_vec)), ...);
    }

    // Scalar dot product for row I
    template <std::size_t I, std::size_t... Ks>
    static constexpr inline RESULT dot_row(LHS const* __lhs, RHS const* __rhs,
                                           std::index_sequence<Ks...>) noexcept {
        return ((static_cast<RESULT>(__lhs[I * ColumnsLHS + Ks]) *
                 static_cast<RESULT>(__rhs[Ks])) + ...);
    }

    // Scalar fold: all rows (ColumnsLHS < W)
    template <std::size_t... Is>
    static constexpr inline void gemv_small_scalar(LHS const* __lhs, RHS const* __rhs,
                                                   RESULT* __result,
                                                   std::index_sequence<Is...>) noexcept {
        ((__result[Is] = dot_row<Is>(__lhs, __rhs,
                                     std::make_index_sequence<ColumnsLHS>{})), ...);
    }

    static inline void gemv_small(LHS const* __lhs, RHS const* __rhs,
                                  RESULT* __result) noexcept {
        if constexpr (ColumnsLHS == W) {
            gemv_small_simd(__lhs, __rhs, __result,
                            std::make_index_sequence<RowsLHS>{});
        } else {
            gemv_small_scalar(__lhs, __rhs, __result,
                              std::make_index_sequence<RowsLHS>{});
        }
    }
};

} // namespace detail

#endif // TMECH_HAS_XSIMD

#endif // GEMM_KERNELS_SMALL_H
