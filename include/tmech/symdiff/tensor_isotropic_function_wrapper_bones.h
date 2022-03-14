// Copyright 2022 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------
#ifndef TENSOR_ISOTROPIC_FUNCTION_WRAPPER_BONES_H
#define TENSOR_ISOTROPIC_FUNCTION_WRAPPER_BONES_H

namespace detail {

template <typename _Func>
struct print_isotropic_functions;

template <>
struct print_isotropic_functions<tmech::detail::sqrt_>
{
    static inline std::ostream&  print(std::ostream & __os){
        __os<<"sqrt";
        return __os;
    }
};

template <>
struct print_isotropic_functions<tmech::detail::exp>
{
    static inline std::ostream&  print(std::ostream & __os){
        __os<<"exp";
        return __os;
    }
};

template <>
struct print_isotropic_functions<tmech::detail::negative_part>
{
    static inline std::ostream&  print(std::ostream & __os){
        __os<<"negative";
        return __os;
    }
};

template <>
struct print_isotropic_functions<tmech::detail::positive_part>
{
    static inline std::ostream&  print(std::ostream & __os){
        __os<<"positive";
        return __os;
    }
};

template <>
struct print_isotropic_functions<tmech::detail::log>
{
    static inline std::ostream&  print(std::ostream & __os){
        __os<<"log";
        return __os;
    }
};


template <typename _Expr, typename _Func>
class tensor_isotropic_function_wrapper : public variable_base<tensor_isotropic_function_wrapper<_Expr, _Func>>
{
    friend class variable_base<tensor_isotropic_function_wrapper<_Expr, _Func>>;
    friend class tensor_isotropic_function_wrapper_derivative<tensor_isotropic_function_wrapper<_Expr, _Func>>;
public:
    using data_type = typename _Expr::data_type;

    tensor_isotropic_function_wrapper();

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    static tmech::detail::isotropic_tensor_function<data_type const&, _Func> _iso_data;
    static data_type _data;
    _Expr _expr;
};

template <typename _Expr, typename _Func>
tmech::detail::isotropic_tensor_function<typename tensor_isotropic_function_wrapper<_Expr, _Func>::data_type const&, _Func>
tensor_isotropic_function_wrapper<_Expr, _Func>::_iso_data(tensor_isotropic_function_wrapper<_Expr, _Func>::_data);

template <typename _Expr, typename _Func>
typename tensor_isotropic_function_wrapper<_Expr, _Func>::data_type tensor_isotropic_function_wrapper<_Expr, _Func>::_data;

}

#endif // TENSOR_ISOTROPIC_FUNCTION_WRAPPER_BONES_H
