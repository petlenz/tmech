/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
