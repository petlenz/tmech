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
#ifndef TENSOR_BASIS_CHANGE_WRAPPER_MEAT_H
#define TENSOR_BASIS_CHANGE_WRAPPER_MEAT_H

namespace detail {

template <typename _Expr, typename _Sequence>
constexpr tensor_basis_change_wrapper<_Expr, _Sequence>::tensor_basis_change_wrapper(){}

template <typename _Expr, typename _Sequence>
template<typename Data>
constexpr inline auto tensor_basis_change_wrapper<_Expr, _Sequence>::operator()(Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Expr, typename _Sequence>
constexpr inline std::ostream& tensor_basis_change_wrapper<_Expr, _Sequence>::print(std::ostream & __data)const{
    if constexpr (std::is_same_v<_Sequence, tmech::sequence<2,1>> && tensor_info_lhs::rank() == 2){
        print_trans(__data);
    }else{
        print_general(__data);
    }
    return __data;
}

template <typename _Expr, typename _Sequence>
template<typename _Data>
constexpr inline auto tensor_basis_change_wrapper<_Expr, _Sequence>::get_value(_Data const& __data)const{
    return tmech::basis_change<_Sequence>(static_cast<const variable_base<_Expr>&>(_expr).value(__data));
}

template <typename _Expr, typename _Sequence>
template<typename _Data>
constexpr inline auto tensor_basis_change_wrapper<_Expr, _Sequence>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
}

template <typename _Expr, typename _Sequence>
constexpr inline auto tensor_basis_change_wrapper<_Expr, _Sequence>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
}

template <typename _Expr, typename _Sequence>
constexpr inline auto tensor_basis_change_wrapper<_Expr, _Sequence>::print_trans(std::ostream & __os)const{
    __os<<"trans("<<_expr<<")";
}

template <typename _Expr, typename _Sequence>
constexpr inline auto tensor_basis_change_wrapper<_Expr, _Sequence>::print_general(std::ostream & __os)const{
    __os<<"basis_change<";
    print_sequence(__os, _Sequence());
    __os<<">("<<_expr<<")";
}

template <typename _Expr, typename _Sequence>
template<std::size_t _First, std::size_t ..._Numbers>
constexpr auto tensor_basis_change_wrapper<_Expr, _Sequence>::print_sequence(std::ostream & __os, tmech::sequence<_First, _Numbers...>){
    __os<<_First<<",";
    print_sequence(__os, tmech::sequence<_Numbers...>());
}

template <typename _Expr, typename _Sequence>
template<std::size_t _Last>
constexpr auto tensor_basis_change_wrapper<_Expr, _Sequence>::print_sequence(std::ostream & __os, tmech::sequence<_Last>){
    __os<<_Last;
}

}
#endif // TENSOR_BASIS_CHANGE_WRAPPER_MEAT_H
