/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
    evaluate::apply(_data);
}

} // NAMESPACE DETAIL

#endif // SYMMETRIC_PART_WRAPPER_MEAT_H
