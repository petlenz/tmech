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



//This class provides two different methods.
// * In the first method the orthogonal second order tensor \f$\SecondT{R}\f$ is obtained
// * with the <a href="https://en.wikipedia.org/wiki/Polar_decomposition">iterative algorithm</a>.
// * The second method follows the standard approch
// * \f[
// * \SecondT{F}^T\SecondT{F} = (\SecondT{R}\SecondT{U})^T(\SecondT{R}\SecondT{U})
// * =\SecondT{U}^T\SecondT{R}^T\SecondT{R}\SecondT{U}
// * =\SecondT{U}^T\SecondT{U}
// * =\SecondT{U}\SecondT{U}
// * \f]
// * this gives the final result
// * \f[
// * \SecondT{U} = (\SecondT{F}^T\SecondT{F})^{1/2}.
// * \f]
// *
// * Moreover, this class provides also the derivatives \f$\frac{\text{d}\SecondT{R}}{\text{d}\SecondT{F}}\f$,
// *  \f$\frac{\text{d}\SecondT{U}}{\text{d}\SecondT{F}}\f$ and \f$\frac{\text{d}\SecondT{V}}{\text{d}\SecondT{F}}\f$.
// * The derivatives are given  <a href="https://doi.org/10.1023/A:1007663620943">here</a>.
// * Note, that the differentiation rule given in <a href="https://onlinelibrary.wiley.com/doi/abs/10.1002/zamm.200410242">O. Kintzel</a> is used.
// * For a two dimensional problem the derivatives are given as
// * \f[
// * \frac{\text{d}\SecondT{R}}{\text{d}\SecondT{F}} &=\frac{1}{I_u} ((\SecondT{I}\otimes\SecondT{I}) - (\SecondT{R}\boxtimes\SecondT{R}))\\
// * \frac{\text{d}\SecondT{U}}{\text{d}\SecondT{F}} &=\frac{1}{I_u} (\SecondT{R}^T\otimes\SecondT{I}) - ((\SecondT{R}^T\otimes \SecondT{U}^T)  - (\SecondT{I}\boxtimes (\SecondT{U}\SecondT{R}^T)^T))\\
// * \frac{\text{d}\SecondT{V}}{\text{d}\SecondT{F}} &=\frac{1}{I_v} (\SecondT{I}\otimes \SecondT{R}^T) - (\SecondT{V}\otimes \SecondT{R}^T)  - (\SecondT{V}\SecondT{R}\boxtimes \SecondT{I}))
// * \f]
// * where \f$I_u=\text{trace}(\SecondT{U})\f$ and \f$I_v=\text{trace}(\SecondT{V})\f$. For a three dimensional problem
// * \f[
// *  \frac{\text{d}\SecondT{R}}{\text{d}\SecondT{F}} &= (\SecondT{R}^T\otimes \SecondT{I}) - \frac{1}{\text{det}(\tilde{\SecondT{U}})}((\tilde{\SecondT{U}} \SecondT{R}^T\otimes(\SecondT{U}*\tilde{\SecondT{U}})^T) - (\tilde{\SecondT{U}}\boxtimes(\SecondT{U}\tilde{\SecondT{U}}\SecondT{R}^T)^T))\\
// *  \frac{\text{d}\SecondT{U}}{\text{d}\SecondT{F}} &= (I\otimes \SecondT{R}^T) - \frac{1}{\text{det}(\tilde{\SecondT{V}})}*((\SecondT{V}\tilde{\SecondT{V}}\otimes (\tilde{\SecondT{V}}\SecondT{R})^T) - (\SecondT{V}\tilde{\SecondT{V}}\SecondT{R}\boxtimes \tilde{\SecondT{V}}^T))\\
// *  \frac{\text{d}\SecondT{V}}{\text{d}\SecondT{F}} &= \frac{1}{\text{det}(\tilde{\SecondT{U}})}*((\SecondT{R}\tilde{\SecondT{U}}\SecondT{R}^T\otimes\tilde{\SecondT{U}}) - (\SecondT{R}\tilde{\SecondT{U}}\boxtimes (\tilde{\SecondT{U}}\SecondT{R}^T)^T))
// * \f]
// * where \f$\tilde{\SecondT{U}} = \text{trace}(\SecondT{U})\SecondT{I} - \SecondT{U}\f$, \f$ \tilde{\SecondT{V}} = \text{trace}(\SecondT{V})\SecondT{I} - \SecondT{V}\f$.
// *


#endif // POLAR_DECOMPOSITION_BONES_H
