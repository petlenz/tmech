// Copyright 2022 Peter Lenz
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
#ifndef VOLUMETRIC_WRAPPER_MEAT_H
#define VOLUMETRIC_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename Tensor>
constexpr volumetric_wrapper<Tensor>::volumetric_wrapper(data_type_tensor const& data):
    _trace(),
    is_init(false),
    _data(data)
{}

/**
 * Copy constructor
 */
template <typename Tensor>
constexpr volumetric_wrapper<Tensor>::volumetric_wrapper(volumetric_wrapper const& data):
    _trace(),
    is_init(false),
    _data(data._data)
{}
//@}

/**
* @name Data
*/
/**
* @brief Returns the element at the specified position of the wrapper.
* @param i Index specifying the row
* @param j Index specifying the column
*/
template <typename Tensor>
constexpr inline auto volumetric_wrapper<Tensor>::operator ()(size_type const i, size_type const j)const{
    return (i == j ? _trace : static_cast<value_type>(0.0));
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template <typename Tensor>
constexpr inline auto volumetric_wrapper<Tensor>::dimension(){
    return data_type_tensor::dimension();
}

/**
 * Returns the rank of the expression.
 */
template <typename Tensor>
constexpr inline auto volumetric_wrapper<Tensor>::rank(){
    return data_type_tensor::rank();
}
//@}

/**
* @name Evaluation of tensor expression
*/
/**
* Evaluates the underlying tensor expression
* and computes the trace.
*/
template <typename Tensor>
constexpr inline auto volumetric_wrapper<Tensor>::evaluate(){
    if(!is_init){

        evaluate::apply(_data);

        _trace = 0;
        for(size_type i{0}; i<dimension(); ++i){
            _trace += _data(i,i);
        }

        _trace /= static_cast<value_type>(dimension());
        is_init = true;
    }
}
//@}

} // NAMESPACE DETAIL

#endif // VOLUMETRIC_WRAPPER_MEAT_H
