/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POW_TENSOR_WRAPPER_BONES_H
#define POW_TENSOR_WRAPPER_BONES_H

namespace detail {

/**
* @class inverse_wrapper
* @brief Wrapper for the determination of the
* inverse of a tensor expression.
*
* @tparam Tensor Tensor expression from which the
* inverse should be determined.
*/
template <typename _Tensor>
class pow_tensor_wrapper : public tensor_base<pow_tensor_wrapper<_Tensor>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;

    template <typename _Dbase, typename _Dtensor>
    friend class pow_tensor_derivative_wrapper;

    using this_type = pow_tensor_wrapper<_Tensor>;
    using basetype = tensor_base<pow_tensor_wrapper<_Tensor>>;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type = std::size_t;

    pow_tensor_wrapper(data_type_tensor const& __data_expr, size_type const __exponent)noexcept;

    pow_tensor_wrapper(pow_tensor_wrapper const& __data)noexcept;

    template<typename ...Indices>
    constexpr inline auto operator ()(Indices const... indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    template<typename Result>
    constexpr inline auto evaluate(Result & result)noexcept;

    constexpr inline auto evaluate()noexcept;

    constexpr inline auto raw_data()const noexcept;

    constexpr inline auto derivative()noexcept;

private:
    constexpr inline auto evaluate_derivatives()noexcept;

    template<typename Result>
    constexpr inline auto evaluate_imp(Result & result)noexcept;

    //    constexpr inline auto sym_derivative();

    //    constexpr inline auto skew_derivative();

    constexpr inline auto normal_derivative()noexcept;

    tensor<value_type, data_type_tensor::dimension(), 2> _data;
    _Tensor _data_expr;
    tensor<value_type, data_type_tensor::dimension(), 4> _derivative;
    std::vector<tensor<value_type, data_type_tensor::dimension(), data_type_tensor::rank()>> _data_i;
    size_type const _expo;
};


} // NAMESPACE DETAIL

#endif // POW_TENSOR_WRAPPER_BONES_H
