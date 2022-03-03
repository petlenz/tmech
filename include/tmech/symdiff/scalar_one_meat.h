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
#ifndef SCALAR_ONE_MEAT_H
#define SCALAR_ONE_MEAT_H


template<typename _T>
constexpr scalar_one<_T>::scalar_one(){}

template<typename _T>
template<typename _Data>
constexpr inline typename  scalar_one<_T>::data_type scalar_one<_T>::operator()(_Data const& __data) const{
    return get_value(__data);
}

template<typename _T>
inline std::string scalar_one<_T>::get_string()const{
    return std::string("1");
}

template<typename _T>
constexpr inline std::ostream& scalar_one<_T>::print(std::ostream & __os)const{
    __os<<1;
    return __os;
}

template<typename _T>
template<typename _Data>
constexpr inline typename scalar_one<_T>::data_type scalar_one<_T>::get_value(_Data const& /*__data*/)const{
    return 1;
}

#endif // SCALAR_ONE_MEAT_H
