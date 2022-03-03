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
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::operator()(_Data const& x){
    reset_imp();
    update_imp(x);
    return get_value(x);
}

template <typename _LHS, typename _RHS, typename _Op>
constexpr inline std::ostream& binary_expression_wrapper<_LHS, _RHS, _Op>::print(std::ostream & os)const{
    os<<"("<<_lhs<<print_operator<_Op>::print()<<_rhs<<")";
    return os;
}

template <typename _LHS, typename _RHS, typename _Op>
template<typename _Data>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::get_value(_Data const& x)const{
    return _Op::apply(static_cast<const variable_base<_LHS>&>(_lhs).value(x),
                      static_cast<const variable_base<_RHS>&>(_rhs).value(x));
}

template <typename _LHS, typename _RHS, typename _Op>
template<typename _Data>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::update_imp(_Data const& x){
    static_cast<variable_base<_LHS>&>(_lhs).update(x);
    static_cast<variable_base<_RHS>&>(_rhs).update(x);
}

template <typename _LHS, typename _RHS, typename _Op>
constexpr inline auto binary_expression_wrapper<_LHS, _RHS, _Op>::reset_imp(){
    static_cast<variable_base<_LHS>&>(_lhs).reset();
    static_cast<variable_base<_RHS>&>(_rhs).reset();
}

}
#endif // BINARY_EXPRESSION_WRAPPER_MEAT_H
