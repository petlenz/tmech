/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
        evaluate::apply(_data);
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
