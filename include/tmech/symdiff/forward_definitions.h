/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef FORWARD_DEFINITIONS_H
#define FORWARD_DEFINITIONS_H

template <typename _T>
struct is_scalar;

template <typename _T>
struct is_tensor;

template <typename _T>
struct get_tensor_one;

template <typename _Derived>
class variable_base;

template <typename _T, std::size_t _VarID>
class variable;

template <typename _T, std::size_t _ID>
class constant;

template<typename _T, long long int _L, long long int _R, long long int _Exp>
class real;

template<typename _T>
class scalar_one;

template<typename _T>
class scalar_zero;

template<typename _T>
class tensor_one;

template<typename _T>
class tensor_zero;

namespace detail {
template<typename _Expr>
class negative;

template <typename _LHS, typename _RHS, typename _Op>
class binary_expression_wrapper;

template <typename _Expr>
class squeezer;

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct get_tensor;

template<typename _Tensor>
struct get_tensor_info;

template <typename _Tensor>
struct get_tensor_type;

template <typename _T>
struct get_identity_tensor;

template <typename _T>
struct get_zero_tensor;

template <typename _T>
struct is_tensor;

template <typename _T>
struct is_scalar;

template <typename _Type>
struct get_fundamental_one;

template <typename _LHS, typename _RHS>
struct get_fundamental_zero;

template <typename _Type, bool _isTensor>
struct get_fundamental_one_detail;

template <typename _LHS, bool _is_LHS_Tensor, typename _RHS, bool _is_RHS_Tensor>
struct get_fundamental_zero_detail;

template <typename _T>
struct print_operator;

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class tensor_inner_product_wrapper;

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class tensor_outer_product_wrapper;

template <typename _Expr, typename _Sequence>
class tensor_basis_change_wrapper;

template <typename _Expr, typename _Func>
class function_wrapper;

template <typename _Expr, typename _Func>
class tensor_function_wrapper;

template <typename _Base, typename _Exp, bool _IsFundamental>
class diff_pow;

template <typename _Expr, typename ..._Sequences>
class inv_tensor;

template <typename _Expr>
class symdiff_expression_wrapper;

template <typename _Expr, typename _Func>
class tensor_isotropic_function_wrapper;

template <typename _Father>
class tensor_isotropic_function_wrapper_derivative;

template <typename _Var>
class as_sym_wrapper;

template <typename _Expr, typename _RealExpo>
class tensor_pow_wrapper;

template <typename _Expr>
class tensor_pow_wrapper_derivative;

template <typename _Expr>
class tensor_dev_wrapper;

template <typename _Expr>
class tensor_vol_wrapper;

template <typename _Expr>
class tensor_sym_identity_wrapper;

template <typename _Expr>
class tensor_dev_identity_wrapper;

template <typename _Expr>
class tensor_vol_identity_wrapper;

//template <typename Jacobi, typename Functon>
//class newton_solver_multi_variable;

//template <typename Function, typename Variable>
//class newton_solver_single_variable;

template <typename Vector, typename ...Variables>
class jacobi_matrix_wrapper_mixed_type;

template <typename Jacobi, typename Functon>
class newton_raphson_solver;


template <typename ...Functions>
class vector_of_functions;

template <typename _Functions, typename _Variables>
class implicit_function;

template <typename Expression>
class derivative_wrapper;

} // NAMESPACE DETAIL


#endif // FORWARD_DEFINITIONS_H
