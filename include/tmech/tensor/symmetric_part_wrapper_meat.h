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
#ifndef SYMMETRIC_PART_WRAPPER_MEAT_H
#define SYMMETRIC_PART_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename Tensor>
symmetric_part_wrapper<Tensor>::symmetric_part_wrapper(data_type_tensor const& data):
    _data(data)
{}

/**
 * Copy constructor
 */
template <typename Tensor>
symmetric_part_wrapper<Tensor>::symmetric_part_wrapper(symmetric_part_wrapper const& data):
    _data(data._data)
{}
//@}

template <typename Tensor>
constexpr inline auto symmetric_part_wrapper<Tensor>::operator ()(size_type const i, size_type const j)const{
    //check if data.raw_data() is enable for better performace??
    if constexpr(std::experimental::is_detected<detail::has_raw_data, data_type_tensor>::value){
        constexpr auto Dim{dimension()};
        return static_cast<value_type>(0.5)*(_data.raw_data()[i*Dim+j] + _data.raw_data()[j*Dim+i]);
    }else{
        return static_cast<value_type>(0.5)*(_data(i,j) + _data(j,i));
    }
}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension of the expression.
*/
template <typename Tensor>
constexpr inline auto symmetric_part_wrapper<Tensor>::dimension(){
    return data_type_tensor::dimension();
}

/**
 * Returns the rank of the expression.
 */
template <typename Tensor>
constexpr inline auto symmetric_part_wrapper<Tensor>::rank(){
    return data_type_tensor::rank();
}
//@}

template <typename Tensor>
constexpr inline auto symmetric_part_wrapper<Tensor>::evaluate(){
    if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_tensor>::value){
        if constexpr (std::is_reference_v<Tensor>){
            const_cast<data_type_tensor&>(_data).evaluate();
        }else{
            _data.evaluate();
        }
    }
}

} // NAMESPACE DETAIL

#endif // SYMMETRIC_PART_WRAPPER_MEAT_H
