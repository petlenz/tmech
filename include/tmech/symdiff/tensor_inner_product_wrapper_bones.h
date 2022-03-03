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
#ifndef TENSOR_INNER_PRODUCT_WRAPPER_BONES_H
#define TENSOR_INNER_PRODUCT_WRAPPER_BONES_H

namespace detail {

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct return_data_type_inner_product
{
    using type = typename get_tensor<_T, _Dim, _Rank>::type;
};

template <typename _T, std::size_t _Dim>
struct return_data_type_inner_product<_T, _Dim, 0>
{
    using type = double;
};

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class tensor_inner_product_wrapper
        : public variable_base<tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>
{
    friend class variable_base<tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>;

public:
    using data_type_lhs = typename _ExprLHS::data_type;
    using data_type_rhs = typename _ExprRHS::data_type;

    using tensor_info_lhs = get_tensor_info<typename _ExprLHS::data_type>;
    using tensor_info_rhs = get_tensor_info<typename _ExprRHS::data_type>;
    using value_type = typename tmech::detail::result_type<typename tensor_info_lhs::value_type, typename tensor_info_lhs::value_type>::value_type;
    using data_type = typename return_data_type_inner_product<value_type, tensor_info_lhs::dimension(), tensor_info_lhs::rank() + tensor_info_rhs::rank() - _SeqLHS::size() - _SeqRHS::size()>::type;

    static_assert (detail::is_tensor<typename _ExprLHS::data_type>::value,
    "tensor_inner_product_wrapper: lhs expression data type is not a tensor");

    static_assert (detail::is_tensor<typename _ExprRHS::data_type>::value,
    "tensor_inner_product_wrapper: rhs expression data type is not a tensor");

    tensor_inner_product_wrapper();

    template<typename Data>
    constexpr inline auto const& operator()(Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& /*__data*/)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    constexpr inline auto print_general(std::ostream & __os)const;

    template<std::size_t _First, std::size_t ..._Numbers>
    static constexpr auto print_sequence(std::ostream & __os, tmech::sequence<_First, _Numbers...>);

    template<std::size_t _Last>
    static constexpr auto print_sequence(std::ostream & __os, tmech::sequence<_Last>);

    static data_type _data;
    _ExprLHS _lhs;
    _ExprRHS _rhs;
};


template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
typename tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::data_type
tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::_data;
}

#endif // TENSOR_INNER_PRODUCT_WRAPPER_BONES_H
