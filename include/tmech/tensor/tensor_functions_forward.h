/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_FUNCTIONS_FORWARD_H
#define TENSOR_FUNCTIONS_FORWARD_H

/**
 * @brief Compares two tensor expressions if they are almost equal.
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> a, b;
 * a = b = tmech::rand<double, 3, 2>();
 * if(tmech::almost_equal(a,b,5e-7)){
 * std::cout<<"True\n";
 * }else{
 * std::cout<<"False\n";
 * }
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename _T, typename, typename, typename>
constexpr inline auto almost_equal(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs, _T const __eps);

/**
 * @brief The \f$\overline{\otimes}\f$ product between two second-order tensors is
 * defined as
 * \f[
 * \FourthT{C} =  \SecondT{A} \overline{\otimes} \SecondT{B} =  A_{ij} B_{kl} \SecondT{e}_i \otimes \SecondT{e}_k \otimes \SecondT{e}_j \otimes \SecondT{e}_l
 * \f]
 * and represents a transposition of the two inner bases.
 *
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * tmech::tensor<double,3,4> C{tmech::otimesu(A,B)};
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr auto otimesu(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief The \f$\underline{\otimes}\f$ product between two second-order tensors is
 * defined as
 * \f[
 * \FourthT{C} =  \SecondT{A} \underline{\otimes} \SecondT{B} =  A_{ij} B_{kl} \SecondT{e}_i \otimes \SecondT{e}_l \otimes \SecondT{e}_j \otimes \SecondT{e}_k.
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * tmech::tensor<double,3,4> C{tmech::otimesl(A,B)};
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr auto otimesl(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief The dyadic product between a tensor object of order n and a tensor object of order m
 * is a tensor of order m + n.
 * The dyadic product between two first-order tensors is defined as
 * \f[
 * A_{i_{1},...,i_{n},i_{n+1},...,i_{m}} = a_{i_{i},...,i_{n}} b_{i_{n+1},...,i_{m}}
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * tmech::tensor<double,3,4> C{tmech::otimes(A,B)};
 * tmech::tensor<double,3,6> D{tmech::otimes(C,B)};
 * tmech::tensor<double,3,8> E{tmech::otimes(C,C)};
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr auto otimes(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief A double contraction between two tensors objects contracts the two most right and left indices.
 * The result of a double contraction between a tensor of order n and a tensor of order m is a
 * tensor of order m + n - 4.
 *
 * \code{.cpp}
 * tmech::tensor<double,3,4> A;
 * tmech::tensor<double,3,2> B;
 * A.randn(); B.randn();
 * auto c{tmech::dcontract(A,B)};
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr auto inline dcontract(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief A fourth contraction between two tensors objects contracts the fourth most right and left indices.
 * The result of a double contraction between a tensor of order n and a tensor of order m is a
 * tensor of order m + n - 8.
 *
 * \code{.cpp}
 * tmech::tensor<double,3,4> A;
 * tmech::tensor<double,3,2> B;
 * A.randn(); B.randn();
 * auto c{tmech::ddcontract(A,B)};
 * \endcode
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr auto inline ddcontract(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief General outer product. Controlled by template parameters
 *        %_SequenceLHS and %_SequenceRHS. Bases contained in sequence
 *        %_SequenceLHS are used for ordered element acces in %__lhs
 *        tensor expression. Bases contained in sequence
 *        %_SequenceRHS are used for ordered element acces in %__rhs
 *        tensor expression. Both togther peform outer product.
 *
 * \code{.cpp}
 * using SeqL = tmech::sequence<1,2>;
 * using SeqR = tmech::sequence<3,4>;
 * //Bases 1,2 of the new tensor C
 * //are given by a and bases 3,4 are
 * //given by b.
 * tmech::tensor<double, 3, 2> a, b;
 * tmech::tensor<double, 3, 4> C;
 * C = tmech::outer_product<SeqL, SeqR>(a,b);
 * \endcode
 *
 * @tparam _SequenceLHS Left hand side sequence, which contains numbers of bases to peform the outer product.
 * @tparam _SequenceRHS Right hand side sequence, which contains numbers of bases to peform the outer product.
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _SequenceLHS, typename _SequenceRHS, typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr inline auto outer_product(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief General inner product. Controlled by template parameters
 *        %_SequenceLHS and %_SequenceRHS. Bases contained in sequence
 *        %_SequenceLHS are used for ordered element acces in %_DerivedLHS
 *        tensor expression. Bases contained in sequence
 *        %_SequenceRHS are used for ordered element acces in %_DerivedRHS
 *        tensor expression.
 *
 * \code{.cpp}
 * using SeqL = tmech::sequence<3,4>;
 * using SeqR = tmech::sequence<1,2>;
 * //Double contraction of two 4th order tensors
 * tmech::tensor<double, 3, 4> A, B, C;
 * C = tmech::inner_product<SeqL, SeqR>(A,B);
 *
 * //Double contraction of a 4th and a 2th order tensor
 * tmech::tensor<double, 3, 2> a, c;
 * c = tmech::inner_product<SeqL, SeqR>(C,a);
 * \endcode
 *
 * @tparam _SequenceLHS Left hand side sequence, which contains the numbers of bases used for contraction.
 * @tparam _SequenceRHS Right hand side sequence, which contains the numbers of bases used for contraction.
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 *
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _SequenceLHS, typename _SequenceRHS, typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr inline auto inner_product(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief Cross product of two first-order tensors.
 *
 * @tparam _TensorLHS Left hand side tensor object
 * @tparam _TensorRHS Right hand side tensor object
 * @param __tensor_lhs Left hand side tensor expression.
 * @param __tensor_rhs Reft hand side tensor expression.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr inline auto cross(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);

/**
 * @brief The dot product between two first-order tensors is
 * defined as
 * \f[
 * \mathbf{a} \cdot \mathbf{b} = \text{dot}\left(\mathbf{a},\mathbf{b}\right) = a_ib_i
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double,3,1> a{1,2,3}, b{4,5,6};
 * double dot{tmech::dot(a,b)};
 * \endcode
 *
 * @param data_base_lhs Left hand side tensor object.
 * @param data_base_rhs Right hand side tensor object.
 */
template<typename _TensorLHS, typename _TensorRHS, typename, typename>
constexpr inline auto dot(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs);



/**
 * @brief Sign of a scalar value. Only for fundamental
 * types such as double, float, ... .
 * @tparam _T  Type of value.
 * @param __value Scalar for sign determination.
 */
template<typename _T, typename>
constexpr inline _T sign(_T const value);

/**
 * @brief Converts a tensor into Voigt's notation
 *
 */
template<typename _TupleSequence = void, typename _Tensor, typename _T, typename, typename>
constexpr inline auto convert_tensor_to_voigt(_Tensor && __tensor, _T * __ptr);

/**
 * @brief General basis rearrangement. Controlled by template parameter
 *        %_Sequence, which contains the new order of bases.
 *
 * \code{.cpp}
 * //Basis 1,2,3,4 is swaped to 3,4,1,2.
 * tmech::tensor<double, 3, 4> A, B;
 * A.randn();
 * B = tmech::basis_change<tmech::sequence<3,4,1,2>>(A);
 * B = tmech::basis_change<tmech::sequence<3,4,1,2>>(A+2*A);
 * \endcode
 * Self assignment
 * \code{.cpp}
 * //Basis 1,2,3,4 is swaped to 3,4,1,2.
 * tmech::tensor<double, 3, 4> A;
 * A.randn();
 * A = tmech::eval(tmech::basis_change<tmech::sequence<3,4,1,2>>(A));
 * A = tmech::eval(tmech::basis_change<tmech::sequence<3,4,1,2>>(A+2*A));
 * \endcode
 *
 * @tparam _Sequence A tmech::sequence<>, which contains the new order of bases
 * @tparam _Derived Tensor object
 * @param __tensor Tensor expression from which the basis rearrangement is to be formed
 */
template<typename _Sequence, typename _Tensor, typename , typename>
constexpr inline auto basis_change(_Tensor && __tensor);

/**
 * @brief Trace of a second-order tensor.
 * \f$\text{tr}\left(\SecondT{A}\right) = A_{ii} \f$
 *
 * @tparam _Derived Tensor object
 * @param __tensor Tensor expression from which the trace is to be formed.
 */
template<typename _Tensor, typename>
constexpr inline auto trace(_Tensor && __tensor);

/**
 * @brief Absolute value of each entry.
 * \code{.cpp}
 * tmech::tensor<double,3,2> A;
 * A.fill(-1);
 * A = tmech::abs(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression.
 */
template<typename _Tensor, typename >
constexpr inline auto abs(_Tensor && __tensor);

/**
 * @brief Temporary evaluation. Evaluates a tensor expression into a temporary object.
 * \code{.cpp}
 * tmech::tensor<double,3,2> A;
 * A.randn();
 * A = tmech::eval(tmech::trans(A));
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression to be temporary evaluated.
 */
template<typename _Tensor, typename >
constexpr inline auto eval(_Tensor && __tensor);

/**
 * @brief Wrapper function for transposition for a second-order tensor
 * and major transposition of a fourth-order tensor.
 *
 * The transpose of a second-order tensor in index notation \f$(A_{ij})^T = A_{ji}\f$.\\
 * The transpose of a fourth-order tensor in index notation \f$(A_{ijkl})^T = A_{klij}\f$.
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * A.randn();
 * B = tmech::trans(A);
 * \endcode
 * \code{.cpp}
 * tmech::tensor<double,3,2> A;
 * A.randn();
 * A = tmech::eval(tmech::trans(A));
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the transposition is to be formed.
 */
template<typename _Tensor, typename >
constexpr inline auto trans(_Tensor && __tensor);

/**
 * @brief Transposition of the left base a fourth-order tensor \f$(A_{ijkl})^{l} = A_{jikl}\f$.
 * \code{.cpp}
 * tmech::tensor<double,3,4> A, B;
 * A.randn();
 * B = tmech::transl(A);
 * \endcode
 * \code{.cpp}
 * tmech::tensor<double,3,4> A;
 * A.randn();
 * A = tmech::eval(tmech::transl(A));
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the left transpose part is to be formed.
 */
template<typename _Tensor, typename >
constexpr auto transl(_Tensor && __tensor);

/**
 * @brief Skew-symmetric part of a second-order tensor
 * \f$\text{skew}\left(\SecondT{A}\right) = \frac{1}{2}\left(\SecondT{A} - \SecondT{A}^T \right) \f$
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A.randn();
 * B = tmech::skew(A);
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the skew-symmetric part is to be formed.
 */
template<typename _Tensor, typename >
constexpr auto skew(_Tensor && __tensor);

/**
 * @brief Symmetric part of a second-order tensor
 * \f$\text{sym}\left(\SecondT{A}\right) = \frac{1}{2}\left(\SecondT{A} + \SecondT{A}^T \right) \f$
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * A.randn();
 * B = tmech::sym(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the symmetric part is to be formed.
 */
template<typename _Tensor, typename >
constexpr auto sym(_Tensor && __tensor);

/**
 * @brief Volumetric part of a second-order tensor
 * \f$\text{vol}(\SecondT{A}) = \frac{1}{d}\text{trace}(\SecondT{A})\SecondT{I} \f$, where
 * \f$d\f$ is the dimension.
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * A.randn();
 * B = tmech::vol(A);
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the volumetric part is to be formed.
 */
template<typename _Tensor, typename >
constexpr inline auto vol(_Tensor && __tensor);

/**
 * @brief Deviatoric part of a second-order tensor
 * \f$\SecondT{A} = \text{vol}(\SecondT{A}) + \text{dev}(\SecondT{A}) \f$
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * A.randn();
 * B = tmech::dev(A);
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the deviatoric part is to be formed.
*/
template<typename _Tensor, typename >
constexpr inline auto dev(_Tensor && __tensor);

/**
 * @brief Cofactors of a second-order tensor
 * \f$\text{dev}\left(\mathbf{A}\right) = \left(\mathbf{A} - \text{vol}\left(\mathbf{A}\right)\right) \f$
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * A.randn();
 * B = tmech::cof(A);
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the cofactors are to be formed.
*/
template<typename _Tensor, typename >
constexpr inline auto cof(_Tensor && __tensor);

/**
 * @brief Adjoint of a second-order tensor
 * \f$\text{dev}\left(\mathbf{A}\right) = \left(\mathbf{A} - \text{vol}\left(\mathbf{A}\right)\right) \f$
 * \code{.cpp}
 * tmech::tensor<double,3,2> A, B;
 * A.randn();
 * B = tmech::adj(A);
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the adjoint is to be formed.
*/
template<typename _Tensor, typename >
constexpr inline auto adj(_Tensor && __tensor);

/**
 * @brief Inverse of a second- and a fourth-order tensor.
 * The inverse of a second order tensor is defined by \f$ \SecondT{A}^{-1}\SecondT{A} = \SecondT{I}\f$,
 * where \f$\SecondT{I}\f$ is the second order identity tensor.
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A.randn();
 * B = tmech::inv(A);
 * \endcode
 *
 * The inverse of a fourth order tensor depends on the minior-symmetry.
 * Using the following differentiation rule \f$\FourthT{C}_{ijkl} = \frac{\partial \SecondT{a}_{ij} }{\partial \SecondT{b}_{kl}}\f$
 * yields a minior-symmetry in the first and in the second pair of indicies \f$ i,j\f$ and \f$k,l\f$, respectively.
 * In this case the inverse is defined by
 * \f$ (\FourthT{A}^{-1})_{ijmn}\FourthT{A}_{mnkl} =\frac{1}{2}\left(\SecondT{I}_{ik}\SecondT{I}_{jl} + \SecondT{I}_{il}\SecondT{I}_{jk} \right) \f$.
 * \code{.cpp}
 * tmech::tensor<double, 3, 4> A, B;
 * A.randn();
 * //the sequences are indicating the pairs of minior-symmetry
 * B = tmech::inv<tmech::sequence<1,2>, tmech::sequence<3,4>>(A);
 * // the rule above is the most used one, therefore the following is sufficient.
 * // the correct indicies are set inside the function.
 * B = tmech::inv(A);
 * \endcode
 *
 * Using the following differentiation rule \f$\FourthT{C}_{iklj} = \frac{\partial \SecondT{a}_{ij} }{\partial \SecondT{b}_{kl}}\f$,
 * yields a minior symmetry in the outer and in the inner pair of indicies \f$i,j\f$ and \f$k,l\f$, respectively.
 * \code{.cpp}
 * tmech::tensor<double, 3, 4> A, B;
 * A.randn();
 * //the sequences are indicating the pairs of minior-symmetry
 * B = tmech::inv<tmech::sequence<1,4>, tmech::sequence<2,3>>(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the inverse is to be formed.
*/
template<typename ..._Sequences, typename _Tensor, typename >
constexpr inline auto inv(_Tensor && __tensor);

/**
 * @brief Inverse of a second- and a full anisotropic fourth-order tensor.
 * The inverse of a second order tensor is defined by \f$ \SecondT{A}^{-1}\SecondT{A} = \SecondT{I}\f$,
 * where \f$\SecondT{I}\f$ is the second order identity tensor. In the case of a second oder tensor
 * the ouput is the same as from the function %inv.
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A.randn();
 * B = tmech::invf(A);
 * //or
 * B = tmech::inv(A);
 * \endcode
 *
 *
 * This function assumes, that a fourth order tensor has no minior-symmetry and is therefore
 * fully anisotropic. In this case the inverse is defined by
 * \f$ (\FourthT{A}^{-1})_{ijmn}\FourthT{A}_{mnkl} =\SecondT{I}_{ij}\SecondT{I}_{kl}\f$.
 * \code{.cpp}
 * tmech::tensor<double, 3, 4> A, B;
 * A.randn();
 * B = tmech::invf<tmech::sequence<1,2,3,4>>(A);
 * // the rule above is the most used one, therefore the following is sufficient.
 * // the correct indicies are set inside the function.
 * B = tmech::invf(A);
 * \endcode
 *
 * Using other kind of differentiation rules as \f$\FourthT{C}_{ijkl} = \frac{\partial \SecondT{a}_{ij} }{\partial \SecondT{b}_{kl}}\f$
 * must be indicated, due to the internal storage scheme.
 * For example using \f$\FourthT{C}_{iklj} = \frac{\partial \SecondT{a}_{ij} }{\partial \SecondT{b}_{kl}}\f$.
 * \code{.cpp}
 * tmech::tensor<double, 3, 4> A, B;
 * A.randn();
 * B = tmech::invf<tmech::sequence<1,3,4,2>>(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the inverse is to be formed.
*/
template<typename ..._Sequences, typename _Tensor, typename >
constexpr inline auto invf(_Tensor && __tensor);

/**
 * @brief Sign tensor decomposition of a second-order tensor
 * \f[ \SecondT{A} = \SecondT{S}\SecondT{N},
 * \quad \SecondT{S} = \text{sign}(\SecondT{A}),
 * \quad \SecondT{N} = \sqrt{\SecondT{A}\SecondT{A}}\f]
 * The decomposition is based on a Newton iteration as describet
 * <a href="https://www.sciencedirect.com/science/article/pii/002437959490393X">here</a>.
 * \f[
 * \SecondT{X}_{k+1} =\frac{1}{2}\left(\SecondT{X}_{k} + \SecondT{X}_{k}^{-1}\right), \quad \text{with} \quad \SecondT{X}_{0} = \SecondT{A}
 * \f]
 * which converges quadratically to \f$\text{sign}(\SecondT{A})\f$ for any \f$\SecondT{A}\f$ having no pure imaginary eigenvalues.
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A.randn();
 * B = tmech::sign(A);
 * \endcode
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression from which the inverse is to be formed.
 * @param __eps Tolerance for Newton iteration
 * @param __max_iter Maximum of Newton iterations
*/
template<typename _Tensor, typename >
constexpr inline auto sign(_Tensor && __tensor, typename std::decay<_Tensor>::type::value_type __eps = 5e-7, std::size_t __max_iter = 10);


/**
 * @brief Isotropic tensor function of a symmetric second-order tensor.
 *
 * \f$ \SecondT{Y} = \sum_{i=1}^m y_i \SecondT{E}_i\f$
 *
 * \code{.cpp}
 * struct sqrt{
 * template<typename  T>
 * static constexpr inline auto apply(T const& value){return std::sqrt(value);}
 *
 * template<typename  T>
 * static constexpr inline auto derivative(T const& value){return 1./(2.*std::sqrt(value));}
 * };
 *
 * tmech::tensor<double, 3, 2> A, B;
 * A.randn();
 * B = tmech::isotropic_tensor_function<sqrt>(A);
 * \endcode
 *
 * @tparam _Function
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Function, typename _Tensor, typename >
constexpr inline auto isotropic_tensor_function(_Tensor && __tensor);


/**
 * @brief Square root of a positive semi-definite symmetric second-order tensor
 * \f$ \text{sqrt}(\SecondT{A}) = \sqrt{\SecondT{A}}\f$.
 * The square is given by spectral decomposition
 * \f[
 * \sqrt{\SecondT{A}} = \sum_{i}^m \sqrt{\lambda_i} \SecondT{E}_i,
 * \f]
 * where \f$m\f$ is the number of non repeated eigenvalues \f$\lambda_i\f$ and \f$\SecondT{E}_i\f$ is the corresponding
 * eigenbase.
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * B = tmech::sqrt(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Tensor, typename >
constexpr inline auto sqrt(_Tensor && __tensor);

/**
 * @brief Exponential map \f$ \text{exp}(\SecondT{A})\f$ of a positive semi-definite symmetric second-order tensor.
 * The exponential map is given by spectral decomposition
 * \f[
 * \sqrt{\SecondT{A}} = \sum_{i}^m \exp{\lambda_i} \SecondT{E}_i,
 * \f]
 * where \f$m\f$ is the number of non repeated eigenvalues \f$\lambda_i\f$ and \f$\SecondT{E}_i\f$ is the corresponding
 * eigenbase.
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * B = tmech::exp_sym(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Tensor, typename >
constexpr inline auto exp_sym(_Tensor && __tensor);

/**
 * @brief Logarithmic map \f$ \text{log}(\SecondT{A})\f$ of a positive semi-definite symmetric second-order tensor.
 * The Logarithmic map is given by spectral decomposition
 * \f[
 * \sqrt{\SecondT{A}} = \sum_{i}^m \log{\lambda_i} \SecondT{E}_i,
 * \f]
 * where \f$m\f$ is the number of non repeated eigenvalues \f$\lambda_i\f$ and \f$\SecondT{E}_i\f$ is the corresponding
 * eigenbase.
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * B = tmech::log(A);
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Tensor, typename >
constexpr inline auto log(_Tensor && __tensor);

/**
 * @brief The positive part \f$ \SecondT{A}^{+} = \SecondT{A} - \SecondT{A}^{-}\f$
 * of a positive semi-definite symmetric second-order tensor is given by spectral decomposition
 * \f[
 * \SecondT{A}^{+} = \sum_{i}^m \text{pos}(\lambda_i) \SecondT{E}_i,\quad \text{with}
 * \quad
 * \text{pos}(\lambda_i)=
 * \begin{cases}
 * \lambda_i ~\text{if}~ \lambda_i > 0\\
 * 0  ~\text{if}~ \lambda_i < 0
 * \end{cases}
 * \f]
 * where \f$m\f$ is the number of non repeated eigenvalues \f$\lambda_i\f$ and \f$\SecondT{E}_i\f$ is the corresponding
 * eigenbase.
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * B = tmech::positive(A);
 * \endcode
 * If both parts \f$\SecondT{A}^{+}\f$ and \f$\SecondT{A}^{-}\f$ are needed use ``positive_negative_decomposition()``.
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Tensor, typename >
constexpr inline auto positive(_Tensor && __tensor);

/**
 * @brief The negative part \f$ \SecondT{A}^{-} = \SecondT{A} - \SecondT{A}^{+}\f$
 * of a positive semi-definite symmetric second-order tensor is given by spectral decomposition
 * \f[
 * {\SecondT{A}}^{-} = \sum_{i}^m \text{neg}(\lambda_i) \SecondT{E}_i,\quad \text{with}
 * \quad
 * \text{neg}(\lambda_i)=
 * \begin{cases}
 * \lambda_i ~\text{if}~ \lambda_i < 0\\
 * 0  ~\text{if}~ \lambda_i > 0
 * \end{cases}
 * \f]
 * where \f$m\f$ is the number of non repeated eigenvalues \f$\lambda_i\f$ and \f$\SecondT{E}_i\f$ is the corresponding
 * eigenbase.
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * B = tmech::negative(A);
 * \endcode
 * If both parts \f$\SecondT{A}^{+}\f$ and \f$\SecondT{A}^{-}\f$ are needed use ``positive_negative_decomposition()``.
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Tensor, typename >
constexpr inline auto negative(_Tensor && __tensor);

/**
 * @brief Positive and negative decomposition
 * part \f$ \SecondT{A} = \SecondT{A}^{+} + \SecondT{A}^{-}\f$
 * of a positive semi-definite symmetric second-order tensor is given by spectral decomposition
 * \f[
 * \SecondT{A}^{+} = \sum_{i}^m \text{pos}(\lambda_i) \SecondT{E}_i,\quad \text{with}
 * \quad
 * \text{pos}(\lambda_i)=
 * \begin{cases}
 * \lambda_i ~\text{if}~ \lambda_i > 0\\
 * 0  ~\text{if}~ \lambda_i < 0
 * \end{cases}\\
 * {\SecondT{A}}^{-} = \sum_{i}^m \text{neg}(\lambda_i) \SecondT{E}_i,\quad \text{with}
 * \quad
 * \text{neg}(\lambda_i)=
 * \begin{cases}
 * \lambda_i ~\text{if}~ \lambda_i < 0\\
 * 0  ~\text{if}~ \lambda_i > 0
 * \end{cases}
 * \f]
 * where \f$m\f$ is the number of non repeated eigenvalues \f$\lambda_i\f$ and \f$\SecondT{E}_i\f$ is the corresponding
 * eigenbase.
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, B;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * auto A_pos_neg = tmech::positive_negative_decomposition(A);
 * auto A_pos = A_pos_neg.positive();
 * auto A_neg = A_pos_neg.negative();
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression
*/
template<typename _Tensor, typename >
constexpr inline auto positive_negative_decomposition(_Tensor && __tensor);


/**
 * @brief Exponential map of a nonsymmetric second order tensor
 * \f[
 * \text{exp}\SecondT{A} = \sum_{n=0}^\infty \frac{1}{n!} \SecondT{A}^n
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A{9.064107e-01, -4.649874e-01,  4.431378e-01,
 *                               1.557860e+00,  2.493285e-01, -3.458549e-01,
 *                               1.747649e+00, -3.824761e-01, -8.110930e-01};
 *
 * tmech::tensor<double, 3, 2>Aexp{2.333496383103521e+00,  -9.424245337660712e-01,   6.174107765089896e-01,
 *                                 2.458154759604529e+00,   7.615487934000856e-01,   1.403592316302926e-01,
 *                                 1.701192794766057e+00,  -7.849258234991792e-01,   8.233614408596337e-01};
 *
 *  std::cout<<std::boolalpha
 *           <<tmech::almost_equal(Aexp, tmech::exp(A), 5e-7)<<std::endl;
 *
 *  //check the derivative
 *  const auto func_exp = [](auto const& tensor){return tmech::exp(tensor);};
 *  std::cout<<std::boolalpha
 *           <<tmech::almost_equal(tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func_exp,A), tmech::exp(A).derivative(), 5e-7)<<std::endl;
 *  \endcode
 *
*/
template<typename _Tensor, typename >
constexpr inline auto exp(_Tensor && __tensor);

/**
 * @brief Polar decomposition of a positive semi-definite symmetric second-order tensor
 * \f[
 * \SecondT{F}=\SecondT{R}\SecondT{U} = \SecondT{V}\SecondT{R}
 * \f]
 * where \f$\SecondT{R}\f$ is an orthogonal tensor also knwon as the rotation tensor,
 * \f$\SecondT{U}\f$ and \f$\SecondT{V}\f$ are symmetric tensors called the
 * right and the left stretch tensor, respectively.
 * This function provides two different method to determine \f$\SecondT{U}\f$, \f$\SecondT{V}\f$ and \f$\SecondT{R}\f$.
 * The frist method uses spectral decomposition
 * \f[
 * \SecondT{U} = \sqrt{\SecondT{F}^T\SecondT{F}}, \quad \SecondT{R} = \SecondT{F}\SecondT{U}^{-1},
 * \quad \SecondT{V} = \SecondT{R}\SecondT{U}\SecondT{R}^T,
 * \f]
 * \code{.cpp}
 * //use the spectral decomposition
 * tmech::tensor<double, 3, 2> F;
 * F.randn();
 * auto F_polar = tmech::polar_decomposition(F);
 * F = F_polar.R()*F_polar.U();
 * F = F_polar.V()*F_polar.R();
 * \endcode
 *
 * The second one is based on a <a href="https://en.wikipedia.org/wiki/Polar_decomposition">Newton iteration</a>
 * \f[
 * \SecondT{R}_{k+1} =\frac{1}{2} \left( \SecondT{R}_k + \SecondT{R}_k^{-T}\right), \quad \text{with}\quad \SecondT{R}_0 = \SecondT{F}
 * \f]
 *
 * \code{.cpp}
 * //use the newton iteration
 * tmech::tensor<double, 3, 2> F;
 * F.randn();
 * auto F_polar = tmech::polar_decomposition(F, // tensor to decompose
 *                                           true, //use newton iteration
 *                                           5e-7, // tolerance
 *                                           5 // number of max steps
 *                                           );
 * F = F_polar.R()*F_polar.U();
 * F = F_polar.V()*F_polar.R();
 * \endcode
 *
 * The following derivatives are also important
 * \f[
 * \frac{\partial \SecondT{U}}{\partial \SecondT{F}},
 * \quad \frac{\partial \SecondT{V}}{\partial \SecondT{F}},
 * \quad \frac{\partial \SecondT{R}}{\partial \SecondT{F}}
 * \f]
 * explicit results are given <a href="https://doi.org/10.1023/A:1007663620943">here</a>
 * \code{.cpp}
 * //use the spectral decomposition
 * tmech::tensor<double, 3, 2> F;
 * F.randn();
 * auto F_polar = tmech::polar_decomposition(F);
 * auto dR = F_polar.R().derivative();
 * auto dU = F_polar.U().derivative();
 * auto dV = F_polar.V().derivative();
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression, from which the polar decomposition is to be computed.
 * @param __newton_method True if Newton iteration should be used
 * @param __tol Tolerance for Newton iteration
 * @param __max_steps Maximum number of Newton iterations
*/
template<typename _Tensor, typename >
constexpr inline auto polar_decomposition(_Tensor && __tensor, bool const __newton_method, typename std::decay<_Tensor>::type::value_type const __tol, std::size_t const __max_steps);

/**
 * @brief Eigendecomposition of a positive semi-definite symmetric second-order tensor
 * \f[ \SecondT{Y} = \sum_{i=1}^m \lambda_i \FirstT{e}_i \otimes \FirstT{e}_i = \sum_{i=1}^m \lambda_i \SecondT{E}_i,\f]
 * where \f$m\f$ is the number of non repeated eigenvalues, \f$\lambda_i\f$ are the corresponding eigenvalues,
 * \f$\FirstT{e}_i\f$ eigenvectors and \f$\SecondT{E}_i\f$ eigenbasen.
 *
 * Using eigenbases to get the inverse
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, A_inv;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * auto A_eig = tmech::eigen_decomposition(A);
 * const auto[eigenvalues, eigenbasis]{A_eig.decompose_eigenbasis()};
 * const auto idx{A_eig.non_repeated_eigenvalues_index()};
 * for(std::size_t i{0}; i<A_eig.number_non_repeated_eigenvalues(); ++i){
 * A_inv += (1.0/eigenvalues[idx[i]])*eigenbasis[idx[i]];
 * }
 * std::cout<<std::boolalpha<<tmech::almost_equal(tmech::inv(A), A_inv, 5e-7)<<std::endl;
 * \endcode
 *
 * Using eigenvectors to get the inverse
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A, A_inv;
 * A = tmech::sym(tmech::randn<double,3,2>());
 * auto A_eig = tmech::eigen_decomposition(A);
 * const auto[eigenvalues, eigenvectors]{A_eig.decompose()};
 * const auto idx{A_eig.non_repeated_eigenvalues_index()};
 * for(std::size_t i{0}; i<A_eig.number_non_repeated_eigenvalues(); ++i){
 * A_inv += (1.0/eigenvalues[idx[i]])*tmech::otimes(eigenvectors[idx[i]],eigenvectors[idx[i]]);
 * }
 * std::cout<<std::boolalpha<<tmech::almost_equal(tmech::inv(A), A_inv, 5e-7)<<std::endl;
 * \endcode
 *
 * @tparam _Tensor Tensor object
 * @param __tensor Tensor expression, from which the eigen decomposition is to be computed.
*/
template<typename _Tensor, typename >
constexpr inline auto eigen_decomposition(_Tensor && __tensor);


/**
 * @brief The determinant of a second-order tensor object.
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A;
 * A.randn();
 * auto detA = tmech::det(A);
 * \endcode
 *
 * @param _Derived Tensor object from which the determinant is to be formed.
*/
template<typename _Derived>
constexpr inline auto det(tensor_base<_Derived>const& __A);


/**
 * @brief n-th Power of a second-order tensor object.
 *
 * \f[
 * \boldsymbol{a}^n &= \boldsymbol{a}\boldsymbol{a}
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double, 3, 2> A;
 * A.randn();
 * auto powA = tmech::pow(A, 3);
 * \endcode
 *
 * @param data_base Tensor object from which the n-th power is to be formed.
*/
template<typename _Tensor, typename T, typename, typename >
constexpr inline auto pow(_Tensor && __tensor, T const exp);

/**
 * @brief The (2)-norm of a tensor is defined for a first-, second- and fourth-order tensor as
 *
 * \f[
 * ||\mathbf{a}|| &= \sqrt{\text{dot}\left(\mathbf{a},\mathbf{a}\right)}  = \sqrt{a_ia_i} \\
 * ||\mathbf{A}|| &= \sqrt{\mathbf{A}:\mathbf{A}}= \sqrt{A_{ij}A_{ij}} \\
 * ||\mathbb{A}||     &= \sqrt{\mathbb{A}::\mathbb{A}}= \sqrt{A_{ijkl}A_{ijkl}}
 * \f]
 * @param data_base Tensor object from which the (2)-norm is to be formed.
 */
template<typename Derived>
constexpr inline auto norm(tensor_base<Derived> const& data_base);


/**
 * @brief Numerical differentiation based on central difference scheme
 * \f[
 *  f'(x) \approx \frac{f(x+h) - f(x-h)}{2h}
 * \f]
 *
 * \code{.cpp}
 * tmech::tensor<double,3,2> X;
 * X.randn();
 * auto func = [&](auto const& F){
 *             return 1.5*(tmech::trace(tmech::trans(F)*F) - 3);
 *             };
 *
 * auto dFunc = tmech::num_diff_central(func, X);
 * auto dFunc_ = [&](auto const& F){
 *               return tmech::num_diff_central(func, F);
 *               };
 * auto ddFunc = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func, X);
 * \endcode
 *
 * @param func Input type/function f(x), used for the numerical differentiation.
 * This type/function needs to provide an overloaded access operator(x).
 * @param x A given point, at which the function is evaluated.
 * @param __dx Is
 */
template<typename _Position = void, typename _Function, typename _Point>
inline auto num_diff_central(_Function __func, _Point const& __x, double const __h = 1e-7);


/**
 * @brief Symmetric numerical differentiation based on central difference scheme
 * \f[
 *  f'(x) \approx \frac{f(x+h) - f(x-h)}{2h}
 * \f]
 *
 * \code{.cpp}
 * //symmetry of the input
 * using Sym2x2 = std::tuple<tmech::sequence<1,2>,tmech::sequence<2,1>>;
 * //symmetry of the result
 * using Sym4x4 = std::tuple<
 *                tmech::sequence<1,2,3,4>,
 *                tmech::sequence<2,1,3,4>,
 *                tmech::sequence<1,2,4,3>,
 *                tmech::sequence<2,1,4,3>>;
 * tmech::tensor<double,3,2> X;
 * X = tmech::sym(tmech::rand<double,3,2>());
 * //first deriv
 * auto func = [&](auto const& F){
 *             return 1.5*(tmech::trace(tmech::trans(F)*F) - 3);
 *             };
 * auto dfunc_res = tmech::num_diff_sym_central<Sym2x2>(func, X);
 *
 * //second deriv
 * auto dfunc = [&](auto const& F){
 *               return tmech::num_diff_sym_central<Sym2x2>(func, F);
 *               };
 * auto ddfunc_res = tmech::num_diff_sym_central<Sym2x2, Sym4x4>(dfunc, X);
 * \endcode
 *
 * @param func Input type/function f(x), used for the numerical differentiation.
 * This type/function needs to provide an overloaded access operator(x).
 * @param x A given point, at which the function is evaluated.
 * @param __dx Is
 */
template<typename _SymDirection, typename _SymResult = std::tuple<>, typename _Function, typename _Point>
inline auto num_diff_sym_central(_Function __func, _Point const& __x, double const __h = 1e-7);

template<typename T>
constexpr inline auto convert_3D_to_2D(tensor<T, 3, 2> const& A);

template<typename T>
constexpr inline auto convert_3D_to_2D(tensor<T, 3, 4> const& A);

template<typename System, typename ...Data>
inline auto general_newton_raphson_iterate(System & A, std::tuple<Data...> & x, typename System::value_type const tol = 1e-8, std::size_t const max_iter = 20);


#endif // TENSOR_FUNCTIONS_FORWARD_H
