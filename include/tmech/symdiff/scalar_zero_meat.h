/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SCALAR_ZERO_MEAT_H
#define SCALAR_ZERO_MEAT_H

template<typename _T>
constexpr scalar_zero<_T>::scalar_zero(){}

template<typename _T>
template<typename Data>
constexpr inline typename scalar_zero<_T>::data_type scalar_zero<_T>::operator()(Data const& __data) const{
    return get_value(__data);
}

template<typename _T>
inline std::string scalar_zero<_T>::get_string()const{
    return std::string("0");
}

template<typename _T>
constexpr inline std::ostream& scalar_zero<_T>::print(std::ostream & __os)const{
    __os<<0;
    return __os;
}

template<typename _T>
template<typename _Data>
constexpr inline typename scalar_zero<_T>::data_type scalar_zero<_T>::get_value(_Data const& __data)const{
    return 0;
}

#endif // SCALAR_ZERO_MEAT_H
