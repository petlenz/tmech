// Copyright 2021 Peter Lenz
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
#ifndef BASIS_CHANGE_WRAPPER_MEAT_H
#define BASIS_CHANGE_WRAPPER_MEAT_H


namespace detail {

template <typename _Tensor, typename _Sequence>
basis_change_wrapper<_Tensor, _Sequence>::basis_change_wrapper(_Tensor __data):
    _data(__data)
{}

template <typename _Tensor, typename _Sequence>
basis_change_wrapper<_Tensor, _Sequence>::basis_change_wrapper(basis_change_wrapper const& __data):
    _data(__data._data)
{}

template <typename _Tensor, typename _Sequence>
template<typename ...Indicies>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::operator()(Indicies... indices)const{
    return tuple_call(_data, std::make_tuple(indices...), sequence());
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::dimension(){
    return data_type_tensor::dimension();
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::rank(){
    return data_type_tensor::rank();
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::evaluate(){
    evaluate::apply(_data);
}

} // NAMESPACE DETAIL

#endif // BASIS_CHANGE_WRAPPER_MEAT_H
