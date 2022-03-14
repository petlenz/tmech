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
#ifndef TENSOR_POW_DERIVATIVE_WRAPPER_MEAT_H
#define TENSOR_POW_DERIVATIVE_WRAPPER_MEAT_H

namespace detail {

template <typename _Father>
tensor_pow_wrapper_derivative<_Father>::tensor_pow_wrapper_derivative():
    _father(_Father())
{}

template <typename _Father>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Father>
constexpr inline std::ostream& tensor_pow_wrapper_derivative<_Father>::print(std::ostream & __os)const{
    __os<<"d";
    _father.print(__os);
    return __os;
}

template <typename _Father>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::get_value(_Data const& __data)const{
    return _father._pow_data.derivative();
}

template <typename _Father>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::update_imp(_Data const& __data){
    _father.update_imp(__data);
}

template <typename _Father>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::reset_imp(){
    _father.reset_imp();
}

}
#endif // TENSOR_POW_DERIVATIVE_WRAPPER_MEAT_H
