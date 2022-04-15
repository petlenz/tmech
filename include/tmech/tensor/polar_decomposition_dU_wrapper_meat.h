/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_DU_WRAPPER_MEAT_H
#define POLAR_DECOMPOSITION_DU_WRAPPER_MEAT_H

namespace detail {

template <typename _Base, typename _Tensor>
polar_decomposition_dU_wrapper<_Base, _Tensor>::polar_decomposition_dU_wrapper(_Base const & __base, _Tensor const& __dU):
    _base(__base),
    _dU(__dU)
{}

template <typename _Base, typename _Tensor>
polar_decomposition_dU_wrapper<_Base, _Tensor>::polar_decomposition_dU_wrapper(polar_decomposition_dU_wrapper const & __data):
    _base(__data._base),
    _dU(__data._dU)
{}

template <typename _Base, typename _Tensor>
constexpr inline auto const& polar_decomposition_dU_wrapper<_Base, _Tensor>::operator()(size_type const i, size_type const j, size_type const k, size_type const l)const{
    return _dU(i,j,k,l);
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_dU_wrapper<_Base, _Tensor>::dimension(){
    return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_dU_wrapper<_Base, _Tensor>::rank(){
    return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_dU_wrapper<_Base, _Tensor>::evaluate(){
    const_cast<_Base&>(_base).evaluate_derivatives();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_dU_wrapper<_Base, _Tensor>::raw_data()const{
    return _dU.raw_data();
}

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_DU_WRAPPER_MEAT_H
