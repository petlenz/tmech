// Copyright 2022 Peter Lenz
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
