/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SKEW_SYMMETRIC_PART_WRAPPER_MEAT_H
#define SKEW_SYMMETRIC_PART_WRAPPER_MEAT_H


namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename Tensor>
skew_symmetric_part_wrapper<Tensor>::skew_symmetric_part_wrapper(data_type_tensor const& __data)noexcept:
    _data(__data)
{}

/**
 * Copy constructor
 */
template <typename Tensor>
skew_symmetric_part_wrapper<Tensor>::skew_symmetric_part_wrapper(skew_symmetric_part_wrapper const& __data)noexcept:
    basetype(__data),
    _data(__data._data)
{}
//@}

template <typename Tensor>
constexpr inline auto skew_symmetric_part_wrapper<Tensor>::operator ()(size_type const i, size_type const j)const noexcept{
    //check if data.raw_data() is enable for better performace
    if constexpr(std::experimental::is_detected<detail::has_raw_data, typename std::remove_reference<Tensor>::type>::value){
        constexpr auto Dim{dimension()};
        return safe_cast<value_type>(0.5) *
               (_data.raw_data()[i * Dim + j] - _data.raw_data()[j * Dim + i]);
    }else{
      return safe_cast<value_type>(0.5) * (_data(i, j) - _data(j, i));
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
constexpr inline auto skew_symmetric_part_wrapper<Tensor>::dimension()noexcept{
    return data_type_tensor::dimension();
}

/**
 * Returns the rank of the expression.
 */
template <typename Tensor>
constexpr inline auto skew_symmetric_part_wrapper<Tensor>::rank()noexcept{
    return data_type_tensor::rank();
}
//@}

template <typename Tensor>
constexpr inline auto skew_symmetric_part_wrapper<Tensor>::evaluate()noexcept{
    evaluate::apply(_data);
}

} // NAMESPACE DETAIL
#endif // SKEW_SYMMETRIC_PART_WRAPPER_MEAT_H
