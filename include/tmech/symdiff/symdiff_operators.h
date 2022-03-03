// Copyright 2022 Peter Lenz
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
#ifndef SYMDIFF_OPERATORS_H
#define SYMDIFF_OPERATORS_H

template<typename _LHS,
         typename _RHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_RHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_LHS>::value> * = nullptr>
constexpr inline auto operator+(_LHS, _RHS){
    return typename symdiff::detail::squeezer< symdiff::detail::binary_expression_wrapper<_LHS, _RHS, tmech::detail::operator_add> >::squeezedType();
}

template<typename _LHS,
         typename _RHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_RHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_LHS>::value> * = nullptr>
constexpr inline auto operator-(_LHS, _RHS){
    return typename symdiff::detail::squeezer< symdiff::detail::binary_expression_wrapper<_LHS, _RHS, tmech::detail::operator_sub>>::squeezedType();
}

template<typename _LHS,
         typename _RHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_RHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_LHS>::value> * = nullptr>
constexpr inline auto operator*(_LHS, _RHS){
    if constexpr (symdiff::detail::is_tensor<typename _LHS::data_type>::value && symdiff::detail::is_tensor<typename _RHS::data_type>::value){
        return detail::tensor_inner_product_wrapper<_LHS, _RHS, tmech::sequence<symdiff::detail::get_tensor_info<typename _LHS::data_type>::rank()>, tmech::sequence<1>>();
    }else{
        return typename symdiff::detail::squeezer< symdiff::detail::binary_expression_wrapper<_LHS, _RHS, tmech::detail::operator_mul>>::squeezedType();
    }
}

template<typename _LHS,
         typename _RHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_RHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_LHS>::value> * = nullptr>
constexpr inline auto operator/(_LHS, _RHS){
    return typename symdiff::detail::squeezer< symdiff::detail::binary_expression_wrapper<_LHS, _RHS, tmech::detail::operator_div>>::squeezedType();
}

#endif // SYMDIFF_OPERATORS_H
