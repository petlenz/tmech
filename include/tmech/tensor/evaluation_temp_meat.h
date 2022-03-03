// Copyright 2021 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------
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
