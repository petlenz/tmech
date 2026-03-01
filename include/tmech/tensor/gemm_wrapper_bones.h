/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_WRAPPER_BONES_H
#define GEMM_WRAPPER_BONES_H

#include <array>

namespace detail {

//--------------------------------------------------------------------------
// gemm_wrapper_base — scalar kernels shared by all configurations
//--------------------------------------------------------------------------
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
struct gemm_wrapper_base
{
    using size_type = std::size_t;

    static constexpr inline auto gemm_simple_bigger(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        for(size_type i{0}; i<RowsLHS; ++i){
            for(size_type j{0}; j<ColumnsRHS; ++j){
                __result[i*ColumnsRHS+j] = 0;
            }
            for(size_type k{0}; k<RowsRHS; ++k){
                for(size_type j{0}; j<ColumnsRHS; ++j){
                    __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
            }
        }
    }

    // j-i-k loop order: fuses zeroing with accumulation into a single
    // result element — the compiler keeps __result[i*ColumnsRHS+j] in a
    // register across the entire k-loop.  Best for scalar (non-SIMD) code.
    static constexpr inline auto gemm_simple(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        for(size_type j{0}; j<ColumnsRHS; ++j){
            for(size_type i{0}; i<RowsLHS; ++i){
                __result[i*ColumnsRHS+j] = 0;
                for(size_type k{0}; k<RowsRHS; ++k){
                    __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
            }
        }
    }

    // i-k-j loop order: inner j-loop accesses result[] and rhs[]
    // sequentially, enabling auto-vectorisation (broadcast lhs element,
    // FMA across j-vector).  Requires pre-zeroed result buffer.
    static constexpr inline auto gemm_simple_ikj(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        for(size_type i{0}; i<RowsLHS; ++i){
            for(size_type k{0}; k<ColumnsLHS; ++k){
                const auto a = __lhs[i*ColumnsLHS+k];
                for(size_type j{0}; j<ColumnsRHS; ++j){
                    __result[i*ColumnsRHS+j] += a*__rhs[k*ColumnsRHS+j];
                }
            }
        }
    }

    static constexpr inline auto gemv(LHS const * __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        for(size_type i=0; i<RowsLHS; ++i){
            __result[i] = 0;
            for(size_type j=0; j<RowsRHS; ++j){
                __result[i] += __lhs[i*ColumnsLHS+j]*__rhs[j];
            }
        }
    }

    static constexpr inline auto gevm(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        for(size_type i{0}; i<RowsLHS; ++i){
            RESULT sum{0};
            for(size_type j{0}; j<ColumnsLHS; ++j){
                sum += __lhs[j]*__rhs[j*ColumnsRHS+i];
            }
            __result[i] = sum;
        }
    }
};

//--------------------------------------------------------------------------
// gemm_wrapper_xsimd_kernels — thin aggregation of the three kernel tiers
//--------------------------------------------------------------------------
#ifdef TMECH_HAS_XSIMD
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
struct gemm_wrapper_xsimd_kernels
    : public gemm_wrapper_base<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
    , public gemm_kernels_small<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
    , public gemm_kernels_medium<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
    , public gemm_kernels_large<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
{
    using base = gemm_wrapper_base<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>;
    using typename base::size_type;

    using small = gemm_kernels_small<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>;
    using medium = gemm_kernels_medium<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>;
    using large = gemm_kernels_large<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>;

    static constexpr bool simd_supported = std::is_same_v<LHS, RESULT>
                                        && std::is_same_v<RHS, RESULT>
                                        && has_xsimd_batch_v<RESULT>;

    // Tier selection flags
    static constexpr bool is_small_gemm = small::is_small_gemm;
    static constexpr bool is_small_gemv = small::is_small_gemv;
};
#endif

//--------------------------------------------------------------------------
// gemm_wrapper — one clean definition per macro combination
//--------------------------------------------------------------------------

#ifndef TMECH_HAS_XSIMD
// Scalar only
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
class gemm_wrapper
    : public gemm_wrapper_base<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
{
    using base = gemm_wrapper_base<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>;
    using typename base::size_type;
    using base::gemm_simple;
    using base::gemm_simple_bigger;
    using base::gemv;
    using base::gevm;
public:
    static constexpr inline void evaluate(LHS const* __lhs, RHS const* __rhs, RESULT* __result) noexcept {
        if constexpr (ColumnsRHS == 1) { gemv(__lhs, __rhs, __result); return; }
        if constexpr (ColumnsLHS == 1) { gevm(__lhs, __rhs, __result); return; }

        constexpr std::size_t size{3*3*3*3};
        if constexpr (RowsLHS >= size && ColumnsLHS >= size &&
                      RowsRHS >= size && ColumnsRHS >= size) {
            gemm_simple_bigger(__lhs, __rhs, __result);
            return;
        }
        gemm_simple(__lhs, __rhs, __result);
    }
};

#else
// xsimd
template <typename LHS, typename RHS, typename RESULT,
          std::size_t RowsLHS, std::size_t ColumnsLHS,
          std::size_t RowsRHS, std::size_t ColumnsRHS>
class gemm_wrapper
    : public gemm_wrapper_xsimd_kernels<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
{
    using xsimd_base = gemm_wrapper_xsimd_kernels<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>;
    using typename xsimd_base::size_type;
    using xsimd_base::gemm_simple;
    using xsimd_base::gemm_simple_bigger;
    using xsimd_base::gemv;
    using xsimd_base::gevm;
    // Small tier
    using xsimd_base::gemm_small;
    using xsimd_base::gemv_small;
    // Medium tier
    using xsimd_base::gevm_simd;
    using xsimd_base::gemm_simd_regaccum;
    using xsimd_base::gemv_simd_multirow;
    // Large tier
    using xsimd_base::gemm_simd_tiled;
    static constexpr bool simd_supported = xsimd_base::simd_supported;
    static constexpr bool is_small_gemm = xsimd_base::is_small_gemm;
    static constexpr bool is_small_gemv = xsimd_base::is_small_gemv;
public:
    static constexpr inline void evaluate(LHS const* __lhs, RHS const* __rhs, RESULT* __result) noexcept {
        // GEMV — writes directly, no pre-zeroing
        if constexpr (ColumnsRHS == 1) {
            if constexpr (simd_supported) {
                if constexpr (is_small_gemv) { gemv_small(__lhs, __rhs, __result); return; }
                gemv_simd_multirow(__lhs, __rhs, __result); return;
            }
            gemv(__lhs, __rhs, __result);
            return;
        }
        // GEVM — gevm_simd uses +=, needs pre-zeroing
        if constexpr (ColumnsLHS == 1) {
            if constexpr (simd_supported) {
                for (size_type i{0}; i < RowsLHS * ColumnsRHS; ++i) __result[i] = RESULT{0};
                gevm_simd(__lhs, __rhs, __result);
                return;
            }
            gevm(__lhs, __rhs, __result);
            return;
        }
        // GEMM — SIMD paths write directly, no pre-zeroing
        if constexpr (simd_supported) {
            if constexpr (is_small_gemm) { gemm_small(__lhs, __rhs, __result); return; }
            constexpr size_type W_ = xsimd::batch<RESULT>::size;
            constexpr size_type NJ_FULL_ = ColumnsRHS / W_;
            constexpr size_type J_TAIL_ = ColumnsRHS % W_;
            constexpr size_type NJ_REG_ = NJ_FULL_ + (J_TAIL_ > 0 ? 1 : 0);
            constexpr size_type NR_ = (NJ_REG_ > 0) ? (13 / NJ_REG_) : 0;
            constexpr bool fits = (NR_ >= 1 && NJ_FULL_ >= 1);
            if constexpr (fits) {
                gemm_simd_regaccum(__lhs, __rhs, __result);
            } else {
                gemm_simd_tiled(__lhs, __rhs, __result);
            }
            return;
        }
        // Scalar fallback (type not supported by xsimd)
        constexpr std::size_t size{3*3*3*3};
        if constexpr (RowsLHS >= size && ColumnsLHS >= size &&
                      RowsRHS >= size && ColumnsRHS >= size) {
            gemm_simple_bigger(__lhs, __rhs, __result);
            return;
        }
        gemm_simple(__lhs, __rhs, __result);
    }
};

#endif

} // NAMESPACE DETAIL

#endif // GEMM_WRAPPER_BONES_H
