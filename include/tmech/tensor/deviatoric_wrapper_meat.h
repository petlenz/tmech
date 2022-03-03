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
#ifndef DEVIATORIC_WRAPPER_MEAT_H
#define DEVIATORIC_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename _Tensor>
constexpr deviatoric_wrapper<_Tensor>::deviatoric_wrapper(data_type_tensor const& data):
    _trace(0),
    _data(data)
{}

/**
 * @brief Copy constructor
 */
template <typename _Tensor>
constexpr deviatoric_wrapper<_Tensor>::deviatoric_wrapper(deviatoric_wrapper const& data):
    _trace(0),
    _data(data._data)
{}
//@}

template <typename _Tensor>
constexpr inline auto deviatoric_wrapper<_Tensor>::operator()(size_type const i, size_type const j)const{
    return (i == j ? _data(i,j)-_trace : _data(i,j));
}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
*/
template <typename _Tensor>
constexpr inline auto deviatoric_wrapper<_Tensor>::dimension(){
    return data_type_tensor::dimension();
}

/**
* @brief Returns the rank.
*/
template <typename _Tensor>
constexpr inline auto deviatoric_wrapper<_Tensor>::rank(){
    return data_type_tensor::rank();
}
//@}

/**
* @name Evaluation of tensor expression
*/
/**
* @brief Evaluates the underlying tensor expression and determines the trace.
*/
template <typename _Tensor>
constexpr inline auto deviatoric_wrapper<_Tensor>::evaluate(){
    if(!this->_is_init){
        if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_tensor>::value){
            if constexpr (std::is_reference_v<_Tensor>){
                const_cast<data_type_tensor&>(_data).evaluate();
            }else{
                _data.evaluate();
            }
        }
        _trace = 0;
        for(size_type i{0}; i<dimension(); ++i){
            _trace += _data(i,i);
        }

        _trace /= static_cast<value_type>(dimension());
        this->_is_init = true;
    }
}
//@}
} // NAMESPACE DETAIL
#endif // DEVIATORIC_WRAPPER_MEAT_H
