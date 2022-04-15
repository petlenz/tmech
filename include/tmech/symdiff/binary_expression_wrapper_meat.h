/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef BINARY_EXPRESSION_WRAPPER_MEAT_H
#define BINARY_EXPRESSION_WRAPPER_MEAT_H

namespace detail {

template <typename _LHS, typename _RHS, typename _Op>
binary_expression_wrapper<_LHS, _RHS, _Op>::binary_expression_wrapper():
    _lhs(_LHS()),
    _rhs(_RHS())
{}

template <typename _LHS, typename _RHS, typename _Op>
binary_expression_wrapper<_LHS, _RHS, _Op>::~binary_expression_wrapper()
{}

template <typename _LHS, typename _RHS, typename _Op>
template<typename _Data>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _LHS, typename _RHS, typename _Op>
constexpr inline std::ostream& binary_expression_wrapper<_LHS, _RHS, _Op>::print(std::ostream & __os)const{
    __os<<"("<<_lhs<<print_operator<_Op>::print()<<_rhs<<")";
    return __os;
}

template <typename _LHS, typename _RHS, typename _Op>
template<typename _Data>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::get_value(_Data const& __data)const{
    return _Op::apply(static_cast<const variable_base<_LHS>&>(_lhs).value(__data),
                      static_cast<const variable_base<_RHS>&>(_rhs).value(__data));
}

template <typename _LHS, typename _RHS, typename _Op>
template<typename _Data>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::update_imp(_Data const& __data){
    static_cast<variable_base<_LHS>&>(_lhs).update(__data);
    static_cast<variable_base<_RHS>&>(_rhs).update(__data);
}

template <typename _LHS, typename _RHS, typename _Op>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::reset_imp(){
    static_cast<variable_base<_LHS>&>(_lhs).reset();
    static_cast<variable_base<_RHS>&>(_rhs).reset();
}

}
#endif // BINARY_EXPRESSION_WRAPPER_MEAT_H
