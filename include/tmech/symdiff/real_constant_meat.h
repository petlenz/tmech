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
#ifndef REAL_CONSTANT_MEAT_H
#define REAL_CONSTANT_MEAT_H

template<typename _T, long long int _L, long long int _R, long long int _Exp>
real<_T, _L, _R, _Exp>::real(){
    std::stringstream strout;
    strout << "0." << _L << _R << "e" << _Exp;
    strout >> _value;
}

template<typename _T, long long int _L, long long int _R, long long int _Exp>
real<_T, _L, _R, _Exp>::real(real const& __data):
    _value(__data._value)
{}

template<typename _T, long long int _L, long long int _R, long long int _Exp>
template<typename _Data>
constexpr inline auto real<_T, _L, _R, _Exp>::operator()(_Data const& __data) const{
    return get_value(__data);
}

template<typename _T, long long int _L, long long int _R, long long int _Exp>
inline std::string real<_T, _L, _R, _Exp>::get_string()const{
    return std::to_string(_value);
}

template<typename _T, long long int _L, long long int _R, long long int _Exp>
constexpr inline std::ostream& real<_T, _L, _R, _Exp>::print(std::ostream & __os)const{
    __os<<_value;
    return __os;
}

template<typename _T, long long int _L, long long int _R, long long int _Exp>
template<typename _Data>
constexpr inline auto real<_T, _L, _R, _Exp>::get_value(_Data const& /*__data*/)const{
    return static_cast<_T>(_value);
}
#endif // REAL_CONSTANT_MEAT_H
