/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef RANDU_WRAPPER_BONES_H
#define RANDU_WRAPPER_BONES_H

#include "traits.h"

namespace detail {

template <typename T, bool IsFloatingPoint = false>
struct uniform_distribution
{
    using type = std::uniform_int_distribution<T>;
};

template <typename T>
struct uniform_distribution<T, true>
{
    using type = std::uniform_real_distribution<T>;
};

} // NAMESPACE DETAIL

template <typename _T, std::size_t _Dim, std::size_t _Rank>
class randu : public tensor_base<randu<_T, _Dim, _Rank>>
{
    using basetype = tensor_base<randu<_T, _Dim, _Rank>>;
    using uniform_distribution = typename detail::uniform_distribution<_T, std::is_floating_point<_T>::value>::type;
public:
    using value_type = _T;
    using el_type = typename detail::element_type<_T>::value_type;

    randu(el_type __a = el_type(0), el_type __b = el_type(1))noexcept;

    randu(randu const& data)noexcept;

    template<typename ...Indices>
    constexpr inline auto operator()(const Indices... indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    std::random_device dev;
    std::mt19937 rng;
    uniform_distribution dist;
    tensor<value_type, _Dim, _Rank> _data;
};

#endif // RANDU_WRAPPER_BONES_H
