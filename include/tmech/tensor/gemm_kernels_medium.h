/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_KERNELS_MEDIUM_H
#define GEMM_KERNELS_MEDIUM_H

#ifdef TMECH_HAS_XSIMD

namespace detail {

// Loop-based register-accumulated GEMM/GEMV kernels.
// Condition: ColumnsRHS > W and register-accumulation fits.
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
struct gemm_kernels_medium
{
    using size_type = std::size_t;

    static inline void gevm_simd(LHS const* TMECH_RESTRICT __lhs, RHS const* TMECH_RESTRICT __rhs, RESULT* TMECH_RESTRICT __result) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        constexpr size_type W = batch_type::size;
        constexpr size_type N = RowsLHS;

        for (size_type j = 0; j < ColumnsLHS; ++j) {
            auto lhs_val = batch_type(static_cast<RESULT>(__lhs[j]));
            const RESULT* rhs_row = __rhs + j * ColumnsRHS;
            size_type i = 0;
            for (; i + W <= N; i += W) {
                auto res = batch_type::load_unaligned(__result + i);
                res += lhs_val * batch_type::load_unaligned(rhs_row + i);
                res.store_unaligned(__result + i);
            }
            for (; i < N; ++i) {
                __result[i] += __lhs[j] * rhs_row[i];
            }
        }
    }

    // ---- Register-accumulated GEMM block helper ----
    // Processes NR_BLK output rows starting at i_start, accumulating
    // in SIMD registers across the full k-loop (zero intermediate
    // stores). Writes final values directly — no pre-zeroing needed.
    template <size_type NR_BLK>
    static inline void gemm_regaccum_block(LHS const* TMECH_RESTRICT __lhs, RHS const* TMECH_RESTRICT __rhs,
                                           RESULT* TMECH_RESTRICT __result, size_type i_start) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        constexpr size_type W = batch_type::size;
        constexpr size_type NJ_FULL = ColumnsRHS / W;
        constexpr size_type J_TAIL = ColumnsRHS % W;
        // Use 1 as minimum dimension to avoid zero-length arrays
        constexpr size_type NJ_DIM = NJ_FULL > 0 ? NJ_FULL : 1;
        constexpr size_type TAIL_DIM = J_TAIL > 0 ? J_TAIL : 1;

        // SIMD accumulators — compiler register-allocates at -O3
        // with constexpr dimensions
        batch_type acc[NR_BLK][NJ_DIM];
        if constexpr (NJ_FULL > 0) {
            for (size_type r = 0; r < NR_BLK; ++r)
                for (size_type jj = 0; jj < NJ_FULL; ++jj)
                    acc[r][jj] = batch_type(RESULT(0));
        }

        // Scalar tail accumulators
        RESULT tail[NR_BLK][TAIL_DIM];
        if constexpr (J_TAIL > 0) {
            for (size_type r = 0; r < NR_BLK; ++r)
                for (size_type jj = 0; jj < J_TAIL; ++jj)
                    tail[r][jj] = RESULT(0);
        }

        // Main k-loop: accumulate in registers, no stores
        for (size_type k = 0; k < ColumnsLHS; ++k) {
            const RESULT* rhs_row = __rhs + k * ColumnsRHS;
            for (size_type r = 0; r < NR_BLK; ++r) {
                const auto a_val = __lhs[(i_start + r) * ColumnsLHS + k];
                if constexpr (NJ_FULL > 0) {
                    const auto a_bcast = batch_type(a_val);
                    for (size_type jj = 0; jj < NJ_FULL; ++jj) {
                        acc[r][jj] += a_bcast * batch_type::load_unaligned(rhs_row + jj * W);
                    }
                }
                if constexpr (J_TAIL > 0) {
                    for (size_type jj = 0; jj < J_TAIL; ++jj) {
                        tail[r][jj] += a_val * rhs_row[NJ_FULL * W + jj];
                    }
                }
            }
        }

        // Single store pass — write final results
        for (size_type r = 0; r < NR_BLK; ++r) {
            RESULT* res_row = __result + (i_start + r) * ColumnsRHS;
            if constexpr (NJ_FULL > 0) {
                for (size_type jj = 0; jj < NJ_FULL; ++jj) {
                    acc[r][jj].store_unaligned(res_row + jj * W);
                }
            }
            if constexpr (J_TAIL > 0) {
                for (size_type jj = 0; jj < J_TAIL; ++jj) {
                    res_row[NJ_FULL * W + jj] = tail[r][jj];
                }
            }
        }
    }

    // ---- gemm_simd_regaccum: multi-row register-accumulated GEMM ----
    // Dispatches rows in blocks of NR (compile-time), with a remainder
    // block for the last few rows. No pre-zeroing required.
    static inline void gemm_simd_regaccum(LHS const* TMECH_RESTRICT __lhs, RHS const* TMECH_RESTRICT __rhs,
                                          RESULT* TMECH_RESTRICT __result) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        constexpr size_type W = batch_type::size;
        constexpr size_type NJ_FULL = ColumnsRHS / W;
        constexpr size_type J_TAIL = ColumnsRHS % W;
        constexpr size_type NJ_REG = NJ_FULL + (J_TAIL > 0 ? 1 : 0);
        constexpr size_type NR_raw = (NJ_REG > 0) ? (13 / NJ_REG) : 1;
        constexpr size_type NR = (NR_raw > 4) ? 4 : ((NR_raw < 1) ? 1 : NR_raw);

        constexpr size_type I_FULL = (RowsLHS / NR) * NR;
        constexpr size_type I_REM = RowsLHS % NR;

        for (size_type i = 0; i < I_FULL; i += NR) {
            gemm_regaccum_block<NR>(__lhs, __rhs, __result, i);
        }
        if constexpr (I_REM > 0) {
            gemm_regaccum_block<I_REM>(__lhs, __rhs, __result, I_FULL);
        }
    }

    // ---- gemv_simd_multirow: GEMV with multi-row blocking ----
    // Processes NR rows simultaneously, sharing the RHS vector load
    // and providing multiple independent FMA chains to hide latency.
    // Writes results directly — no pre-zeroing needed.
    static inline void gemv_simd_multirow(LHS const* TMECH_RESTRICT __lhs, RHS const* TMECH_RESTRICT __rhs,
                                          RESULT* TMECH_RESTRICT __result) noexcept {
        using batch_type = xsimd::batch<RESULT>;
        constexpr size_type W = batch_type::size;
        constexpr size_type N = ColumnsLHS;
        constexpr size_type vec_end = (N / W) * W;
        constexpr size_type NR = (RowsLHS >= 4) ? 4 : ((RowsLHS >= 2) ? 2 : 1);
        constexpr size_type I_FULL = (RowsLHS / NR) * NR;

        size_type i = 0;
        for (; i < I_FULL; i += NR) {
            batch_type acc[NR];
            for (size_type r = 0; r < NR; ++r)
                acc[r] = batch_type(RESULT(0));

            size_type j = 0;
            for (; j < vec_end; j += W) {
                auto rhs_vec = batch_type::load_unaligned(__rhs + j);
                for (size_type r = 0; r < NR; ++r) {
                    acc[r] += batch_type::load_unaligned(__lhs + (i + r) * N + j) * rhs_vec;
                }
            }

            for (size_type r = 0; r < NR; ++r) {
                RESULT sum = xsimd::reduce_add(acc[r]);
                for (size_type jj = vec_end; jj < N; ++jj) {
                    sum += __lhs[(i + r) * N + jj] * __rhs[jj];
                }
                __result[i + r] = sum;
            }
        }
        // Remainder rows (one at a time)
        for (; i < RowsLHS; ++i) {
            batch_type acc(RESULT(0));
            size_type j = 0;
            for (; j < vec_end; j += W) {
                acc += batch_type::load_unaligned(__lhs + i * N + j)
                     * batch_type::load_unaligned(__rhs + j);
            }
            RESULT sum = xsimd::reduce_add(acc);
            for (; j < N; ++j) {
                sum += __lhs[i * N + j] * __rhs[j];
            }
            __result[i] = sum;
        }
    }
};

} // namespace detail

#endif // TMECH_HAS_XSIMD

#endif // GEMM_KERNELS_MEDIUM_H
