/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef FUNCTION_WRAPPER_MEAT_H
#define FUNCTION_WRAPPER_MEAT_H

namespace detail {

template <typename _Expr, typename _Func>
function_wrapper<_Expr, _Func>::function_wrapper():
    _expr(_Expr())
{}

template <typename _Expr, typename _Func>
template<typename Data>
constexpr inline auto const& function_wrapper<_Expr, _Func>::operator()(Data const& __data){
    reset_imp();
    update_imp(__data);
    return _value;
}

template <typename _Expr, typename _Func>
constexpr inline std::ostream& function_wrapper<_Expr, _Func>::print(std::ostream & __os)const{
    __os<<_Func::get_string()<<"("<<_expr<<")";
    return __os;
}

template <typename _Expr, typename _Func>
template<typename _Data>
constexpr inline auto function_wrapper<_Expr, _Func>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
    if(!this->_update){
        _value = _Func::apply(_expr(__data));
        this->_update = true;
    }
}

template <typename _Expr, typename _Func>
constexpr inline auto function_wrapper<_Expr, _Func>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
    this->_update = false;
}

template <typename _Expr, typename _Func>
template <typename _Data>
constexpr inline auto const &function_wrapper<_Expr, _Func>::get_value(
    [[maybe_unused]] _Data const &__data) const {
  return _value;
}
}
#endif // FUNCTION_WRAPPER_MEAT_H
