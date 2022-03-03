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
#ifndef BINARY_EXPRESSION_WRAPPER_BONES_H
#define BINARY_EXPRESSION_WRAPPER_BONES_H

#include "utility.h"

namespace detail {

template <typename _LHS, typename _RHS, typename _Op>
class binary_expression_wrapper
        : public variable_base<binary_expression_wrapper<_LHS, _RHS, _Op>>
{
    friend class variable_base<binary_expression_wrapper<_LHS, _RHS, _Op>>;
public:
    using lhs_expression  = _LHS;
    using rhs_expression  = _RHS;
    using binary_operator = _Op;
    using data_type       = typename detail::result_type<typename _LHS::data_type, typename _RHS::data_type>::data_type;

    binary_expression_wrapper();

    ~binary_expression_wrapper();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline  auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline  auto update(_Data) = delete;

    constexpr inline  auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    lhs_expression _lhs;
    rhs_expression _rhs;
};

} // NAMESPACE DETAIL

#endif // BINARY_EXPRESSION_WRAPPER_BONES_H
