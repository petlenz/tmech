/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POW_TENSOR_DERIVATIVE_WRAPPER_MEAT_H
#define POW_TENSOR_DERIVATIVE_WRAPPER_MEAT_H


namespace detail {

template <typename _Base, typename _Tensor>
pow_tensor_derivative_wrapper<_Base, _Tensor>::pow_tensor_derivative_wrapper(_Base const & __base, _Tensor const& __derivative)noexcept:
    _base(__base),
    _derivative(__derivative)
{}

template <typename _Base, typename _Tensor>
pow_tensor_derivative_wrapper<_Base, _Tensor>::pow_tensor_derivative_wrapper(pow_tensor_derivative_wrapper const & __data)noexcept:
    _base(__data._base),
    _derivative(__data._derivative)
{}

template <typename _Base, typename _Tensor>
constexpr inline auto const& pow_tensor_derivative_wrapper<_Base, _Tensor>::operator()(size_type const i, size_type const j, size_type const k, size_type const l)const noexcept{
    return _derivative(i,j,k,l);
}

template <typename _Base, typename _Tensor>
constexpr inline auto pow_tensor_derivative_wrapper<_Base, _Tensor>::dimension()noexcept{
    return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto pow_tensor_derivative_wrapper<_Base, _Tensor>::rank()noexcept{
    return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto pow_tensor_derivative_wrapper<_Base, _Tensor>::evaluate()noexcept{
    const_cast<_Base&>(_base).evaluate_derivatives();
}

template <typename _Base, typename _Tensor>
constexpr inline auto pow_tensor_derivative_wrapper<_Base, _Tensor>::raw_data()const noexcept{
    return _derivative.raw_data();
}

} // NAMESPACE DETAIL

#endif // POW_TENSOR_DERIVATIVE_WRAPPER_MEAT_H
