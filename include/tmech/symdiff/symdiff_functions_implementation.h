#ifndef SYMDIFF_FUNCTIONS_IMPLEMENTATION_H
#define SYMDIFF_FUNCTIONS_IMPLEMENTATION_H

template<std::size_t N = 1, typename _Expression, typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr>
constexpr inline auto derivative(_Expression, _Variable){
    using _derivative = typename detail::diff_wrapper_n<N, _Variable, _Expression>::deriv_type;
    return detail::derivative_wrapper<_derivative>(_derivative());
}



#endif // SYMDIFF_FUNCTIONS_IMPLEMENTATION_H
