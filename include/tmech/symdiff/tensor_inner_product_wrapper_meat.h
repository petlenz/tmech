/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_INNER_PRODUCT_WRAPPER_MEAT_H
#define TENSOR_INNER_PRODUCT_WRAPPER_MEAT_H

namespace detail {

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::tensor_inner_product_wrapper() {}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<typename Data>
constexpr inline auto const& tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::operator()(Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline std::ostream& tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print(std::ostream & __os)const{
    print_general(__os);
    return __os;
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<typename _Data>
constexpr inline auto const& tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::get_value(_Data const& /*__data*/)const{
    return _data;
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<typename _Data>
constexpr inline auto tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::update_imp(_Data const& __data){
    static_cast<variable_base<_ExprLHS>&>(_lhs).update(__data);
    static_cast<variable_base<_ExprRHS>&>(_rhs).update(__data);
    if(!this->_update){
        _data = tmech::inner_product<_SeqLHS, _SeqRHS>(static_cast<const variable_base<_ExprLHS>&>(_lhs).value(__data),
                                                       static_cast<const variable_base<_ExprRHS>&>(_rhs).value(__data));
        this->_update = true;
    }
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::reset_imp(){
    static_cast<variable_base<_ExprLHS>&>(_lhs).reset();
    static_cast<variable_base<_ExprRHS>&>(_rhs).reset();
    this->_update = false;
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_general(std::ostream & __os)const{
    __os<<"inner_product<<";
    print_sequence(__os, _SeqLHS());
    __os<<">,<";
    print_sequence(__os, _SeqRHS());
    __os<<">>("<<_lhs<<", "<<_rhs<<")";
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<std::size_t _First, std::size_t ..._Numbers>
constexpr auto tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_sequence(std::ostream & __os, tmech::sequence<_First, _Numbers...>){
    __os<<_First<<",";
    print_sequence(__os, tmech::sequence<_Numbers...>());
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<std::size_t _Last>
constexpr auto tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_sequence(std::ostream & __os, tmech::sequence<_Last>){
    __os<<_Last;
}

}
#endif // TENSOR_INNER_PRODUCT_WRAPPER_MEAT_H
