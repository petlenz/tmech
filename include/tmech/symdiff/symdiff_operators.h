/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
