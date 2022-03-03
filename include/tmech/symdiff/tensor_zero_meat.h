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
#ifndef TENSOR_ZERO_MEAT_H
#define TENSOR_ZERO_MEAT_H

template<typename _T>
tensor_zero<_T>::tensor_zero(){}

template<typename _T>
template<typename _Data>
constexpr inline auto tensor_zero<_T>::operator()(_Data const& /*__data*/) const{
    return typename detail::get_zero_tensor<data_type>::type();
}

template<typename _T>
constexpr inline std::ostream& tensor_zero<_T>::print(std::ostream & __os)const{
    __os<<"0"<<detail::get_tensor_info<_T>::rank();
    return __os;
}

template<typename _T>
template<typename _Data>
constexpr inline auto tensor_zero<_T>::get_value(_Data const& /*__data*/)const{
    return typename detail::get_zero_tensor<data_type>::type();
}

#endif // TENSOR_ZERO_MEAT_H
