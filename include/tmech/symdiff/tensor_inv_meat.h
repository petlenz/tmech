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
#ifndef TENSOR_INV_MEAT_H
#define TENSOR_INV_MEAT_H

namespace detail {

template <typename _Expr, typename ..._Sequences>
inv_tensor<_Expr, _Sequences...>::inv_tensor():
    _expr(_Expr())
{}

template <typename _Expr, typename ..._Sequences>
template<typename _Data>
constexpr inline auto const& inv_tensor<_Expr, _Sequences...>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Expr, typename ..._Sequences>
constexpr inline std::ostream& inv_tensor<_Expr, _Sequences...>::print(std::ostream & __os)const{
    __os<<"inv("<<_expr<<")";
    return __os;
}

template <typename _Expr, typename ..._Sequences>
template<typename _Data>
constexpr inline auto const& inv_tensor<_Expr, _Sequences...>::get_value(_Data const& /*__data*/)const{
    return _data;
}

template <typename _Expr, typename ..._Sequences>
template<typename _Data>
constexpr inline auto inv_tensor<_Expr, _Sequences...>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
    if(!this->_update){
        _data = tmech::inv(static_cast<const variable_base<_Expr>&>(_expr).value(__data));
        this->_update = true;
    }
}

template <typename _Expr, typename ..._Sequences>
constexpr inline auto inv_tensor<_Expr, _Sequences...>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
    this->_update = false;
}

}


#endif // TENSOR_INV_MEAT_H
