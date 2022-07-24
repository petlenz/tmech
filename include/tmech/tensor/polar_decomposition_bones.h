/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_BONES_H
#define POLAR_DECOMPOSITION_BONES_H

namespace detail {

/**
 * @brief Computes the right and left polar decomposition \f$\SecondT{F} = \SecondT{R}\SecondT{U} = \SecondT{V}\SecondT{R}\f$
 * of a real second order tensor.
 *
 * @tparam _Tensor Tensor expression from which the polar decomposition should be determined.
*/
template <typename _Tensor>
class polar_decomposition_wrapper
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;
    using this_type  = polar_decomposition_wrapper<_Tensor>;
    using tensor2 = tensor<typename data_type_tensor::value_type, data_type_tensor::dimension(), 2>;
    using tensor4 = tensor<typename data_type_tensor::value_type, data_type_tensor::dimension(), 4>;


    friend class polar_decomposition_dR_wrapper<polar_decomposition_wrapper<_Tensor>, tensor4>;
    friend class polar_decomposition_dU_wrapper<polar_decomposition_wrapper<_Tensor>, tensor4>;
    friend class polar_decomposition_dV_wrapper<polar_decomposition_wrapper<_Tensor>, tensor4>;

    friend class polar_decomposition_R_wrapper<polar_decomposition_wrapper<_Tensor>, tensor2>;
    friend class polar_decomposition_U_wrapper<polar_decomposition_wrapper<_Tensor>, tensor2>;
    friend class polar_decomposition_V_wrapper<polar_decomposition_wrapper<_Tensor>, tensor2>;

    static_assert(data_type_tensor::dimension() == 2 || data_type_tensor::dimension() == 3, "Polar decomposition only for Dim == 2, 3");
    static_assert(data_type_tensor::rank() == 2,                                  "Polar decomposition only for Rank == 2");

    constexpr polar_decomposition_wrapper(data_type_tensor const& __data, bool const __newton_method, value_type const __tol, size_type const __max_steps)noexcept;

    constexpr polar_decomposition_wrapper(polar_decomposition_wrapper const& __data)noexcept;

    constexpr inline auto R()const noexcept;

    constexpr inline auto U()const noexcept;

    constexpr inline auto V()const noexcept;

private:
    constexpr inline auto evaluate_newton()noexcept;

    constexpr inline auto set_up_R_newton()noexcept;

    constexpr inline auto evaluate_eigen()noexcept;

    constexpr inline auto evaluate_derivatives()noexcept;

    constexpr inline auto evaluate_derivatives_2D()noexcept;

    constexpr inline auto evaluate_derivatives_3D()noexcept;

    constexpr inline auto evaluate()noexcept;

    static constexpr auto Rank = data_type_tensor::rank();
    static constexpr auto Dim  = data_type_tensor::dimension();
    bool _is_init;
    bool _is_init_deriv;
    bool newton_method;
    value_type newton_tol;
    size_type max_steps;
    tensor2 _U;
    tensor2 _V;
    tensor2 _R;
    tensor4 _dR;
    tensor4 _dU;
    tensor4 _dV;
    _Tensor _data;
};

} // NAMESPACE DETAIL

#endif // POLAR_DECOMPOSITION_BONES_H
