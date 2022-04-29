/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_U_WRAPPER_MEAT_H
#define POLAR_DECOMPOSITION_U_WRAPPER_MEAT_H

namespace detail {

template <typename _Base, typename _Tensor>
polar_decomposition_U_wrapper<_Base, _Tensor>::polar_decomposition_U_wrapper(_Base const& __base, _Tensor const& __U)noexcept:
    _base(__base),
    _U(__U)
{}

template <typename _Base, typename _Tensor>
polar_decomposition_U_wrapper<_Base, _Tensor>::polar_decomposition_U_wrapper(polar_decomposition_U_wrapper const & __data)noexcept:
    _base(__data._base),
    _U(__data._U)
{}

template <typename _Base, typename _Tensor>
constexpr inline auto const& polar_decomposition_U_wrapper<_Base, _Tensor>::operator()(size_type const i, size_type const j)const noexcept{
    return _U(i,j);
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_U_wrapper<_Base, _Tensor>::dimension()noexcept{
    return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_U_wrapper<_Base, _Tensor>::rank()noexcept{
    return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_U_wrapper<_Base, _Tensor>::evaluate()noexcept{
    const_cast<_Base&>(_base).evaluate();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_U_wrapper<_Base, _Tensor>::raw_data()const noexcept{
    return _U.raw_data();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_U_wrapper<_Base, _Tensor>::derivative()const noexcept{
    return polar_decomposition_dU_wrapper<_Base, typename _Base::tensor4>(_base, _base._dU);
}

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_U_WRAPPER_MEAT_H
