/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POSITIVE_NEGATIVE_DECOMPOSITION_POS_WRAPPER_MEAT_H
#define POSITIVE_NEGATIVE_DECOMPOSITION_POS_WRAPPER_MEAT_H

namespace detail {

template <typename Base, typename Tensor>
positive_negative_decomposition_pos_wrapper<Base, Tensor>::positive_negative_decomposition_pos_wrapper(Base const& __base, Tensor const& __V)noexcept:
    base(__base),
    V(__V)
{}

template <typename Base, typename Tensor>
positive_negative_decomposition_pos_wrapper<Base, Tensor>::positive_negative_decomposition_pos_wrapper(positive_negative_decomposition_pos_wrapper const & __data)noexcept:
    base(__data.base),
    V(__data.V)
{}

template <typename Base, typename Tensor>
constexpr inline auto const& positive_negative_decomposition_pos_wrapper<Base, Tensor>::operator()(size_type const i, size_type const j)const noexcept{
    return V(i,j);
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_pos_wrapper<Base, Tensor>::dimension()noexcept{
    return Tensor::dimension();
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_pos_wrapper<Base, Tensor>::rank()noexcept{
    return Tensor::rank();
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_pos_wrapper<Base, Tensor>::evaluate()noexcept{
    const_cast<Base&>(base).evaluate_positive();
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_pos_wrapper<Base, Tensor>::raw_data()const noexcept{
    return V.raw_data();
}

} // NAMESPACE DETAIL
#endif // POSITIVE_NEGATIVE_DECOMPOSITION_POS_WRAPPER_MEAT_H
