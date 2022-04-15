/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_ONE_MEAT_H
#define TENSOR_ONE_MEAT_H

template <typename _T>
tensor_one<_T>::tensor_one(){}

template <typename _T>
template<typename _Data>
constexpr inline auto tensor_one<_T>::operator()(_Data const& /*__data*/){
    return typename detail::get_identity_tensor<data_type>::type();
}

template <typename _T>
constexpr inline std::ostream& tensor_one<_T>::print(std::ostream & __os)const{
    __os<<"I";
    return __os;
}

template <typename _T>
template<typename _Data>
constexpr inline auto tensor_one<_T>::get_value(_Data const& /*__data*/)const{
    return typename detail::get_identity_tensor<data_type>::type();
}

#endif // TENSOR_ONE_MEAT_H
