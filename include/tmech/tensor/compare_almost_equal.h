/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef COMPARE_ALMOST_EQUAL_H
#define COMPARE_ALMOST_EQUAL_H

namespace detail {

struct compare_almost_equal
{
    template <typename _LHS, typename _RHS, typename _T>
    static inline bool check(_LHS const& __lhs, _RHS const& __rhs, _T const __eps)noexcept{
        using value_type = typename result_type<typename _LHS::value_type, typename _RHS::value_type>::value_type;

        tensor<value_type, _RHS::dimension(), _RHS::rank()> R{__rhs - __lhs};
        return check_detail<decltype (R)::size()>(R.raw_data(), __eps);

//        constexpr bool LHS_raw_data{std::experimental::is_detected<has_raw_data, _LHS>::value};
//        constexpr bool RHS_raw_data{std::experimental::is_detected<has_raw_data, _RHS>::value};
//        constexpr bool LHS_evaluate{std::experimental::is_detected<detail::has_evaluate, _LHS>::value};
//        constexpr bool RHS_evaluate{std::experimental::is_detected<detail::has_evaluate, _RHS>::value};

//        if constexpr (LHS_raw_data && RHS_raw_data){
//            if constexpr(LHS_evaluate){const_cast<_LHS&>(__lhs).evaluate();}
//            if constexpr (RHS_evaluate){const_cast<_RHS&>(__rhs).evaluate();}
//            return check_detail<_LHS::size()>(__lhs.raw_data(), __rhs.raw_data(), __eps);
//        }else if constexpr (!LHS_raw_data && RHS_raw_data){
//            if constexpr (RHS_evaluate){const_cast<_RHS&>(__rhs).evaluate();}
//            tensor<typename _LHS::value_type, _LHS::dimension(), _LHS::rank()> lhs{__lhs};
//            return check_detail<decltype (lhs)::size()>(lhs.raw_data(), __rhs.raw_data(), __eps);
//        }else if constexpr (LHS_raw_data && !RHS_raw_data){
//            if constexpr(LHS_evaluate){const_cast<_LHS&>(__lhs).evaluate();}
//            tensor<typename _RHS::value_type, _RHS::dimension(), _RHS::rank()> rhs{__rhs};
//            return check_detail<decltype (rhs)::size()>(__lhs.raw_data(), rhs.raw_data(), __eps);
//        }else{
//            tensor<typename _RHS::value_type, _RHS::dimension(), _RHS::rank()> rhs{__rhs};
//            tensor<typename _LHS::value_type, _LHS::dimension(), _LHS::rank()> lhs{__lhs};
//            return check_detail<decltype (lhs)::size()>(lhs.raw_data(), rhs.raw_data(), __eps);
//        }
    }

private:
    template <std::size_t _Size, typename _R, typename _T>
    static constexpr inline bool check_detail(_R * __R, _T const __eps)noexcept{
        using R = typename std::remove_const<_R>::type;

        if constexpr (is_complex<R>()){
            typename R::value_type temp[_Size];
            for(std::size_t i{0}; i<_Size; ++i){
                temp[i] = std::abs(__R[i]);
            }
            return check_detail_imp<_Size>(temp, __eps);
        }else if constexpr (!is_complex<R>()){
            return check_detail_imp<_Size>(__R, __eps);
        }
    }

    template <std::size_t _Size, typename _R, typename _T>
    static constexpr inline bool check_detail_imp(_R * __R, _T const __eps)noexcept{
        const auto max{std::max(std::abs(*std::min_element(__R, __R+_Size)), *std::max_element(__R, __R+_Size))};
        return (max < std::abs(__eps));
    }

//    template <std::size_t _Size, typename _LHS, typename _RHS, typename _T>
//    static constexpr inline bool check_detail(_LHS * __lhs, _RHS * __rhs, _T const __eps){
//        using LHS = typename std::remove_const<_LHS>::type;
//        using RHS = typename std::remove_const<_RHS>::type;

//        if constexpr (is_complex<LHS>() && !is_complex<RHS>()){
//            typename LHS::value_type lhs_temp[_Size];
//            for(std::size_t i{0}; i<_Size; ++i){
//                lhs_temp[i] = std::abs(__lhs[i]);
//            }
//            return check_detail_imp<_Size>(lhs_temp, __rhs, __eps);
//        }else if constexpr (!is_complex<LHS>() && is_complex<RHS>()){
//            typename RHS::value_type rhs_temp[_Size];
//            for(std::size_t i{0}; i<_Size; ++i){
//                rhs_temp[i] = std::abs(__rhs[i]);
//            }
//            return check_detail_imp<_Size>(__lhs, rhs_temp, __eps);
//        }else if constexpr (is_complex<LHS>() && is_complex<RHS>()){
//            typename LHS::value_type lhs_temp[_Size];
//            typename RHS::value_type rhs_temp[_Size];
//            for(std::size_t i{0}; i<_Size; ++i){
//                rhs_temp[i] = std::abs(__rhs[i]);
//                lhs_temp[i] = std::abs(__lhs[i]);
//            }
//            return check_detail_imp<_Size>(lhs_temp, rhs_temp, __eps);
//        }else if constexpr (!is_complex<LHS>() && !is_complex<RHS>()){
//            return check_detail_imp<_Size>(__lhs, __rhs, __eps);
//        }
//    }

//    template <std::size_t _Size, typename _LHS, typename _RHS, typename _T>
//    static constexpr inline bool check_detail_imp(_LHS * __lhs, _RHS * __rhs, _T const __eps){
//        const auto max_lhs{std::max(std::abs(*std::min_element(__lhs, __lhs+_Size)), *std::max_element(__lhs, __lhs+_Size))};
//        const auto max_rhs{std::max(std::abs(*std::min_element(__rhs, __rhs+_Size)), *std::max_element(__lhs, __lhs+_Size))};
//        const auto max{std::max(max_lhs, max_rhs)};
//        for(std::size_t i{0}; i<_Size; ++i){
//            if(std::abs(__lhs[i]-__rhs[i]) > std::abs(__eps)*max){
//                return false;
//            }
//        }
//        return true;
//    }
};

}
#endif // COMPARE_ALMOST_EQUAL_H
