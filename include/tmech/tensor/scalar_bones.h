/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SCALAR_BONES_H
#define SCALAR_BONES_H

namespace detail {

/**
* @class scalar
* @brief Wrapper for scalar value in a tensor expression.
*
* @tparam _T Tensor expression from which the
* inverse should be determined.
*/
template<typename _T>
class scalar
{
public:
    using value_type = _T;

    constexpr scalar(value_type const scalar)noexcept;

    constexpr scalar(scalar const& scalar)noexcept;

    constexpr inline value_type operator()()const noexcept;

    template<typename ...Numbers>
    constexpr inline value_type operator()(Numbers...)const noexcept;

    static constexpr inline auto simple_evaluation()noexcept{
        return true;
    }

    constexpr inline auto direct_access(std::size_t /*__idx*/)const noexcept{
        return scalar_;
    }

private:
    value_type const scalar_;
};

} // NAMESPACE DETAIL

#endif // SCALAR_BONES_H
