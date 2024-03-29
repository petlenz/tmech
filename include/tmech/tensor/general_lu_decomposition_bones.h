/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GENERAL_LU_DECOMPOSITION_BONES_H
#define GENERAL_LU_DECOMPOSITION_BONES_H

namespace detail {


class general_lu_solver
{
public:

    general_lu_solver()noexcept{}

    template<typename Jacobian, typename Residuum, typename Vector_x>
    static constexpr inline auto apply(Jacobian & A, Residuum const& R, Vector_x & x)noexcept;

private:
    template<std::size_t I, typename Jacobian>
    static constexpr inline auto elimination_I(Jacobian & A)noexcept;

    template<std::size_t I, std::size_t J, typename Jacobian, typename Val_inv>
    static constexpr inline auto elimination_J(Jacobian & A, Val_inv const& invAii)noexcept;

    template<std::size_t I, std::size_t J, typename Jacobian>
    static constexpr inline auto elimination_J(Jacobian & A)noexcept;

    template<std::size_t I, std::size_t J, std::size_t K, typename Jacobian>
    static constexpr inline auto elimination_K(Jacobian & A)noexcept;

    template<std::size_t I, typename Jacobian, typename Residuum, typename Vector_x>
    static constexpr inline auto forward_I(Jacobian & A, Residuum const& R, Vector_x & x)noexcept;

    template<std::size_t I, std::size_t K, typename Jacobian, typename Vector_x>
    static constexpr inline auto forward_K(Jacobian & A, Vector_x & x)noexcept;

    template<std::size_t I, typename Jacobian, typename Vector_x>
    static constexpr inline auto backward_I(Jacobian & A, Vector_x & x)noexcept;

    template<std::size_t I, std::size_t K, typename Jacobian, typename Vector_x>
    static constexpr inline auto backward_K(Jacobian & A, Vector_x & x)noexcept;

    template<typename T>
    static constexpr inline auto invert(T const& A)noexcept;

    template<typename T>
    static constexpr inline auto invert(T && A)noexcept;

    template<typename LHS, typename RHS>
    static constexpr inline auto inner_product(LHS const& A, RHS const& B)noexcept;

    template<typename T>
    static constexpr inline auto eval_data(T const& data)noexcept;

    template<typename T>
    static constexpr inline auto eval_data(T && data)noexcept;
};

} // NAMESPACE DETAIL
#endif // GENERAL_LU_DECOMPOSITION_BONES_H
