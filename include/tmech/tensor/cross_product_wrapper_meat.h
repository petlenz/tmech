/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef CROSS_PRODUCT_WRAPPER_MEAT_H
#define CROSS_PRODUCT_WRAPPER_MEAT_H


namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template<typename _LHS, typename _RHS>
constexpr cross_product_wrapper<_LHS, _RHS>::cross_product_wrapper(_LHS lhs, _RHS rhs)noexcept:
    _lhs(lhs),
    _rhs(rhs)
{}

/**
 * Copy constructor
 */
template<typename _LHS, typename _RHS>
constexpr cross_product_wrapper<_LHS, _RHS>::cross_product_wrapper(cross_product_wrapper const& data)noexcept:
    _lhs(data._lhs),
    _rhs(data._rhs)
{}
//@}

/**
* @name Data
*/
/**
* Returns the element at the specified position in the expression.
* @param idx Index specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template<typename _LHS, typename _RHS>
constexpr inline auto cross_product_wrapper<_LHS, _RHS>::operator ()(size_type const idx)const noexcept{
    switch (idx) {
    case 0:
        return static_cast<value_type>(_lhs(1) * _rhs(2) - _lhs(2) * _rhs(1));
    case 1:
        return static_cast<value_type>(_lhs(2) * _rhs(0) - _lhs(0) * _rhs(2));
    case 2:
        return static_cast<value_type>(_lhs(0) * _rhs(1) - _lhs(1) * _rhs(0));
    default:
        return static_cast<value_type>(0.0);
    }
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template<typename _LHS, typename _RHS>
constexpr inline auto cross_product_wrapper<_LHS, _RHS>::dimension()noexcept{
    return data_type_LHS::dimension();
}

/**
* Returns the rank.
*/
template<typename _LHS, typename _RHS>
constexpr inline auto cross_product_wrapper<_LHS, _RHS>::rank()noexcept{
    return data_type_LHS::rank();
}
//@}

/**
* @name Evaluation of tensor expression
*/
/**
* Evaluates the underlying left and right hand side tensor expression.
*/
template<typename _LHS, typename _RHS>
constexpr inline auto cross_product_wrapper<_LHS, _RHS>::evaluate()noexcept{
    evaluate::apply(_lhs);
    evaluate::apply(_rhs);
}
//@}

}// NAMESPACE DETAIL

#endif // CROSS_PRODUCT_WRAPPER_MEAT_H
