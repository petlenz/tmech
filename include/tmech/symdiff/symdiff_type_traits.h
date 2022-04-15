/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SYMDIFF_TYPE_TRAITS_H
#define SYMDIFF_TYPE_TRAITS_H

namespace detail {

template <typename _LHS, typename _RHS, bool _IsTensorLHS, bool _IsTensorRHS>
struct result_type_tensor_expression{
    using data_type = typename tmech::detail::result_type<_LHS, _RHS>::value_type;
};

template <typename _LHS, typename _RHS>
struct result_type_tensor_expression<_LHS, _RHS, true, false>
{
    using data_type = _LHS;
};

template <typename _LHS, typename _RHS>
struct result_type_tensor_expression<_LHS, _RHS, false, true>
{
    using data_type = _RHS;
};

template <typename _LHS, typename _RHS>
struct result_type{
    using data_type = typename result_type_tensor_expression<_LHS, _RHS, is_tensor<_LHS>::value, is_tensor<_RHS>::value>::data_type;
};

}

#endif // SYMDIFF_TYPE_TRAITS_H
