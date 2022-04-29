/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ADJOINT_WRAPPER_BONES_H
#define ADJOINT_WRAPPER_BONES_H


namespace detail {

/**
* @class adjoint_wrapper
* @brief Class for the computation of the adjoint of a second-order tensor.
* \f[
* \SecondT{A}^{-1} = \frac{1}{\text{det}(\SecondT{A})}\text{adj}(\SecondT{A})
* \f]
*
* @tparam Tensor Data type of the tensor expression.
*/
template <typename _Tensor>
class adjoint_wrapper : public tensor_base<adjoint_wrapper<_Tensor>>
{
    using data_type_tensor  = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    adjoint_wrapper(data_type_tensor const& data)noexcept;

    adjoint_wrapper(adjoint_wrapper const& data)noexcept;

    constexpr inline auto operator()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    template<typename _Result>
    constexpr inline auto evaluate(_Result & result)noexcept;

    constexpr inline auto raw_data()const noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result & result)noexcept;

    static constexpr inline auto evaluate_details(value_type * result, value_type const* data)noexcept;

    static constexpr inline auto adjoint_details_22(value_type * result,
                                                    value_type const A11, value_type const A12,
                                                    value_type const A21, value_type const A22)noexcept;

    static constexpr inline auto adjoint_details_33(value_type * result,
                                                    value_type const A11, value_type const A12, value_type const A13,
                                                    value_type const A21, value_type const A22, value_type const A23,
                                                    value_type const A31, value_type const A32, value_type const A33)noexcept;
    tensor<value_type, dimension(), rank()> _data;
    _Tensor _data_basis;
};

} // NAMESPACE DETAIL

#endif // ADJOINT_WRAPPER_BONES_H
