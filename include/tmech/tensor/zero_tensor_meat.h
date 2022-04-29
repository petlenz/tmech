/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ZERO_TENSOR_MEAT_H
#define ZERO_TENSOR_MEAT_H


/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr zeros<T, Dim, Rank>::zeros()noexcept
{}
//@}


/**
* @name Data
*/
/**
* @brief Returns a zeros.
* @param indicies A list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indices>
constexpr inline auto zeros<T, Dim, Rank>::operator ()(const Indices... /*indices*/)const noexcept{
    return static_cast<value_type>(0);
}
//@}


/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto zeros<T, Dim, Rank>::dimension()noexcept{
    return Dim;
}

/**
 * Returns the rank.
 */
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto zeros<T, Dim, Rank>::rank()noexcept{
    return Rank;
}
//@}

#endif // ZERO_TENSOR_MEAT_H
