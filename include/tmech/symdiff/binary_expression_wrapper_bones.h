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
    constexpr inline auto operator()(_Data const& x);

    constexpr inline std::ostream& print(std::ostream & os)const;

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& x)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& x);

    constexpr inline auto reset_imp();

    lhs_expression _lhs;
    rhs_expression _rhs;
};

} // NAMESPACE DETAIL

#endif // BINARY_EXPRESSION_WRAPPER_BONES_H
