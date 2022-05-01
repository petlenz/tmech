/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef DERIVATIVE_WRAPPER_MEAT_H
#define DERIVATIVE_WRAPPER_MEAT_H

namespace detail {

template <typename Expression>
derivative_wrapper<Expression>::derivative_wrapper(Expression const& expr)noexcept:
    _data(expr)
{}

template <typename Expression>
template<typename _Data>
constexpr inline decltype(auto) derivative_wrapper<Expression>::operator()(_Data const& __data)noexcept{
    return _data(__data);
}

template <typename Expression>
constexpr inline std::ostream& derivative_wrapper<Expression>::print(std::ostream & os)const noexcept{
    _data.print(os);
    return os;
}

}
#endif // DERIVATIVE_WRAPPER_MEAT_H
