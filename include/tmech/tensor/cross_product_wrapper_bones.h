/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef CROSS_PRODUCT_WRAPPER_BONES_H
#define CROSS_PRODUCT_WRAPPER_BONES_H

namespace detail {

/**
* @class cross_product_wrapper
* @brief Cross product of two frist-order tensor.
*
* @tparam LHS Left hand side tensor expression
* @tparam RHS Right hand side tensor expression
*/
template<typename _LHS, typename _RHS>
class cross_product_wrapper : public tensor_base<cross_product_wrapper<_LHS, _RHS>>
{
    using data_type_RHS  = typename std::remove_const<typename std::remove_reference<_RHS>::type>::type;
    using data_type_LHS  = typename std::remove_const<typename std::remove_reference<_LHS>::type>::type;
    using value_type_LHS = typename data_type_LHS::value_type;
    using value_type_RHS = typename data_type_RHS::value_type;

public:
    using value_type = typename result_type<value_type_LHS, value_type_RHS>::value_type;
    using size_type  = std::size_t;

    constexpr cross_product_wrapper(_LHS lhs, _RHS rhs)noexcept;

    constexpr cross_product_wrapper(cross_product_wrapper const& data)noexcept;

    constexpr inline auto operator()(size_type const idx)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    _LHS _lhs;
    _RHS _rhs;
};

} // NAMESPACE DETAIL
#endif // CROSS_PRODUCT_WRAPPER_BONES_H
