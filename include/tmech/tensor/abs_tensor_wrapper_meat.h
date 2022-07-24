/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ABS_TENSOR_WRAPPER_MEAT_H
#define ABS_TENSOR_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _Tensor>
constexpr abs_tensor_wrapper<_Tensor>::abs_tensor_wrapper(data_type_tensor const& __data)noexcept:
    _data(__data)
{}

/**
 * Copy constructor
 */
template <typename _Tensor>
constexpr abs_tensor_wrapper<_Tensor>::abs_tensor_wrapper(abs_tensor_wrapper const& __data)noexcept:
    basetype(__data),
    _data(__data._data)
{}
//@}

/**
* @name Data
*/
/**
* @brief Returns the element at the specified position in the expression.
* @param indicies A list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename _Tensor>
template<typename ...Indicies>
constexpr inline auto abs_tensor_wrapper<_Tensor>::operator ()(Indicies ... __indicies)const noexcept{
    return std::fabs(_data(__indicies...));
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
* @return dimension
*/
template <typename _Tensor>
constexpr inline auto abs_tensor_wrapper<_Tensor>::dimension()noexcept{
    return data_type_tensor::dimension();
}

/**
 * @brief Returns the rank.
 * @return rank
 */
template <typename _Tensor>
constexpr inline auto abs_tensor_wrapper<_Tensor>::rank()noexcept{
    return data_type_tensor::rank();
}
//@}

/**
* @name Evaluation of expression
*/
/**
* @brief Evaluates the underlying tensor expression.
*/
template <typename _Tensor>
constexpr inline auto abs_tensor_wrapper<_Tensor>::evaluate()noexcept{
    evaluate::apply(_data);
}
//@}
} // NAMESPACE DETAIL


#endif // ABS_TENSOR_WRAPPER_MEAT_H
