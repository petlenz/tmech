/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef IDENTITY_TENSOR_MEAT_H
#define IDENTITY_TENSOR_MEAT_H


/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr eye<T, Dim, Rank>::eye()noexcept
{}
//@}

/**
* @name Data
*/
/**
* Returns the element at the specified position in the expression.
* @param indicies a list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indices>
constexpr inline auto eye<T, Dim, Rank>::operator ()(const Indices... indices)const noexcept{
    return static_cast<value_type>(value(indices...));
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
constexpr inline auto eye<T, Dim, Rank>::dimension()noexcept{
    return Dim;
}

/**
* @brief Returns the rank.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto eye<T, Dim, Rank>::rank()noexcept{
    return Rank;
}
//@}

/**
* @name Private functions
*/
/**
* @brief Recursive function to determine the element at the specified position.
* @param indicies a list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indicies>
constexpr inline auto eye<T, Dim, Rank>::value(size_type const i, size_type const j, Indicies const ... indices)noexcept{
    return (i == j ? 1 : 0)*value(indices...);
}

/**
* @brief Determines the element at the specified position.
* @param i First index specifying the position in the tensor expression.
* @param j Second index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto eye<T, Dim, Rank>::value(size_type const i, size_type const j)noexcept{
    return (i == j ? 1 : 0);
}
//@}


#endif // IDENTITY_TENSOR_MEAT_H
