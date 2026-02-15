/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef BINARY_EXPRESSION_WRAPPER_BONES_H
#define BINARY_EXPRESSION_WRAPPER_BONES_H

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
    constexpr inline  auto value(_Data const&) = delete;

    template<typename _Data>
    constexpr inline  auto update(_Data const&) = delete;

    constexpr inline  auto reset() = delete;

    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

  private:
    lhs_expression _lhs;
    rhs_expression _rhs;
};

} // NAMESPACE DETAIL

#endif // BINARY_EXPRESSION_WRAPPER_BONES_H
