/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POSITIVE_NEGATIVE_DECOMPOSITION_NEG_WRAPPER_BONES_H
#define POSITIVE_NEGATIVE_DECOMPOSITION_NEG_WRAPPER_BONES_H

namespace detail {

template <typename Base, typename Tensor>
class positive_negative_decomposition_neg_wrapper : public tensor_base<positive_negative_decomposition_neg_wrapper<Base, Tensor>>
{
public:
    using size_type  = std::size_t;
    using value_type = typename Tensor::value_type;

    positive_negative_decomposition_neg_wrapper(Base const & __base, Tensor const& __R)noexcept;

    positive_negative_decomposition_neg_wrapper(positive_negative_decomposition_neg_wrapper const & __data)noexcept;

    constexpr inline auto const& operator()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto raw_data()const noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    Tensor const& V;
    Base const& base;
};

} // NAMESPACE DETAIL
#endif // POSITIVE_NEGATIVE_DECOMPOSITION_NEG_WRAPPER_BONES_H
