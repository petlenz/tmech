/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef IDENTITY_TENSOR_BONES_H
#define IDENTITY_TENSOR_BONES_H


/**
* @class eye
* @brief Identity tensor. Only for a Rank which is a multiplier of two.
*
* @tparam T The type of the tensor holding the elements.
* @tparam Dim Dimension of the underlying physical problem.
* @tparam Rank Rank of the tensor. Rank needs to be a multiplier of two.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
class eye : public tensor_base<eye<T, Dim, Rank>>
{
public:
    using value_type = T;
    using size_type = std::size_t;

    static_assert(Rank % 2 == 0,
    "eye: No matching Rank. Only for a Rank which is a multiplier of two");

    constexpr eye()noexcept;

    ~eye()noexcept{}

    template<typename ...Indices>
    constexpr inline auto operator()(const Indices... indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

private:
    template<typename ...Indicies>
    static constexpr inline auto value(size_type const i, size_type const j, Indicies const ... indices)noexcept;

    static constexpr inline auto value(size_type const i, size_type const j)noexcept;
};

#endif // IDENTITY_TENSOR_BONES_H
