/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
