#ifndef STD_FUNCTIONS_IMPLEMENTATION_H
#define STD_FUNCTIONS_IMPLEMENTATION_H

namespace std {

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto exp(_Variable ){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::exp_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto log(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::log_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto log2(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::log2_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto log10(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::log10_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto sqrt(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::sqrt_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto abs(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::abs_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto cos(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::cos_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto sin(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::sin_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto acos(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::acos_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto asin(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::asin_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto tan(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::tan_wrapper>();
}

template<typename _Variable,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value>::type * = nullptr>
constexpr inline auto sign(_Variable){
    return symdiff::detail::function_wrapper<_Variable, symdiff::detail::sign_wrapper>();
}

template<typename _Variable,
         typename T, long long int L, long long int R, long long int E,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value> * = nullptr>
constexpr inline auto pow(_Variable, symdiff::real<T, L, R, E>){
    return symdiff::detail::diff_pow<_Variable, symdiff::real<T, L, R, E>, true>();
}

template<typename _Variable,
         typename T, std::size_t ID,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<T>::value> * = nullptr>
constexpr inline auto pow(_Variable , symdiff::constant<T, ID>){
    return symdiff::detail::diff_pow<_Variable, symdiff::constant<T, ID>, true>();
}

template<typename _Variable,
         typename _VariableExp,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Variable>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableExp>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _Variable::data_type>::value> * = nullptr,
         typename std::enable_if<std::is_fundamental<typename _VariableExp::data_type>::value> * = nullptr>
constexpr inline auto pow(_Variable, _VariableExp){
    return symdiff::detail::diff_pow<_Variable, _VariableExp, false>();
}

}
#endif // STD_FUNCTIONS_IMPLEMENTATION_H
