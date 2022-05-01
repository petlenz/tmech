/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_OUTER_PRODUCT_WRAPPER_MEAT_H
#define TENSOR_OUTER_PRODUCT_WRAPPER_MEAT_H

namespace detail {

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::tensor_outer_product_wrapper():
    _lhs(_ExprLHS()),
    _rhs(_ExprRHS())
{}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<typename Data>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::operator()(Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline std::ostream& tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print(std::ostream & __os)const{
    if constexpr (std::is_same_v<_SeqLHS, tmech::sequence<1,2>> && std::is_same_v<_SeqRHS, tmech::sequence<3,4>>){
        print_otimes(__os);
    }else if constexpr (std::is_same_v<_SeqLHS, tmech::sequence<1,3>> && std::is_same_v<_SeqRHS, tmech::sequence<2,4>>){
        print_otimesu(__os);
    }else if constexpr (std::is_same_v<_SeqLHS, tmech::sequence<1,4>> && std::is_same_v<_SeqRHS, tmech::sequence<2,3>>){
        print_otimesl(__os);
    }else{
        print_general(__os);
    }
    return __os;
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<typename _Data>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::get_value(_Data const& __data)const{
    return tmech::outer_product<_SeqLHS, _SeqRHS>(static_cast<const variable_base<_ExprLHS>&>(_lhs).value(__data),
                                                            static_cast<const variable_base<_ExprRHS>&>(_rhs).value(__data));
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<typename _Data>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::update_imp(_Data const& __data){
    static_cast<variable_base<_ExprLHS>&>(_lhs).update(__data);
    static_cast<variable_base<_ExprRHS>&>(_rhs).update(__data);
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::reset_imp(){
    static_cast<variable_base<_ExprLHS>&>(_lhs).reset();
    static_cast<variable_base<_ExprRHS>&>(_rhs).reset();
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_general(std::ostream & __os)const{
    __os<<"outer<<";
    print_sequence(__os, _SeqLHS());
    __os<<">,<";
    print_sequence(__os, _SeqRHS());
    __os<<">>(";
    _lhs.print(__os);
    __os<<", ";
    _rhs.print(__os);
    __os<<")";
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_otimes(std::ostream & __os)const{
    __os<<"otimes(";
    __os<<_rhs;
    __os<<", ";
    __os<<_lhs;
    __os<<")";
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_otimesu(std::ostream & __os)const{
    __os<<"otimesu(";
    __os<<_rhs;
    __os<<", ";
    __os<<_lhs;
    __os<<")";
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
constexpr inline auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_otimesl(std::ostream & __os)const{
    __os<<"otimesl(";
    __os<<_rhs;
    __os<<", ";
    __os<<_lhs;
    __os<<")";
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<std::size_t First, std::size_t ...Numbers>
constexpr auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_sequence(std::ostream & __os, tmech::sequence<First, Numbers...>){
    __os<<First<<",";
    print_sequence(__os, tmech::sequence<Numbers...>());
}

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
template<std::size_t Last>
constexpr auto tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::print_sequence(std::ostream & __os, tmech::sequence<Last>){
    __os<<Last;
}

}

#endif // TENSOR_OUTER_PRODUCT_WRAPPER_MEAT_H
