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
constexpr negative_tensor_wrapper<Tensor>::negative_tensor_wrapper(data_type_tensor const& data):
    _data(data)
{}

/**
 * @brief Copy constructor
 */
template <typename Tensor>
constexpr negative_tensor_wrapper<Tensor>::negative_tensor_wrapper(negative_tensor_wrapper const& data):
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
constexpr inline auto negative_tensor_wrapper<Tensor>::operator ()(Indices ... indices)const{
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
constexpr inline auto negative_tensor_wrapper<Tensor>::dimension(){
    return data_type_tensor::dimension();
}

/**
* @brief Returns the rank.
*/
template <typename Tensor>
constexpr inline auto negative_tensor_wrapper<Tensor>::rank(){
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
constexpr inline auto negative_tensor_wrapper<Tensor>::evaluate(){
    if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_tensor>::value){
        if constexpr (std::is_reference_v<Tensor>){
            const_cast<data_type_tensor&>(_data).evaluate();
        }else{
            _data.evaluate();
        }
    }
}
//@}

} // NAMESPACE DETAIL

#endif // NEGATIVE_TENSOR_WRAPPER_MEAT_H
