/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_FUNCTION_WRAPPER_MEAT_H
#define TENSOR_FUNCTION_WRAPPER_MEAT_H

namespace detail {

template <typename _Expr, typename _Func>
tensor_function_wrapper<_Expr, _Func>::tensor_function_wrapper():
    _expr(_Expr())
{}

template <typename _Expr, typename _Func>
template<typename _Data>
constexpr inline auto const& tensor_function_wrapper<_Expr, _Func>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Expr, typename _Func>
constexpr inline std::ostream& tensor_function_wrapper<_Expr, _Func>::print(std::ostream & __os)const{
    __os<<_Func::get_string()<<"("<<_expr<<")";
    return __os;
}

template <typename _Expr, typename _Func>
template<typename _Data>
constexpr inline auto const& tensor_function_wrapper<_Expr, _Func>::get_value(_Data const& __data)const{
    return _value;
}

template <typename _Expr, typename _Func>
template<typename _Data>
constexpr inline auto tensor_function_wrapper<_Expr, _Func>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
    if(!this->_update){
        _value = _Func::apply(static_cast<variable_base<_Expr>&>(_expr).value(__data));
        this->_update = true;
    }
}

template <typename _Expr, typename _Func>
constexpr inline auto tensor_function_wrapper<_Expr, _Func>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
    this->_update = false;
}

}

#endif // TENSOR_FUNCTION_WRAPPER_MEAT_H
