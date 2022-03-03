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
#ifndef SYMDIFF_UTILITY_H
#define SYMDIFF_UTILITY_H

template <typename _T>
struct is_symdiff_type
{
    using type = std::false_type;
    static constexpr bool value = false;
};

template <typename _Derived>
struct is_symdiff_type<variable_base<_Derived>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _T, std::size_t _VarID>
struct is_symdiff_type<variable<_T, _VarID>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _T, std::size_t _ID>
struct is_symdiff_type<constant<_T, _ID>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _T, long long int _L, long long int _R, long long int _Exp>
struct is_symdiff_type<real<_T, _L, _R, _Exp>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _T>
struct is_symdiff_type<scalar_one<_T>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _T>
struct is_symdiff_type<scalar_zero<_T>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _T>
struct is_symdiff_type<tensor_one<_T>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _T>
struct is_symdiff_type<tensor_zero<_T>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _Expr>
struct is_symdiff_type<detail::negative<_Expr>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _LHS, typename _RHS, typename _Op>
struct is_symdiff_type<detail::binary_expression_wrapper<_LHS, _RHS, _Op>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _LHS, typename _RHS, typename _SeqLHS, typename _SeqRHS>
struct is_symdiff_type<detail::tensor_outer_product_wrapper<_LHS, _RHS, _SeqLHS, _SeqRHS>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _LHS, typename _RHS, typename _SeqLHS, typename _SeqRHS>
struct is_symdiff_type<detail::tensor_inner_product_wrapper<_LHS, _RHS, _SeqLHS, _SeqRHS>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Expr, typename _Sequence>
struct is_symdiff_type<detail::tensor_basis_change_wrapper<_Expr, _Sequence>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Expr, typename _Func>
struct is_symdiff_type<detail::function_wrapper<_Expr, _Func>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Exp, bool _IsFundamental>
struct is_symdiff_type<detail::diff_pow<_Base, _Exp, _IsFundamental>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _T, typename ..._Sequences>
struct is_symdiff_type<detail::inv_tensor<_T, _Sequences...>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Expr, typename _Func>
struct is_symdiff_type<detail::tensor_function_wrapper<_Expr, _Func>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};


namespace detail {

template<typename _Type, typename... _Arguments>
using has_update = decltype(std::declval<_Type>().update(std::declval<_Arguments>()...));

template<typename _Type, typename... _Arguments>
using has_reset = decltype(std::declval<_Type>().reset(std::declval<_Arguments>()...));



template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct get_tensor
{
    using type = tmech::tensor<_T, _Dim, _Rank>;
};

template<typename Tensor>
struct get_tensor_info{

    using value_type = typename Tensor::value_type;

    static constexpr inline auto rank(){
        return Tensor::rank();
    }

    static constexpr inline auto dimension(){
        return Tensor::dimension();
    }
};

template <typename _T, bool _IsTensor>
struct get_value_type_detail;

template <typename _T>
struct get_value_type_detail<_T, false>
{
    using value_type = _T;
};

template <typename _T>
struct get_value_type_detail<_T, true>
{
    using value_type = typename get_tensor_info<_T>::value_type;
};

template <typename _T>
struct get_value_type
{
    using value_type = typename get_value_type_detail<_T, is_tensor<_T>::value>::value_type;
};

template <typename Tensor>
struct get_tensor_type
{
    using t_info = get_tensor_info<Tensor>;
    using type = typename get_tensor<typename t_info::value_type, t_info::dimension(), t_info::rank()>::type;
};

template <typename _T>
struct get_identity_tensor;

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct get_identity_tensor<tmech::tensor<_T, _Dim, _Rank>>
{
    using data_type = tmech::tensor<_T, _Dim, 2>;
    using type = tmech::eye<_T, _Dim, 2>;
};


template <typename _T>
struct get_zero_tensor;

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct get_zero_tensor<tmech::tensor<_T, _Dim, _Rank>>
{
    using type = tmech::zeros<_T, _Dim, _Rank>;
};


template <typename _T>
struct is_scalar
{
    static constexpr bool value = std::is_fundamental_v<_T>;
};

template <typename _T>
struct is_tensor
{
    static constexpr bool value = false;
};

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct is_tensor<tmech::tensor<_T, _Dim, _Rank>>
{
    static constexpr bool value = true;
};



template <typename _Tensor>
struct get_tensor_one
{
    using t_info = get_tensor_info<_Tensor>;
    using type = tmech::eye<typename t_info::value_type, t_info::dimension(), t_info::rank()>;
};




template <typename _DataType, bool _isTensor>
class fundamental_one_derivative_detail;

template <typename _DataType, std::size_t _Rank>
class fundamental_tensor_one_derivative_detail;

//dX/dX = dX_{i}/dX_{j} = I
template <typename _DataType>
class fundamental_tensor_one_derivative_detail<_DataType, 1ul>
{
public:
    using type = tensor_one<_DataType>;
};

//dX/dX = dX_{ij}/dX_{kl} = outer<I, I, <1,2>, <3,4>>
template <typename _DataType>
class fundamental_tensor_one_derivative_detail<_DataType, 2ul>
{
    using I = tensor_one<_DataType>;
public:
    using type = tensor_outer_product_wrapper<I, I, tmech::sequence<1,3>, tmech::sequence<2,4>>;
};

//dX/dX = dX_{ijkl}/dX_{mnop} = 1,5,2,6, 3,7,4,8
template <typename _DataType>
class fundamental_tensor_one_derivative_detail<_DataType, 4ul>
{
    using I = tensor_one<_DataType>;
    using II = tensor_outer_product_wrapper<I, I, tmech::sequence<1,2>, tmech::sequence<3,4>>;
public:
    using type = tensor_outer_product_wrapper<II, II, tmech::sequence<1,5,2,6>, tmech::sequence< 3,7,4,8>>;
};


//dx/dx = 1
template <typename _DataType>
class fundamental_one_derivative_detail<_DataType, false>
{
public:
    using type = scalar_one<_DataType>;
};

template <typename _DataType>
class fundamental_one_derivative_detail<_DataType, true>
{
public:
    using type = typename fundamental_tensor_one_derivative_detail<_DataType, get_tensor_info<_DataType>::rank()>::type;
};

template <typename _DataType>
class fundamental_one_derivative
{
public:
    using type = typename fundamental_one_derivative_detail<_DataType, is_tensor<_DataType>::value>::type;
};


//dx/dx = 1
template <typename _Type, bool _isTensor>
struct get_fundamental_one_detail;

template <typename _Type>
struct get_fundamental_one_detail<_Type, false>
{
    //or _RHS; it dosent matter both are the same type
    using type = scalar_one<_Type>;
};

//dX/dX = I<X::dimension(), 2*X::rank()>
template <typename _Type>
struct get_fundamental_one_detail<_Type, true>
{
    using type = tensor_one<_Type>;
    //typename fundamental_tensor_one_derivative<_Type, get_tensor_info<_Type>::rank()>::type;
};

template <typename _Type>
struct get_fundamental_one
{
    using type = typename get_fundamental_one_detail<_Type, is_tensor<_Type>::value>::type;
};




template <typename _LHS, bool _is_LHS_Tensor, typename _RHS, bool _is_RHS_Tensor>
struct get_fundamental_zero_detail;

template <typename _LHS, typename _RHS>
struct get_fundamental_zero_detail<_LHS, false, _RHS, false>
{
    //or _RHS; it dosent matter both are the same type
    using type = scalar_zero<_LHS>;
};

template <typename _LHS, typename _RHS>
struct get_fundamental_zero_detail<_LHS, true, _RHS, false>
{
    using type = tensor_zero<_LHS>;
};

template <typename _LHS, typename _RHS>
struct get_fundamental_zero_detail<_LHS, false, _RHS, true>
{
    using type = tensor_zero<_RHS>;
};

template <typename _LHS, typename _RHS>
struct get_fundamental_zero_detail<_LHS, true, _RHS, true>
{
    using type = tensor_zero<tmech::tensor<typename _LHS::value_type, _LHS::dimension(), _LHS::rank()+_RHS::rank()>>;
};

template <typename _LHS, typename _RHS>
struct get_fundamental_zero
{
    using type = typename get_fundamental_zero_detail<_LHS, is_tensor<_LHS>::value, _RHS, is_tensor<_RHS>::value>::type;
};


template <typename _T, bool _IsTensor>
struct get_zero_detail
{
    using type = scalar_zero<_T>;
};

template <typename _T>
struct get_zero_detail<_T, true>
{
    using type = tensor_zero<_T>;
};

template <typename _T>
struct get_zero
{
    using type = typename get_zero_detail<_T, is_tensor<_T>::value>::type;
};


template <typename _T, bool _IsTensor>
struct get_one_detail
{
    using type = scalar_one<_T>;
};

template <typename _T>
struct get_one_detail<_T, true>
{
    using type = tensor_one<_T>;
};

template <typename _T>
struct get_one
{
    using type = typename get_one_detail<_T, is_tensor<_T>::value>::type;
};


template <typename T>
struct print_operator;

template <>
struct print_operator<tmech::detail::operator_add>{
    static inline std::string print(){
        return "+";
    }
};

template <>
struct print_operator<tmech::detail::operator_sub>{
    static inline std::string print(){
        return "-";
    }
};

template <>
struct print_operator<tmech::detail::operator_mul>{
    static inline std::string print(){
        return "*";
    }
};

template <>
struct print_operator<tmech::detail::operator_div>{
    static inline std::string print(){
        return "/";
    }
};

} // NAMESPACE DETAIL

#endif // SYMDIFF_UTILITY_H
