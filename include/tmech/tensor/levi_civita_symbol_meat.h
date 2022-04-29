/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef LEVI_CIVITA_SYMBOL_MEAT_H
#define LEVI_CIVITA_SYMBOL_MEAT_H



/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename T, std::size_t Dim>
constexpr levi_civita<T, Dim>::levi_civita()noexcept{}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the rank.
*/
template <typename T, std::size_t Dim>
constexpr inline auto levi_civita<T, Dim>::rank()noexcept{
    return Dim;
}
/**
* @brief Returns the dimension.
*/
template <typename T, std::size_t Dim>
constexpr inline auto levi_civita<T, Dim>::dimension()noexcept{
    return Dim;
}
//@}

/**
* @name Data
*/
/**
* @brief Returns the element at the specified position in the expression.
* @param i Index specifying the position in the tensor expression.
* @param j Index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim>
template<std::size_t DIM, typename >
constexpr inline auto levi_civita<T, Dim>::operator()(size_type const i, size_type const j)const noexcept{
    return static_cast<value_type>(j) - static_cast<value_type>(i);
}

/**
* @brief Returns the element at the specified position in the expression.
* @param i Index specifying the position in the tensor expression.
* @param j Index specifying the position in the tensor expression.
* @param k Index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim>
template<std::size_t DIM, typename >
constexpr inline auto levi_civita<T, Dim>::operator()(size_type const i, size_type const j, size_type const k)const noexcept{
    return (static_cast<value_type>(j) - static_cast<value_type>(i))
            *(static_cast<value_type>(k) - static_cast<value_type>(j))
            *(static_cast<value_type>(i) - static_cast<value_type>(k))*static_cast<value_type>(0.5);
}

/**
* @brief Returns the element at the specified position in the expression.
* @param i Index specifying the position in the tensor expression.
* @param j Index specifying the position in the tensor expression.
* @param k Index specifying the position in the tensor expression.
* @param l Index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim>
template<std::size_t DIM, typename>
constexpr inline auto levi_civita<T, Dim>::operator()(size_type const i, size_type const j, size_type const k, size_type const l)const noexcept{
    constexpr value_type fac{1./12.};
    return (static_cast<value_type>(j) - static_cast<value_type>(i))
            *(static_cast<value_type>(k) - static_cast<value_type>(i))
            *(static_cast<value_type>(l) - static_cast<value_type>(i))
            *(static_cast<value_type>(k) - static_cast<value_type>(j))
            *(static_cast<value_type>(l) - static_cast<value_type>(j))
            *(static_cast<value_type>(l) - static_cast<value_type>(k))*fac;
}
//@}

#endif // LEVI_CIVITA_SYMBOL_MEAT_H
