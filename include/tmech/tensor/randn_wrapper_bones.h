/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef RANDN_WRAPPER_BONES_H
#define RANDN_WRAPPER_BONES_H

#include "traits.h"

template <typename _T, std::size_t _Dim, std::size_t _Rank>
class randn : public tensor_base<randn<_T, _Dim, _Rank>>
{
    using basetype = tensor_base<randn<_T, _Dim, _Rank>>;
public:
    using value_type = _T;
    using el_type = typename detail::element_type<_T>::value_type;

    randn(el_type mean = 0, el_type stddev = 1)noexcept;

    randn(randn const& data)noexcept;

    template<typename ...Indices>
    constexpr inline auto operator()(const Indices... indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    std::random_device dev;
    std::mt19937 rng;
    std::normal_distribution<el_type> dist;
    tensor<value_type, _Dim, _Rank> _data;
};

#endif // RANDN_WRAPPER_BONES_H
