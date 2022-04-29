/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef NEGATIVE_TENSOR_WRAPPER_MEAT_H
#define NEGATIVE_TENSOR_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename Tensor>
constexpr negative_tensor_wrapper<Tensor>::negative_tensor_wrapper(data_type_tensor const& data)noexcept:
    _data(data)
{}

/**
 * @brief Copy constructor
 */
template <typename Tensor>
constexpr negative_tensor_wrapper<Tensor>::negative_tensor_wrapper(negative_tensor_wrapper const& data)noexcept:
    _data(data._data)
{}
//@}

/**
* @name Data
*/
/**
* @brief Returns the element at the specified position in the expression.
* @param indicies a list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename Tensor>
template<typename ...Indices>
constexpr inline auto negative_tensor_wrapper<Tensor>::operator ()(Indices ... indices)const noexcept{
    return -_data(indices...);
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
*/
template <typename Tensor>
constexpr inline auto negative_tensor_wrapper<Tensor>::dimension()noexcept{
    return data_type_tensor::dimension();
}

/**
* @brief Returns the rank.
*/
template <typename Tensor>
constexpr inline auto negative_tensor_wrapper<Tensor>::rank()noexcept{
    return data_type_tensor::rank();
}
//@}

/**
* @name Evaluation of tensor expression
*/
/**
* @brief Evaluates the underlying left and right hand side tensor expression.
*/
template <typename Tensor>
constexpr inline auto negative_tensor_wrapper<Tensor>::evaluate()noexcept{
    evaluate::apply(_data);
}
//@}

} // NAMESPACE DETAIL

#endif // NEGATIVE_TENSOR_WRAPPER_MEAT_H
