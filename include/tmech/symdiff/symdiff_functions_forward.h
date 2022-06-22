/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SYMDIFF_FUNCTIONS_FORWARD_H
#define SYMDIFF_FUNCTIONS_FORWARD_H

template<std::size_t N = 1, typename _Expression, typename _Variable, typename, typename>
constexpr inline auto derivative(_Expression, _Variable);

template<std::size_t N = 1, typename _Derivative, typename _Variable, typename, typename>
constexpr inline auto derivative(detail::derivative_wrapper<_Derivative>, _Variable);


namespace experimental {
template<typename ...Functions>
constexpr inline auto make_vector(Functions const & ...);

template<typename ...Functions, typename ...Variables>
constexpr inline auto make_jacobi_matrix(detail::vector_of_functions<Functions...> const & , Variables const& ...);

template<typename ...Functions, typename ...Variables>
constexpr inline auto newton_solver(detail::vector_of_functions<Functions...> const &, Variables const& ...);

template<typename ...Functions, typename ...Variables>
constexpr inline auto newton_solver(detail::jacobi_matrix_wrapper_mixed_type<detail::vector_of_functions<Functions...>, Variables...> const& , detail::vector_of_functions<Functions...> const &);

template<typename Derived, typename T, std::size_t ID_Var>
constexpr inline auto newton_solver(variable_base<Derived> const& __function, variable<T, ID_Var> const& __variable);

template<typename Derived, typename T1, std::size_t ID_Var1, typename T2, std::size_t ID_Var2>
constexpr inline auto make_implicit(variable_base<Derived> const& , variable<T1, ID_Var1> const& , variable<T2, ID_Var2> const&);
}

#endif // SYMDIFF_FUNCTIONS_FORWARD_H
