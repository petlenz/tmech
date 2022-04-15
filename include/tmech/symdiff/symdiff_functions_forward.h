/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SYMDIFF_FUNCTIONS_FORWARD_H
#define SYMDIFF_FUNCTIONS_FORWARD_H

template<std::size_t N = 1, typename _Expression, typename _Variable, typename, typename>
constexpr inline auto derivative(_Expression, _Variable);

#endif // SYMDIFF_FUNCTIONS_FORWARD_H
