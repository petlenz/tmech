/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
