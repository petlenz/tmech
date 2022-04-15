/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
