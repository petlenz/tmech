#ifndef TENSOR_OPERATORS_FORWARD_H
#define TENSOR_OPERATORS_FORWARD_H

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
template<typename _TensorLHS, typename _TensorRHS, typename , typename >
constexpr inline auto operator+(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

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
template<typename _TensorLHS, typename _TensorRHS, typename , typename>
constexpr inline auto operator-(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);


/**
 * @brief Single contraction of two tensors.
 * Assume that the left hand side tensor is of rank m and the right
 * hand side tensor of rank n. The resulting tensor is of rank m+n-2.
 * The most right index of the left hand side tensor is contracted
 * with the most left index of the right hand tensor
 * \f[
 * C_{i_{1},...,i_{m+n-2}} = A_{i_{1},...,i_{m-1},k} * B_{k,i_{2},...,i_{n}}
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
template<typename _TensorLHS, typename _TensorRHS, typename , typename>
constexpr inline auto operator*(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);


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
template<typename _Scalar, typename _Tensor, typename , typename>
constexpr inline auto operator*(_Scalar && __scalar, _Tensor && __tensor);

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
template<typename _Scalar, typename _Tensor, typename , typename>
constexpr inline auto operator*(_Tensor && __tensor, _Scalar && __scalar);

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
template<typename _Scalar, typename _Tensor, typename , typename>
constexpr inline auto operator/(_Tensor && __tensor, _Scalar && __scalar);



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
template<typename _ScalarComplex, typename _Tensor, typename , typename ,typename>
constexpr inline auto operator*(_ScalarComplex const& __scalar_complex, _Tensor && __tensor);

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
template<typename _ScalarComplex, typename _Tensor, typename , typename ,typename>
constexpr inline auto operator*(_Tensor && __tensor, _ScalarComplex __scalar_complex);

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
template<typename _ScalarComplex, typename _Tensor, typename , typename ,typename>
constexpr inline auto operator/(_Tensor && __tensor, _ScalarComplex __scalar_complex);
#endif // TENSOR_OPERATORS_FORWARD_H
