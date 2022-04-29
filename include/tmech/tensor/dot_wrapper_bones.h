/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef DOT_WRAPPER_MEAT_H
#define DOT_WRAPPER_MEAT_H


namespace detail {

class dot_wrapper
{
public:
    using size_type = std::size_t;

    dot_wrapper(){}

    template<typename _DerivedLHS, typename _DerivedRHS>
    static constexpr inline auto evaluate(tensor_base<_DerivedLHS> const& __lhs_base, tensor_base<_DerivedRHS> const& __rhs_base)noexcept{
        using value_type_LHS = typename _DerivedLHS::value_type;
        using value_type_RHS = typename _DerivedLHS::value_type;
        constexpr bool LHS_raw_data{is_detected<has_raw_data, _DerivedLHS>::value};
        constexpr bool RHS_raw_data{is_detected<has_raw_data, _DerivedRHS>::value};
        constexpr bool LHS_has_evaluate{is_detected<has_evaluate, _DerivedLHS>::value};
        constexpr bool RHS_has_evaluate{is_detected<has_evaluate, _DerivedRHS>::value};

        _DerivedLHS const& lhs{__lhs_base.convert()};
        _DerivedRHS const& rhs{__rhs_base.convert()};

        if constexpr(RHS_raw_data && RHS_has_evaluate){
            evaluate::apply(rhs);
        }

        if constexpr(LHS_raw_data && LHS_has_evaluate){
            evaluate::apply(lhs);
        }

        constexpr auto Size{get_tensor_size<_DerivedLHS::dimension(), _DerivedLHS::rank()>::size};

        if constexpr (LHS_raw_data && RHS_raw_data){
            return evaluate_implementation<Size>(lhs.raw_data(), rhs.raw_data());
        }else if constexpr (!LHS_raw_data && RHS_raw_data){
            tensor<value_type_LHS, _DerivedLHS::dimension(), _DerivedLHS::rank()> lhs_temp{lhs};
            return evaluate_implementation<Size>(lhs_temp.raw_data(), rhs.raw_data());
        }else if constexpr (LHS_raw_data && !RHS_raw_data){
            tensor<value_type_RHS, _DerivedRHS::dimension(), _DerivedRHS::rank()> rhs_temp{rhs};
            return evaluate_implementation<Size>(lhs.raw_data(), rhs_temp.raw_data());
        }else{
            tensor<value_type_LHS, _DerivedLHS::dimension(), _DerivedLHS::rank()> lhs_temp{lhs};
            tensor<value_type_RHS, _DerivedRHS::dimension(), _DerivedRHS::rank()> rhs_temp{rhs};
            return evaluate_implementation<Size>(lhs_temp.raw_data(), rhs_temp.raw_data());
        }
    }

private:
    template <std::size_t _Size, typename _LHS, typename _RHS>
    static constexpr inline auto evaluate_implementation(_LHS const* __lhs, _RHS const* __rhs)noexcept{
        decltype (_LHS()*_RHS()) sum{0};
        for(size_type i{0}; i<_Size; ++i){
            sum += __lhs[i]*__rhs[i];
        }
        return sum;
    }
};

} // NAMESPACE DETAIL

#endif // DOT_WRAPPER_MEAT_H
