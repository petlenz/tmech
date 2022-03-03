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
#ifndef TENSOR_OPERATORS_H
#define TENSOR_OPERATORS_H

#include <utility>
#include <type_traits>

/**
 * @brief Addition of two tensors of same rank and dimension
 * \f[
 * C_{ijkl...} = A_{ijkl...} + B_{ijkl...}
 * \f]
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a,b,c;
 * a.randn(); b.randn();
 * c = a + b;
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Right hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator+(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};
    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator +: no matching rank");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator +: no matching dimensions");
    if constexpr (isLvalueLHS && isLvalueLHS){
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, tmech::detail::operator_add>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueLHS){
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS, TensorTypeRHS const&, tmech::detail::operator_add>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueLHS){
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS const&, TensorTypeRHS, tmech::detail::operator_add>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS, TensorTypeRHS, tmech::detail::operator_add>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}

/**
 * @brief Subtraction of two tensors of same rank and dimension.
 * \f[
 * C_{ijkl...} = A_{ijkl...} - B_{ijkl...}
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a,b,c;
 * a.randn(); b.randn();
 * c = a - b;
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Right hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator-(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};
    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator -: no matching rank");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator -: no matching dimensions");
    if constexpr (isLvalueLHS && isLvalueLHS){
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, tmech::detail::operator_sub>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueLHS){
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS, TensorTypeRHS const&, tmech::detail::operator_sub>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueLHS){
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS const&, TensorTypeRHS, tmech::detail::operator_sub>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorTypeLHS, TensorTypeRHS, tmech::detail::operator_sub>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}


/**
 * @brief Single contraction of two tensors.
 * Assume that the left hand side tensor is of rank m and the right
 * hand side tensor of rank n. The resulting tensor is of rank m+n-2.
 * The most right index of the left hand side tensor is contracted
 * with the most left index of the right hand tensor
 * \f[
 * C_{i_{1},...,i_{m+n-2}} = A_{i_{1},...,i_{m-1},k} - B_{k,i_{2},...,i_{n}}
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a,b,c;
 * a.randn(); b.randn();
 * c = a * b;
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Right hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator*(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    using _SequenceLHS = tmech::sequence<TensorTypeLHS::rank()>;
    using _SequenceRHS = tmech::sequence<1>;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator -: no matching dimensions");

    if constexpr (isLvalueLHS && isLvalueLHS){
        return tmech::detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueLHS){
        return tmech::detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueLHS){
        return tmech::detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return tmech::detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}


/**
 * @brief Scalar update of a tensor
 * \f[
 * C_{ijkl...} = a*C_{ijkl...}
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a,b;
 * a.randn();
 * b = 2.0*a;
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @tparam _Scalar Scalar type. Must be a std::fundamental
 * @param __scalar Scalar
 * @param __tensor Tensor expression.
 */
template<typename _Scalar, typename _Tensor,
         typename std::enable_if<std::is_fundamental<typename std::decay<_Scalar>::type>::value>::type* = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto operator*(_Scalar && __scalar, _Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    using scalar = tmech::detail::scalar<typename TensorType::value_type>;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return tmech::detail::tensor_binary_expression_wrapper<const TensorType&, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(std::forward<_Scalar>(__scalar)));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorType, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(std::forward<_Scalar>(__scalar)));
    }
}

/**
 * @brief Scalar update of a tensor
 * \f[
 * C_{ijkl...} = C_{ijkl...}*a
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a,b;
 * a.randn();
 * b = a*2.0;
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @tparam _Scalar Scalar type. Must be a std::fundamental
 * @param __scalar Scalar
 * @param __tensor Tensor expression.
 */
template<typename _Scalar, typename _Tensor,
         typename std::enable_if<std::is_fundamental<typename std::decay<_Scalar>::type>::value>::type* = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto operator*(_Tensor && __tensor, _Scalar && __scalar){
    using TensorType = typename std::decay<_Tensor>::type;
    using scalar = tmech::detail::scalar<typename TensorType::value_type>;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return tmech::detail::tensor_binary_expression_wrapper<const TensorType&, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(std::forward<_Scalar>(__scalar)));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorType, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(std::forward<_Scalar>(__scalar)));
    }
}

/**
 * @brief Tensor division by a scalar
 * \f[
 * C_{ijkl...} = C_{ijkl...}/a
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a,b;
 * a.randn();
 * b = a/2.0;
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @tparam _Scalar Scalar type. Must be a std::fundamental
 * @param __scalar Scalar
 * @param __tensor Tensor expression.
 */
template<typename _Scalar, typename _Tensor,
         typename std::enable_if<std::is_fundamental<typename std::decay<_Scalar>::type>::value>::type* = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto operator/(_Tensor && __tensor, _Scalar && __scalar){
    using TensorType = typename std::decay<_Tensor>::type;
    using scalar = tmech::detail::scalar<typename TensorType::value_type>;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return tmech::detail::tensor_binary_expression_wrapper<const TensorType&, scalar, tmech::detail::operator_div>(std::forward<_Tensor>(__tensor), scalar(std::forward<_Scalar>(__scalar)));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorType, scalar, tmech::detail::operator_div>(std::forward<_Tensor>(__tensor), scalar(std::forward<_Scalar>(__scalar)));
    }
}



/**
 * @brief Complex scalar update of a tensor
 * \f[
 * C_{ijkl...} = a*C_{ijkl...}
 * \f]
 *
 * @tparam _Tensor Tensor object
 * @tparam _Scalar Scalar type. Must be a std::complex
 * @param __scalar Scalar
 * @param __tensor Tensor expression.
 */
template<typename _ScalarComplex, typename _Tensor,
         typename std::enable_if<tmech::detail::is_complex_t<typename std::decay<_Tensor>::type::value_type>::value>::type* = nullptr,
         typename std::enable_if<tmech::detail::is_complex_t<_ScalarComplex>::value>::type* = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto operator*(_ScalarComplex const& __scalar_complex, _Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    using scalar = tmech::detail::scalar<_ScalarComplex>;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return tmech::detail::tensor_binary_expression_wrapper<const TensorType&, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(__scalar_complex));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorType, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(__scalar_complex));
    }
}

/**
 * @brief Complex scalar update of a tensor
 * \f[
 * C_{ijkl...} = C_{ijkl...}*a
 * \f]
 *
 * @tparam _Tensor Tensor object
 * @tparam _Scalar Scalar type. Must be a std::complex
 * @param __scalar Scalar
 * @param __tensor Tensor expression.
 */
template<typename _ScalarComplex, typename _Tensor,
         typename std::enable_if<tmech::detail::is_complex_t<typename std::decay<_Tensor>::type::value_type>::value>::type* = nullptr,
         typename std::enable_if<tmech::detail::is_complex_t<_ScalarComplex>::value>::type* = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto operator*(_Tensor && __tensor, _ScalarComplex __scalar_complex){
    using TensorType = typename std::decay<_Tensor>::type;
    using scalar = tmech::detail::scalar<_ScalarComplex>;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return tmech::detail::tensor_binary_expression_wrapper<const TensorType&, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(__scalar_complex));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorType, scalar, tmech::detail::operator_mul>(std::forward<_Tensor>(__tensor), scalar(__scalar_complex));
    }
}

/**
 * @brief Tensor division by a complex scalar
 * \f[
 * C_{ijkl...} = C_{ijkl...}/a
 * \f]
 *
 * @tparam _Tensor Tensor object
 * @tparam _Scalar Scalar type. Must be a std::fundamental
 * @param __scalar Scalar
 * @param __tensor Tensor expression.
 */
template<typename _ScalarComplex, typename _Tensor,
         typename std::enable_if<tmech::detail::is_complex_t<typename std::decay<_Tensor>::type::value_type>::value>::type* = nullptr,
         typename std::enable_if<tmech::detail::is_complex_t<_ScalarComplex>::value>::type* = nullptr,
         typename std::enable_if_t<tmech::is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto operator/(_Tensor && __tensor, _ScalarComplex __scalar_complex){
    using TensorType = typename std::decay<_Tensor>::type;
    using scalar = tmech::detail::scalar<_ScalarComplex>;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return tmech::detail::tensor_binary_expression_wrapper<const TensorType&, scalar, tmech::detail::operator_div>(std::forward<_Tensor>(__tensor), scalar(__scalar_complex));
    }else{
        return tmech::detail::tensor_binary_expression_wrapper<TensorType, scalar, tmech::detail::operator_div>(std::forward<_Tensor>(__tensor), scalar(__scalar_complex));
    }
}
#endif // TENSOR_OPERATORS_H
