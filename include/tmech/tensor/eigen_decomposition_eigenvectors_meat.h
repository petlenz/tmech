/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EIGEN_DECOMPOSITION_EIGENVECTORS_MEAT_H
#define EIGEN_DECOMPOSITION_EIGENVECTORS_MEAT_H

namespace detail {

template <typename Base>
constexpr eigen_decomposition_vectors_wrapper<Base>::eigen_decomposition_vectors_wrapper(Base const& __basis):
    _basis(__basis)
{}

template <typename Base>
constexpr eigen_decomposition_vectors_wrapper<Base>::eigen_decomposition_vectors_wrapper(eigen_decomposition_vectors_wrapper const& __basis):
    _basis(__basis._basis)
{}

template <typename Base>
constexpr inline auto const& eigen_decomposition_vectors_wrapper<Base>::operator[](size_type const __idx)const{
    return _basis._eigvectors[__idx];
}


} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_EIGENVECTORS_MEAT_H
