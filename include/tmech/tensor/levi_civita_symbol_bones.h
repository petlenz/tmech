/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef LEVI_CIVITA_SYMBOL_BONES_H
#define LEVI_CIVITA_SYMBOL_BONES_H


/**
* @class levi_civita
* @brief Levi-Civita symbol. See https://en.wikipedia.org/wiki/Levi-Civita_symbol
* for more details.
*
* @tparam T The type of the holding elements.
* @tparam Dim Dimension of the Levi-Civita symbol. Only supported Dim=2,3,4.
*/
template <typename T, std::size_t Dim>
class levi_civita : public tensor_base<levi_civita<T, Dim>>
{
public:
    using value_type = T;
    using size_type = std::size_t;

    static_assert(Dim != 2 || Dim != 3 || Dim != 4,
    "levi_civita: dimension is not supported. Only Dim=2,3,4");

    constexpr levi_civita();

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

    template<std::size_t DIM = Dim, typename = typename std::enable_if<(DIM == 2)>::type>
    constexpr inline auto operator ()(size_type const i, size_type const j)const;

    template<std::size_t DIM = Dim, typename = typename std::enable_if<(DIM == 3)>::type>
    constexpr inline auto operator ()(size_type const i, size_type const j, size_type const k)const;

    template<std::size_t DIM = Dim, typename = typename std::enable_if<(DIM == 4)>::type>
    constexpr inline auto operator ()(size_type const i, size_type const j, size_type const k, size_type const l)const;
};

#endif // LEVI_CIVITA_SYMBOL_BONES_H
