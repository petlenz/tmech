/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ZERO_TENSOR_BONES_H
#define ZERO_TENSOR_BONES_H


/**
* @class zeros
* @brief Wrapper for a tensor, which contains only zeros.
* No memory is initialised.
*
* @tparam T The type of the tensor holding the elements.
* @tparam Dim Dimension of the underlying physical problem.
* @tparam Rank Rank of the tensor.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
class zeros : public tensor_base<zeros<T, Dim, Rank>>
{
public:
    using value_type = T;
    using size_type  = std::size_t;

    constexpr zeros()noexcept;

    template<typename ...Indices>
    constexpr inline auto operator()(const Indices... /*indices*/)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;
};


#endif // ZERO_TENSOR_BONES_H
