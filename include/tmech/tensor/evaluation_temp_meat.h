/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EVALUATION_TEMP_MEAT_H
#define EVALUATION_TEMP_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename _Tensor>
constexpr eval_tensor_wrapper<_Tensor>::eval_tensor_wrapper(_Tensor __data):
    _data(__data),
    _data_base(__data)
{}

/**
 * @brief Copy constructor
 */
template <typename _Tensor>
constexpr eval_tensor_wrapper<_Tensor>::eval_tensor_wrapper(eval_tensor_wrapper const& __data):
    _data(__data._data),
    _data_base(__data._data_base)
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
template <typename _Tensor>
template<typename ...Indices>
constexpr inline auto eval_tensor_wrapper<_Tensor>::operator()(Indices const... indices)const{
    return _data(indices...);
}

template <typename _Tensor>
constexpr inline auto eval_tensor_wrapper<_Tensor>::raw_data()const{
    return _data.raw_data();
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
*/
template <typename _Tensor>
constexpr inline auto eval_tensor_wrapper<_Tensor>::dimension(){
    return data_type_tensor::dimension();
}

/**
* @brief Returns the rank.
*/
template <typename _Tensor>
constexpr inline auto eval_tensor_wrapper<_Tensor>::rank(){
    return data_type_tensor::rank();
}
//@}

} // NAMESPACE DETAIL

#endif // EVALUATION_TEMP_MEAT_H
