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
basis_change_wrapper<_Tensor, _Sequence>::basis_change_wrapper(_Tensor __data):
    _data(__data)
{}

template <typename _Tensor, typename _Sequence>
basis_change_wrapper<_Tensor, _Sequence>::basis_change_wrapper(basis_change_wrapper const& __data):
    _data(__data._data)
{}

template <typename _Tensor, typename _Sequence>
template<typename ...Indicies>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::operator()(Indicies... indices)const{
    return tuple_call(_data, std::make_tuple(indices...), sequence());
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::dimension(){
    return data_type_tensor::dimension();
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::rank(){
    return data_type_tensor::rank();
}

template <typename _Tensor, typename _Sequence>
constexpr inline auto basis_change_wrapper<_Tensor, _Sequence>::evaluate(){
    evaluate::apply(_data);
}

} // NAMESPACE DETAIL

#endif // BASIS_CHANGE_WRAPPER_MEAT_H
