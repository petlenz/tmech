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
#ifndef TRAITS_H
#define TRAITS_H



//template <typename T>
//struct tensor_info
//{
//    using value_type = T;
//    static constexpr std::size_t dimension{0};
//    static constexpr std::size_t rank{0};
//};


//template <typename T, std::size_t Dim, std::size_t Rank>
//struct tensor_info<tensor<T, Dim, Rank>>
//{
//    using value_type = T;
//    static constexpr std::size_t dimension{Dim};
//    static constexpr std::size_t rank{Rank};
//};

//for complex support
namespace detail {


//template <typename T>
//struct name
//{
//    static constexpr inline auto evaluate(){
//        if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_LHS>::value){
//            if constexpr (std::is_reference_v<LHS>){
//                const_cast<data_type_LHS&>(lhs_).evaluate();
//            }else{
//                lhs_.evaluate();
//            }
//        }
//        if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_RHS>::value){
//            if constexpr (std::is_reference_v<RHS>){
//                const_cast<data_type_RHS&>(rhs_).evaluate();
//            }else{
//                rhs_.evaluate();
//            }
//        }
//    }
//};




template<typename T>
struct is_complex_t : public std::false_type {};

template<typename T>
struct is_complex_t<std::complex<T>> : public std::true_type {};
}

template<typename T>
static constexpr bool is_complex() { return detail::is_complex_t<T>::value; }


namespace detail {

template <typename T>
struct num_diff_get_function
{
    using type = T;
};

template <typename T>
struct num_diff_get_function<std::reference_wrapper<T>>
{
    using type = T;
};

template <typename Function, typename Direction, bool = false>
struct num_diff_get_data_type_result
{
    using return_type = decltype (Function()(Direction()));
    using type = tensor<typename Direction::value_type, Direction::dimension(), return_type::rank() + Direction::rank()>;
};


template <typename Function, typename Direction>
struct num_diff_get_data_type_result<Function, Direction, true>
{
    using type = tensor<typename Direction::value_type, Direction::dimension(), Direction::rank()>;
};

template <typename T>
struct result_expression_type
{
    using result_type = T;
};

template <typename T, std::size_t Dim, std::size_t Rank>
struct result_expression_type<tmech::tensor<T, Dim, Rank>>
{
    using result_type = tmech::tensor<T, Dim, Rank> const &;
};


template<typename _T>
struct element_type
{
    using value_type = _T;
};


template<typename _T>
struct element_type<std::complex<_T>>
{
    using value_type = _T;
};

template<typename LHS, typename RHS>
struct result_type
{
    using value_type = typename result_type<typename LHS::value_type, typename RHS::value_type>::value_type;
    using type = typename result_type<LHS, RHS>::value_type;
    using pointer = typename result_type<typename LHS::value_type, typename RHS::value_type>::pointer;
    using const_pointer = typename result_type<typename LHS::value_type, typename RHS::value_type>::const_pointer;
};


template<typename _T>
struct result_type<_T, _T>
{
    using value_type = _T;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

//template<>
//struct result_type<float, float>
//{
//    using value_type = float;
//    using type = float;
//    using pointer = value_type*;
//    using const_pointer = const pointer;
//    using expr_ref = value_type&;
//};

template<>
struct result_type<float, int>
{
    using value_type = float;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<int, float>
{
    using value_type = float;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

//template<>
//struct result_type<double, double>
//{
//    using value_type = double;
//    using type = double;
//    using pointer = value_type*;
//    using const_pointer = const pointer;
//    using expr_ref = value_type&;
//};

template<>
struct result_type<double, int>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<int, double>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<double, long int>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<long int, double>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<double, float>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<float, double>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<double, std::size_t>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<>
struct result_type<std::size_t, double>
{
    using value_type = double;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<typename T>
struct result_type<std::complex<T>, std::complex<T>>
{
    using value_type = std::complex<T>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};


template<>
struct result_type<std::complex<float>, std::complex<double>>
{
    using value_type = std::complex<double>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};


template<>
struct result_type<std::complex<double>, std::complex<float>>
{
    using value_type = std::complex<double>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};


template<typename T>
struct result_type<float, std::complex<T>>
{
    using value_type = std::complex<typename result_type<T, float>::value_type>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<typename T>
struct result_type<std::complex<T>, float>
{
    using value_type = std::complex<typename result_type<T, float>::value_type>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<typename T>
struct result_type<double, std::complex<T>>
{
    using value_type = std::complex<typename result_type<T, double>::value_type>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

template<typename T>
struct result_type<std::complex<T>, double>
{
    using value_type = std::complex<typename result_type<T, double>::value_type>;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using expr_ref = value_type&;
};

} // NAMESPACE DETAIL

#endif // TRAITS_H
