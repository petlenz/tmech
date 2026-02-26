/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_KERNELS_LARGE_H
#define GEMM_KERNELS_LARGE_H

#ifdef TMECH_HAS_XSIMD

namespace detail {

// Cache-tiled GEMM kernel for large matrices.
// Condition: register-accumulation doesn't fit.
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
struct gemm_kernels_large
{
    using size_type = std::size_t;

    // ---- gemm_simd_tiled: cache-tiled GEMM for large matrices ----
    // Tiles in the j-dimension so that each tile's accumulators fit
    // in registers. Within each tile, full k-loop with register
    // accumulation. No pre-zeroing required.
    static inline void gemm_simd_tiled(LHS const* __lhs, RHS const* __rhs,
                                       RESULT* __result) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        constexpr size_type W = batch_type::size;
        constexpr size_type JB = 4 * W;      // tile width: 16 (AVX2) or 32 (AVX-512)
        constexpr size_type NJ_TILE = JB / W; // = 4
        constexpr size_type NR_raw = 13 / NJ_TILE;
        constexpr size_type NR_TILE = (NR_raw > 4) ? 4 : ((NR_raw < 1) ? 1 : NR_raw);

        // Full j-tiles
        size_type j0 = 0;
        for (; j0 + JB <= ColumnsRHS; j0 += JB) {
            // Full row blocks
            size_type i = 0;
            for (; i + NR_TILE <= RowsLHS; i += NR_TILE) {
                batch_type acc[NR_TILE][NJ_TILE];
                for (size_type r = 0; r < NR_TILE; ++r)
                    for (size_type jj = 0; jj < NJ_TILE; ++jj)
                        acc[r][jj] = batch_type(RESULT(0));

                for (size_type k = 0; k < ColumnsLHS; ++k) {
                    const RESULT* rhs_ptr = __rhs + k * ColumnsRHS + j0;
                    for (size_type r = 0; r < NR_TILE; ++r) {
                        const auto a_bcast = batch_type(__lhs[(i + r) * ColumnsLHS + k]);
                        for (size_type jj = 0; jj < NJ_TILE; ++jj) {
                            acc[r][jj] += a_bcast * batch_type::load_unaligned(rhs_ptr + jj * W);
                        }
                    }
                }

                for (size_type r = 0; r < NR_TILE; ++r) {
                    RESULT* res_ptr = __result + (i + r) * ColumnsRHS + j0;
                    for (size_type jj = 0; jj < NJ_TILE; ++jj) {
                        acc[r][jj].store_unaligned(res_ptr + jj * W);
                    }
                }
            }
            // Remaining rows
            for (; i < RowsLHS; ++i) {
                batch_type acc[NJ_TILE];
                for (size_type jj = 0; jj < NJ_TILE; ++jj)
                    acc[jj] = batch_type(RESULT(0));

                for (size_type k = 0; k < ColumnsLHS; ++k) {
                    const RESULT* rhs_ptr = __rhs + k * ColumnsRHS + j0;
                    const auto a_bcast = batch_type(__lhs[i * ColumnsLHS + k]);
                    for (size_type jj = 0; jj < NJ_TILE; ++jj) {
                        acc[jj] += a_bcast * batch_type::load_unaligned(rhs_ptr + jj * W);
                    }
                }

                RESULT* res_ptr = __result + i * ColumnsRHS + j0;
                for (size_type jj = 0; jj < NJ_TILE; ++jj) {
                    acc[jj].store_unaligned(res_ptr + jj * W);
                }
            }
        }

        // Remaining j-columns (scalar, typically small)
        if (j0 < ColumnsRHS) {
            const size_type j_rem = ColumnsRHS - j0;
            for (size_type i = 0; i < RowsLHS; ++i) {
                RESULT* res_ptr = __result + i * ColumnsRHS + j0;
                for (size_type jj = 0; jj < j_rem; ++jj)
                    res_ptr[jj] = RESULT(0);
                for (size_type k = 0; k < ColumnsLHS; ++k) {
                    const auto a_val = __lhs[i * ColumnsLHS + k];
                    const RESULT* rhs_ptr = __rhs + k * ColumnsRHS + j0;
                    for (size_type jj = 0; jj < j_rem; ++jj) {
                        res_ptr[jj] += a_val * rhs_ptr[jj];
                    }
                }
            }
        }
    }
};

} // namespace detail

#endif // TMECH_HAS_XSIMD

#endif // GEMM_KERNELS_LARGE_H
