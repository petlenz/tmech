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
