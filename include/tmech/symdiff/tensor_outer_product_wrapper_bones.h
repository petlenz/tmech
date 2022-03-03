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
#ifndef TENSOR_OUTER_PRODUCT_WRAPPER_BONES_H
#define TENSOR_OUTER_PRODUCT_WRAPPER_BONES_H

namespace detail {

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class tensor_outer_product_wrapper :
        public variable_base<tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>
{
    friend class variable_base<tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>;
public:
    using tensor_info_lhs = get_tensor_info<typename _ExprLHS::data_type>;
    using tensor_info_rhs = get_tensor_info<typename _ExprRHS::data_type>;
    using value_type = typename tmech::detail::result_type<typename tensor_info_lhs::value_type, typename tensor_info_lhs::value_type>::value_type;
    using data_type = typename get_tensor<value_type, tensor_info_lhs::dimension(), tensor_info_lhs::rank() + tensor_info_rhs::rank()>::type;

    static_assert (detail::is_tensor<typename _ExprLHS::data_type>::value,
    "tensor_outer_product_wrapper: lhs expression data type is not a tensor");

    static_assert (detail::is_tensor<typename _ExprRHS::data_type>::value,
    "tensor_outer_product_wrapper: rhs expression data type is not a tensor");

    tensor_outer_product_wrapper();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& __x);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    constexpr inline auto print_general(std::ostream & __os)const;

    constexpr inline auto print_otimes(std::ostream & __os)const;

    constexpr inline auto print_otimesu(std::ostream & __os)const;

    constexpr inline auto print_otimesl(std::ostream & __os)const;

    template<std::size_t First, std::size_t ...Numbers>
    static constexpr auto print_sequence(std::ostream & __os, tmech::sequence<First, Numbers...>);

    template<std::size_t Last>
    static constexpr auto print_sequence(std::ostream & __os, tmech::sequence<Last>);

    _ExprLHS _lhs;
    _ExprRHS _rhs;
};

}
#endif // TENSOR_OUTER_PRODUCT_WRAPPER_BONES_H
