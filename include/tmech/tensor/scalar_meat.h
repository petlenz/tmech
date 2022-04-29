/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SCALAR_MEAT_H
#define SCALAR_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template<typename _T>
constexpr scalar<_T>::scalar(_T const scalar)noexcept:
    scalar_(scalar)
{}

/**
 * Copy constructor
 */
template<typename _T>
constexpr scalar<_T>::scalar(scalar const& scalar)noexcept:
    scalar_(scalar.scalar_)
{}
//@}


template<typename _T>
constexpr inline _T scalar<_T>::operator()()const noexcept{
    return scalar_;
}

template<typename _T>
template<typename ...Numbers>
constexpr inline _T scalar<_T>::operator()(Numbers...)const noexcept{
    return scalar_;
}

} // NAMESPACE DETAIL
#endif // SCALAR_MEAT_H
