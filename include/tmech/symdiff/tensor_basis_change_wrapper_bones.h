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
#ifndef TENSOR_BASIS_CHANGE_WRAPPER_BONES_H
#define TENSOR_BASIS_CHANGE_WRAPPER_BONES_H

namespace detail {

template <typename _Expr, typename _Sequence>
class tensor_basis_change_wrapper :
        public variable_base<tensor_basis_change_wrapper<_Expr, _Sequence>>
{
    friend class variable_base<tensor_basis_change_wrapper<_Expr, _Sequence>>;
public:
    using data_type       = typename _Expr::data_type;
    using tensor_info_lhs = get_tensor_info<typename _Expr::data_type>;

    static_assert (detail::is_tensor<typename _Expr::data_type>::value,
    "tensor_basis_change_wrapper: expression data_type is not a tensor");

    constexpr tensor_basis_change_wrapper();

    template<typename Data>
    constexpr inline auto operator()(Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __data)const;

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    constexpr inline auto print_trans(std::ostream & __os)const;

    constexpr inline auto print_general(std::ostream & __os)const;

    template<std::size_t _First, std::size_t ..._Numbers>
    static constexpr auto print_sequence(std::ostream & __os, tmech::sequence<_First, _Numbers...>);

    template<std::size_t _Last>
    static constexpr auto print_sequence(std::ostream & __os, tmech::sequence<_Last>);

    _Expr _expr;
};

}

#endif // TENSOR_BASIS_CHANGE_WRAPPER_BONES_H
