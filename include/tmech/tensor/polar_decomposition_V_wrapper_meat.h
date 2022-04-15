/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_V_WRAPPER_MEAT_H
#define POLAR_DECOMPOSITION_V_WRAPPER_MEAT_H

namespace detail {

template <typename _Base, typename _Tensor>
polar_decomposition_V_wrapper<_Base, _Tensor>::polar_decomposition_V_wrapper(_Base const& __base, _Tensor const& __V):
    _base(__base),
    _V(__V)
{}

template <typename _Base, typename _Tensor>
polar_decomposition_V_wrapper<_Base, _Tensor>::polar_decomposition_V_wrapper(polar_decomposition_V_wrapper const & __data):
    _base(__data._base),
    _V(__data._V)
{}

template <typename _Base, typename _Tensor>
constexpr inline auto const& polar_decomposition_V_wrapper<_Base, _Tensor>::operator()(size_type const i, size_type const j)const{
    return _V(i,j);
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_V_wrapper<_Base, _Tensor>::dimension(){
    return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_V_wrapper<_Base, _Tensor>::rank(){
    return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_V_wrapper<_Base, _Tensor>::evaluate(){
    const_cast<_Base&>(_base).evaluate();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_V_wrapper<_Base, _Tensor>::raw_data()const{
    return _V.raw_data();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_V_wrapper<_Base, _Tensor>::derivative()const{
    return polar_decomposition_dV_wrapper<_Base, typename _Base::tensor4>(_base, _base._dV);
}

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_V_WRAPPER_MEAT_H
