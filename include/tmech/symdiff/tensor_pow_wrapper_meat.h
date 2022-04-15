/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_POW_WRAPPER_MEAT_H
#define TENSOR_POW_WRAPPER_MEAT_H

namespace detail {

template <typename _Expr, typename _RealExpo>
tensor_pow_wrapper<_Expr, _RealExpo>::tensor_pow_wrapper():
    _expr(_Expr())
{}

template <typename _Expr, typename _RealExpo>
template<typename _Data>
constexpr inline auto const& tensor_pow_wrapper<_Expr, _RealExpo>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Expr, typename _RealExpo>
constexpr inline std::ostream& tensor_pow_wrapper<_Expr, _RealExpo>::print(std::ostream & __os)const{
    __os<<"pow"<<"("<<_expr<<","<<_RealExpo()<<")";
    return __os;
}

template <typename _Expr, typename _RealExpo>
template<typename _Data>
constexpr inline auto const& tensor_pow_wrapper<_Expr, _RealExpo>::get_value(_Data const& /*__data*/)const{
    return _pow_data;
}

template <typename _Expr, typename _RealExpo>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper<_Expr, _RealExpo>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
    if(!this->_update){
        _data = static_cast<const variable_base<_Expr>&>(_expr).value(__data);
        this->_update = true;
    }
}

template <typename _Expr, typename _RealExpo>
constexpr inline auto tensor_pow_wrapper<_Expr, _RealExpo>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
    this->_update = false;
}

}

#endif // TENSOR_POW_WRAPPER_MEAT_H
