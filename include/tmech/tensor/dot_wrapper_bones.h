// Copyright 2021 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------
#ifndef DOT_WRAPPER_MEAT_H
#define DOT_WRAPPER_MEAT_H


namespace detail {

class dot_wrapper
{
public:
    using size_type = std::size_t;

    dot_wrapper(){}

    template<typename _DerivedLHS, typename _DerivedRHS>
    static constexpr inline auto evaluate(tensor_base<_DerivedLHS> const& __lhs_base, tensor_base<_DerivedRHS> const& __rhs_base){
        using value_type_LHS = typename _DerivedLHS::value_type;
        using value_type_RHS = typename _DerivedLHS::value_type;
        constexpr bool LHS_raw_data{std::experimental::is_detected<has_raw_data, _DerivedLHS>::value};
        constexpr bool RHS_raw_data{std::experimental::is_detected<has_raw_data, _DerivedRHS>::value};

        _DerivedLHS const& lhs{__lhs_base.convert()};
        _DerivedRHS const& rhs{__rhs_base.convert()};

        if constexpr(RHS_raw_data && std::experimental::is_detected<detail::has_evaluate, _DerivedRHS>::value){
            const_cast<_DerivedRHS&>(__rhs_base).evaluate();
        }

        if constexpr(LHS_raw_data && std::experimental::is_detected<detail::has_evaluate, _DerivedLHS>::value){
            const_cast<_DerivedLHS&>(rhs).evaluate();
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
    static constexpr inline auto evaluate_implementation(_LHS const* __lhs, _RHS const* __rhs){
        decltype (_LHS()*_RHS()) sum{0};
        for(size_type i{0}; i<_Size; ++i){
            sum += __lhs[i]*__rhs[i];
        }
        return sum;
    }
};

} // NAMESPACE DETAIL

#endif // DOT_WRAPPER_MEAT_H
