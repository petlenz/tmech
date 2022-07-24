/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef BASIS_CHANGE_WRAPPER_MEAT_H
#define BASIS_CHANGE_WRAPPER_MEAT_H


namespace detail {

template <typename _Tensor, typename _Sequence>
basis_change_wrapper<_Tensor, _Sequence>::basis_change_wrapper(_Tensor __data)noexcept:
    _data(__data)
{}

template <typename _Tensor, typename _Sequence>
basis_change_wrapper<_Tensor, _Sequence>::basis_change_wrapper(basis_change_wrapper const& __data)noexcept:
    basetype(__data),
    _data(__data._data)
{}

template <typename _Tensor, typename _Sequence>
template<typename ...Indicies>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::operator()(Indicies... indices)const noexcept{
    return tuple_call(_data, std::make_tuple(indices...), sequence());
}

template <typename _Tensor, typename _Sequence>
template<typename _Derived/*, typename = std::enable_if<!std::is_const_v<_Tensor> && std::is_reference_v<_Tensor>, bool>::type*/>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::operator=(tensor_base<_Derived> const& __data_base)noexcept{
    using function_loop  = typename meta_for_loop_deep<dimension(), rank()-1>::type;
    const auto& __data{__data_base.convert()};
    evaluate::apply(__data);
    auto func = [&](auto ... numbers){
            tuple_call(_data, std::make_tuple(numbers...), sequence()) = __data(numbers...);
        };
    function_loop::loop(func);
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::dimension()noexcept{
    return data_type_tensor::dimension();
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::rank()noexcept{
    return data_type_tensor::rank();
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::evaluate()noexcept{
    evaluate::apply(_data);
}

} // NAMESPACE DETAIL

#endif // BASIS_CHANGE_WRAPPER_MEAT_H
