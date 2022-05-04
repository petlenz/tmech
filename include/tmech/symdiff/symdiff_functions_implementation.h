/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SYMDIFF_FUNCTIONS_IMPLEMENTATION_H
#define SYMDIFF_FUNCTIONS_IMPLEMENTATION_H

template<std::size_t N = 1, typename _Expression, typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr>
constexpr inline auto derivative(_Expression, _Variable){
    using _derivative = typename detail::diff_wrapper_n<N, _Variable, _Expression>::deriv_type;
    return detail::derivative_wrapper<_derivative>(_derivative());
}

namespace experimental {

template<typename ...Functions>
constexpr inline auto make_vector(Functions const & ...){
    return detail::vector_of_functions<Functions...>();
}

template<typename ...Functions, typename ...Variables>
constexpr inline auto make_jacobi_matrix(detail::vector_of_functions<Functions...> const & , Variables const& ...){
    return detail::jacobi_matrix_wrapper_mixed_type<detail::vector_of_functions<Functions...>, Variables...>();
}

template<typename ...Functions, typename ...Variables>
constexpr inline auto newton_solver(detail::jacobi_matrix_wrapper_mixed_type<detail::vector_of_functions<Functions...>, Variables...> const& , detail::vector_of_functions<Functions...> const &){
    return detail::newton_raphson_solver<detail::jacobi_matrix_wrapper_mixed_type<detail::vector_of_functions<Functions...>, Variables...>, detail::vector_of_functions<Functions...>>();
}

template<typename ...Functions, typename ...Variables>
constexpr inline auto newton_solver(detail::vector_of_functions<Functions...> const &, Variables const& ...){
    return detail::newton_raphson_solver<detail::jacobi_matrix_wrapper_mixed_type<detail::vector_of_functions<Functions...>, Variables...>, detail::vector_of_functions<Functions...>>();
}

template<typename Derived, typename T, std::size_t ID_Var>
constexpr inline auto newton_solver(variable_base<Derived> const& __function, variable<T, ID_Var> const& __variable){
    return detail::newton_raphson_solver<Derived, variable<T, ID_Var>>(__function.convert(), __variable);
}

template<typename Derived, typename T1, std::size_t ID_Var1, typename T2, std::size_t ID_Var2>
constexpr inline auto make_implicit(variable_base<Derived> const& , variable<T1, ID_Var1> const&, variable<T2, ID_Var2> const&){
    return detail::implicit_function<std::tuple<Derived>, std::tuple<variable<T1, ID_Var1>, variable<T2, ID_Var2>>>();
}

template<typename ..._Functions, typename ..._Variables>
constexpr inline auto make_implicit(std::tuple<_Functions...> const& , std::tuple<_Variables...> const&){
    return detail::implicit_function<std::tuple<_Functions...>, std::tuple<_Variables...>>();
}

}

#endif // SYMDIFF_FUNCTIONS_IMPLEMENTATION_H
