/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef DERIVATIVE_WRAPPER_BONES_H
#define DERIVATIVE_WRAPPER_BONES_H

namespace detail {
using op_mul = tmech::detail::operator_mul;
using op_add = tmech::detail::operator_add;
using op_div = tmech::detail::operator_div;
using op_sub = tmech::detail::operator_sub;

template <typename Variable, typename Expression>
class diff_wrapper;

//Summenregel
//f(x) = g(x) +- h(x)
//f(x)' = g(x)' +- h(x)'
template <typename _Variable, typename _LHS, typename _RHS, typename OP>
class diff_wrapper<_Variable, binary_expression_wrapper<_LHS, _RHS, OP>>
{
    using _dL = typename squeezer<typename diff_wrapper<_Variable, _LHS>::deriv_type >::squeezedType;
    using _dR = typename squeezer<typename diff_wrapper<_Variable, _RHS>::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer< binary_expression_wrapper<_dL, _dR, OP> >::squeezedType;
};

//Quotientenregel
//f(x) = g(x)/h(x)
//f(x) = (g(x)'*h(x) - g(x)*h(x)')/(h(x)*h(x)))
template <typename Variable, typename LHS, typename RHS>
class diff_wrapper<Variable, binary_expression_wrapper<LHS, RHS, op_div>>
{
    using _dL = typename squeezer< typename diff_wrapper< Variable, LHS >::deriv_type >::squeezedType;
    using _dR = typename squeezer< typename diff_wrapper< Variable, RHS >::deriv_type >::squeezedType;
    using _NL = typename squeezer< binary_expression_wrapper<_dL, RHS, op_mul > >::squeezedType;
    using _NR = typename squeezer< binary_expression_wrapper<LHS, _dR, op_mul > >::squeezedType;
    using _N  = typename squeezer< binary_expression_wrapper<_NL, _NR, op_sub > >::squeezedType;
    using _Z  = typename squeezer< binary_expression_wrapper<RHS, RHS, op_mul > >::squeezedType;
public:
    using deriv_type = typename squeezer< binary_expression_wrapper<_N, _Z, op_div > >::squeezedType;
};

//Produktregel
//f(x) = g(x)*h(x)
//f(x) = g(x)'*h(x) + g(x)*h(x)'
//both are scalars
template <typename _Variable, typename _LHS, typename _RHS, bool _isTensorVariable, bool _IsTensorLHS, bool _IsTensorRHS>
struct get_tensor_times_expression
{
    using _dL = typename squeezer< typename diff_wrapper< _Variable, _LHS >::deriv_type >::squeezedType;
    using _dR = typename squeezer< typename diff_wrapper< _Variable, _RHS >::deriv_type >::squeezedType;

    using deriv_type = typename squeezer< binary_expression_wrapper<
    typename squeezer< binary_expression_wrapper< _dL, _RHS, op_mul > >::squeezedType,
    typename squeezer< binary_expression_wrapper< _LHS, _dR, op_mul > >::squeezedType,
    op_add > >::squeezedType;
};

//f(x) = g(x)*H(x)
//f(x) = otimes(H(x), g(x)') + g(x)*H(x)'
//lhs is scalar
template <typename Variable, typename LHS, typename RHS>
struct get_tensor_times_expression<Variable, LHS, RHS, true, false, true>
{
    using _dL = typename squeezer< typename diff_wrapper< Variable, LHS >::deriv_type >::squeezedType;
    using _dR = typename squeezer< typename diff_wrapper< Variable, RHS >::deriv_type >::squeezedType;

//    using _rhs_tensor_info = get_tensor_info<typename RHS::data_type>;
//    using _var_tensor_info = get_tensor_info<typename Variable::data_type>;

//    static constexpr auto _RankLHS{_var_tensor_info::rank()};
//    static constexpr auto _RankRHS{_rhs_tensor_info::rank()};

//    using _SequenceLHS = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankRHS-1>, 1>;
//    using _SequenceRHS = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankRHS-1>, _RankLHS+1>;
    //typename squeezer< tensor_outer_product_wrapper<RHS, _dL, _SequenceLHS, _SequenceRHS>>::squeezedType;

    using lhs = typename squeezer< decltype (otimes(RHS(), _dL()))>::squeezedType;
    using rhs = typename squeezer< binary_expression_wrapper<LHS, _dR, op_mul>>::squeezedType;

    using deriv_type = typename squeezer< binary_expression_wrapper< lhs, rhs, op_add > >::squeezedType;
};

//f(x) = G(x)*h(x)
//f(x) = G(x)'*h(x) + otimes(G(x), h(x)')
//rhs is scalar
template <typename Variable, typename LHS, typename RHS>
struct get_tensor_times_expression<Variable, LHS, RHS, true, true, false>
{
    using deriv_type = typename get_tensor_times_expression<Variable, RHS, LHS, true, false, true>::deriv_type;
};


template <typename _Variable, typename _LHS, typename _RHS>
class diff_wrapper<_Variable, binary_expression_wrapper<_LHS, _RHS, op_mul>>
{
    using _dL                 = typename squeezer<typename diff_wrapper<_Variable, _LHS>::deriv_type >::squeezedType;
    using _dR                 = typename squeezer<typename diff_wrapper<_Variable, _RHS>::deriv_type >::squeezedType;
    using _LHS_New            = typename squeezer< binary_expression_wrapper<_dL, _RHS, op_mul> >::squeezedType;
    using _RHS_New            = typename squeezer< binary_expression_wrapper<_LHS, _dR, op_mul> >::squeezedType;
    using _lhs_data_type      = typename _LHS::data_type;
    using _rhs_data_type      = typename _RHS::data_type;
    using _variable_data_type = typename _Variable::data_type;
public:
    using deriv_type = typename get_tensor_times_expression<_Variable, _LHS, _RHS, is_tensor<_variable_data_type>::value,  is_tensor<_lhs_data_type>::value, is_tensor<_rhs_data_type>::value >::deriv_type;
};


//potenzregel
// g(x)^n n\in nat
// n*g(x)^(n-1)*g(x)'
//(x^n)'=n*x^(n-1)
template <typename _Variable, typename _Expr, typename _T, long long int _L, long long int _R, long long int _E>
class diff_wrapper<_Variable, diff_pow<_Expr, real<_T, _L, _R, _E>, true>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
    using _outer = typename squeezer<diff_pow<_Expr, real<_T, _L-1, _R, _E>, true>>::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<binary_expression_wrapper<real<_T, _L, _R, _E>, _outer, op_mul>, _dExpr, op_mul>>::squeezedType;
};

template <typename _Variable, typename _Expr, typename _Constant>
class diff_wrapper<_Variable, diff_pow<_Expr, _Constant, true>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
    using _outer = typename squeezer<diff_pow<_Expr, binary_expression_wrapper<_Constant, scalar_one<typename _Constant::data_type>, op_sub>, true>>::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<binary_expression_wrapper<_Constant, _outer, op_mul>, _dExpr, op_mul>>::squeezedType;
};

//Implizite
//f(x) = g(x)^h(x)
//f'(x) = f(x)*(h'(x)*log(g(x)) + h(x)*g'(x)/g(x))
//f'(x) = g(x)^h(x)*(h'(x)*log(g(x)) + h(x)*g'(x)/g(x)) |*g(x)
//f'(x) = g(x)^(h(x)-1)*(h'(x)*log(g(x))*g(x) + h(x)*g'(x))
template <typename Variable, typename G, typename H>
class diff_wrapper<Variable,  diff_pow<G, H, false>>
{
    using _dG = typename squeezer<typename diff_wrapper<Variable, G>::deriv_type >::squeezedType;
    using _dH = typename squeezer<typename diff_wrapper<Variable, H>::deriv_type >::squeezedType;
    using _Hn = typename squeezer<binary_expression_wrapper<H, real<typename H::data_type, 1, 0, 1>, op_sub>>::squeezedType;
    using _Log = function_wrapper<G, log_wrapper>;
    using _Pow = diff_pow<G, _Hn, false>;
    using _RHS1 = typename squeezer<binary_expression_wrapper<typename squeezer<binary_expression_wrapper<_dH, _Log, op_mul>>::squeezedType, G, op_mul>>::squeezedType;
    using _RHS2 = typename squeezer<binary_expression_wrapper<H, _dG, op_mul>>::squeezedType;
    using _RHS = typename squeezer<binary_expression_wrapper<_RHS1, _RHS2, op_add>>::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_Pow, _RHS, op_mul>>::squeezedType;
};

//f(x) = abs(g(x))
//f'(x) = g'(x)*sign(g(x)) = g'(x)*(g(x)/g(x))
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, abs_wrapper>>
{
    using _dExpr    = typename squeezer< typename diff_wrapper< _Variable, _Expr >::deriv_type >::squeezedType;
    using sign_expr = function_wrapper<_Expr, sign_wrapper>;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_dExpr, sign_expr, op_mul>>::squeezedType;
};

////f(x) = sign(g(x)) = g(x)/abs(g(x))
////f'(x) = g(x)'/abs(g(x)) + (1/abs(g(x)))'g(x)
//template <typename _Variable, typename _Expr>
//class diff_wrapper<_Variable, function_wrapper<_Expr, sign_wrapper>>
//{
//    using _dExpr = typename squeezer< typename diff_wrapper< _Variable, binary_expression_wrapper<_Expr, function_wrapper<_Expr, abs_wrapper>, op_div> >::deriv_type >::squeezedType;
//public:
//    using deriv_type = _dExpr;
//};

//exponent funktion
//(e^g(x))' = e^g(x)*g(x)'
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, exp_wrapper>>
{
    using _dExpr = typename squeezer< typename diff_wrapper< _Variable, _Expr >::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer< binary_expression_wrapper< function_wrapper< _Expr, exp_wrapper >, _dExpr, op_mul > >::squeezedType;
};

//natural logarithmus
//f(x) = log(g(x))
//log(g(x))' = (1/g(x))*g(x)'
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, log_wrapper>>
{
    using _dExpr = typename squeezer< typename diff_wrapper< _Variable, _Expr >::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer< binary_expression_wrapper< _dExpr, _Expr, op_div > >::squeezedType;
};


//f(x) = sqrt(g(x))
//f'(x) = (g'(x)/(2*sqrt(g(x))))
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, sqrt_wrapper>>
{
    using data_type   = typename _Expr::data_type;
    using _dExpr      = typename squeezer< typename diff_wrapper< _Variable, _Expr >::deriv_type >::squeezedType;
    using denominator = typename squeezer< binary_expression_wrapper< real< data_type, 2, 0, 1 >, function_wrapper< _Expr, sqrt_wrapper >, op_mul>>::squeezedType;
public:
    using deriv_type = typename squeezer< binary_expression_wrapper<_dExpr, denominator, op_div>>::squeezedType;
};


//f(x) = cos(g(x))
//f'(x) = g'(x)*(-sin(g(x))) = -g'(x)*sin(g(x))
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, cos_wrapper>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using deriv_type = negative<typename squeezer<binary_expression_wrapper<_dExpr, function_wrapper<_Expr, sin_wrapper>, op_mul>>::squeezedType>;
};


//f(x) = sin(g(x))
//f'(x) = g'(x)*cos(g(x))
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, sin_wrapper>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_dExpr, function_wrapper<_Expr, cos_wrapper>, op_mul>>::squeezedType;
};

//f(x) = tan(g(x))
//f'(x) = g'(x)*sec^2(g(x))
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, tan_wrapper>>
{
    using data_type = typename _Expr::data_type;
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_dExpr, diff_pow<function_wrapper<_Expr, sec_wrapper>, real<data_type, 2,0,1>, true>, op_mul>>::squeezedType;
};

//f(x) = sec(g(x)) = 1/cos(g(x)) = cos(g(x))^-1
//f'(x) = g(x)'tan(g(x))*sec(g(x))
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, sec_wrapper>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_dExpr, binary_expression_wrapper<function_wrapper<_Expr, tan_wrapper>, function_wrapper<_Expr, sec_wrapper>, op_mul>,op_mul>>::squeezedType;
};

//negativ wrapper
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, negative<_Expr>>{
public:
    using deriv_type = typename squeezer<negative<typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type>::squeezedType>>::squeezedType;
};

//dx/dx = 1
template <typename Variable>
class diff_wrapper<Variable, Variable>
{
    using data_type = typename Variable::data_type;
public:
    using deriv_type = typename fundamental_one_derivative< data_type >::type;
};

//real constant
template <typename Variable, typename _T, long long int _L, long long int  _R, long long int  _Exp>
class diff_wrapper<Variable, real<_T, _L, _R, _Exp>>
{
    using data_type = typename Variable::data_type;
public:
    using deriv_type = typename get_fundamental_zero< data_type, _T >::type;
};

//scalar_one
template <typename _Variable, typename _T>
class diff_wrapper<_Variable, scalar_one<_T>>
{
    using data_type = typename _Variable::data_type;
public:
    using deriv_type = typename get_fundamental_zero< data_type, _T >::type;
};

//tensor_one
template <typename _Variable, typename _T>
class diff_wrapper<_Variable, tensor_one<_T>>
{
    using data_type = typename _Variable::data_type;
public:
    using deriv_type = typename get_fundamental_zero< data_type, _T >::type;
};

//scalar_zero
template <typename _Variable, typename _T>
class diff_wrapper<_Variable, scalar_zero<_T>>
{
    using data_type = typename _Variable::data_type;
public:
    using deriv_type = typename get_fundamental_zero< data_type, _T >::type;
};

//tensor_zero
template <typename _Variable, typename _T>
class diff_wrapper<_Variable, tensor_zero<_T>>
{
    using data_type = typename _Variable::data_type;
public:
    using deriv_type = typename get_fundamental_zero< data_type, _T >::type;
};

//dconstant/dx = 0
template <typename _Variable, typename _T, std::size_t _ID>
class diff_wrapper<_Variable, constant<_T, _ID>>
{
    using data_type = typename _Variable::data_type;
public:
    using deriv_type = typename get_fundamental_zero< data_type, _T >::type;
};

//dx/dx = 1
template <typename _T, std::size_t _ID>
class diff_wrapper<variable<_T, _ID>, variable<_T, _ID>>{
public:
    using deriv_type = typename fundamental_one_derivative< _T >::type;
};

//dy/dx = 0
template <typename T_LHS, std::size_t ID_LHS, typename T_RHS, std::size_t ID_RHS>
class diff_wrapper<variable<T_LHS, ID_LHS>, variable<T_RHS, ID_RHS>>{
public:
    using deriv_type = typename get_fundamental_zero< T_LHS, T_RHS >::type;
};

//symmetric argument
//x' = 0.5*(otimesu(I,I) + otimesl(I,I))
template <typename _Variable>
class diff_wrapper<_Variable, as_sym_wrapper<_Variable>>
{
    using data_type   = typename _Variable::data_type;
    using tensor_info = get_tensor_info<data_type>;
    using I           = tensor_one<data_type>;
    using otimesu     = tensor_outer_product_wrapper<I, I, tmech::sequence<1,3>, tmech::sequence<2,4>>;
    using otimesl     = tensor_outer_product_wrapper<I, I, tmech::sequence<1,4>, tmech::sequence<2,3>>;
    using _05         = real<typename tensor_info::value_type, 5, 0, 0>;
public:
    using deriv_type = tensor_sym_identity_wrapper<_Variable>;
    //using deriv_type = binary_expression_wrapper<_05, binary_expression_wrapper<otimesu, otimesl, op_add>, op_mul>;
};

//symmetric argument
//dadx = 0
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, as_sym_wrapper<_Expr>>
{
public:
    using deriv_type = typename get_fundamental_zero< typename _Variable::data_type, typename _Expr::data_type >::type;
};


//define derivative
//dX/dx --> 0<X::rank(), X::dimension()>
//dX/dX = dX_{ij}/dX_{kl} = outer<I, I, <1,2>, <3,4>>
//II := outer<I, I, <1,2>, <3,4>>
//dX/dX = dX_{ijkl}/dX_{mnop} = outer<II, II, <1,2,3,4>, <5,6,7,8>>


// d(AiBj)/dXmn = D_{ijmn}
// outer<A,B,<I>,<J>>
// outer<dA, B, <1,3,4>, <2>> + outer<A, dB, <1>, <2,3,4>>

// d(XijXkl)/dXmn = D_{ijklmn}
// outer<X,X,<I,J>,<K,L>>
// outer<dX, X, <I,J,7,8>, <K,L>> + outer<X, dX, <I,J>, <K,L,7,8>>

// outer<X,X,<ArgsLHS...>,<ArgsRHS...>>
// RankOuter = sizeof...(ArgsLHS) + sizeof...(ArgsRHS)
// RankArgument = X::rank()
// RankResult = RankOuter + RankArgument

//outer product
//derivative w.r.t. a scalar
template <typename _Variable, bool _isVariableTensor,  typename _LHS, typename _RHS, typename _SeqLHS, typename _SeqRHS>
class get_tensor_outer_product_differentiation
{
    using _dL = typename squeezer<typename diff_wrapper<_Variable, _LHS>::deriv_type >::squeezedType;
    using _dR = typename squeezer<typename diff_wrapper<_Variable, _RHS>::deriv_type >::squeezedType;

    using _lhs = typename squeezer<tensor_outer_product_wrapper<_dL, _RHS, _SeqLHS, _SeqRHS>>::squeezedType;
    using _rhs = typename squeezer<tensor_outer_product_wrapper<_LHS, _dR, _SeqLHS, _SeqRHS>>::squeezedType;
public:
    using type = typename squeezer<binary_expression_wrapper<_lhs, _rhs, op_add>>::squeezedType;
};

//derivative w.r.t. a tensor
//otimes(A,B) = outer<>(A,B) + outer<>(A,B)
template <typename _Variable,  typename _LHS, typename _RHS, typename _SeqLHS, typename _SeqRHS>
class get_tensor_outer_product_differentiation<_Variable, true, _LHS, _RHS, _SeqLHS, _SeqRHS>
{
    using _lhs_data_type = typename _LHS::data_type;
    using _rhs_data_type = typename _RHS::data_type;
    using _var_data_type = typename _Variable::data_type;
    using _lhs_tensor_info = get_tensor_info<_lhs_data_type>;
    using _rhs_tensor_info = get_tensor_info<_rhs_data_type>;
    using _var_tensor_info = get_tensor_info<_var_data_type>;

    static constexpr auto _RankLHS{_lhs_tensor_info::rank()};
    static constexpr auto _RankRHS{_rhs_tensor_info::rank()};
    static constexpr auto _RankOuter{_RankLHS + _RankRHS};
    static constexpr auto _RankArgument{_var_tensor_info::rank()};
    static constexpr auto _RankResult{_RankArgument + _RankOuter};

    using _res_seq = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankArgument-1>, _RankOuter+1>;
    using _lhs_seq = tmech::detail::append_sequence_end_t<_SeqLHS, _res_seq>;
    using _rhs_seq = tmech::detail::append_sequence_end_t<_SeqRHS, _res_seq>;

    using _dL = typename squeezer<typename diff_wrapper<_Variable, _LHS>::deriv_type >::squeezedType;
    using _dR = typename squeezer<typename diff_wrapper<_Variable, _RHS>::deriv_type >::squeezedType;

    using _lhs = typename squeezer<tensor_outer_product_wrapper<_dL, _RHS, _lhs_seq, _SeqRHS>>::squeezedType;
    using _rhs = typename squeezer<tensor_outer_product_wrapper<_LHS, _dR, _SeqLHS, _rhs_seq>>::squeezedType;
public:
    using type = typename squeezer<binary_expression_wrapper<_lhs, _rhs, op_add>>::squeezedType;
};

template <typename _Variable, typename _LHS, typename _RHS, typename _SeqLHS, typename _SeqRHS>
class diff_wrapper<_Variable, tensor_outer_product_wrapper<_LHS, _RHS, _SeqLHS, _SeqRHS>>
{
public:
    using deriv_type = typename get_tensor_outer_product_differentiation<_Variable, is_tensor<typename _Variable::data_type>::value, _LHS, _RHS, _SeqLHS, _SeqRHS>::type;
};




//inner product
//(A_ikB_kj)'mn = A_ikmn B_kj + A_ik B_kjmn
//inner<2,1>(A,B)' = basis_change<1,4,2,3>(inner<2,1>(A',B)) + inner<2,1>(A,B')
//lhs <1,2> <2> <1,2,3,4>
//rhs <1,2> <1>
//result <1,4,2,3>
//Input <1,2> <3,4>
//Number of free indices LHS 1
//Number of free indices RHS 1
//<1> + <1+ResltRank-1> + <2,3>
//<1,4,2,3>
//(A_ijmnB_mnkl)'qr = A_ijmnqr B_mnkl + A_ijmn B_mnklqr
//<1,2> <3,4> <1,2,3,4,5,6>
//remove contracting indices <<1,2,3,4,5,6>, <3,4>> --> <1,2,5,6>
//add contraction indices add end <<1,2,5,6>, <3,4>> --> <1,2,5,6,3,4>
//basis_change<1,2,5,6,3,4>
// inner<A, B, <3,4>, <1,2>>
// inner<dA, B, <3,4>, <1,2>> + inner<A, dB, <3,4>, <1,2>>
template <typename _Variable, typename _LHS, typename _RHS, typename _SeqLHS, typename _SeqRHS>
class diff_wrapper<_Variable, tensor_inner_product_wrapper<_LHS, _RHS, _SeqLHS, _SeqRHS>>{
    using _dL = typename squeezer<typename diff_wrapper<_Variable, _LHS>::deriv_type >::squeezedType;
    using _dR = typename squeezer<typename diff_wrapper<_Variable, _RHS>::deriv_type >::squeezedType;

    using _lhs_data_type = typename _LHS::data_type;
    using _rhs_data_type = typename _RHS::data_type;
    using _var_data_type = typename _Variable::data_type;
    using _lhs_tensor_info = get_tensor_info<_lhs_data_type>;
    using _rhs_tensor_info = get_tensor_info<_rhs_data_type>;
    using _var_tensor_info = get_tensor_info<_var_data_type>;

    static constexpr auto _InnerSizeLHS{_SeqLHS::size()};
    static constexpr auto _InnerSizeRHS{_SeqRHS::size()};
    static constexpr auto _RankLHS{_lhs_tensor_info::rank()};
    static constexpr auto _RankRHS{_rhs_tensor_info::rank()};
    static constexpr auto _RankArgument{_var_tensor_info::rank()};
    static constexpr auto _ResultRank{_RankLHS + _RankRHS + _RankArgument - _InnerSizeLHS - _InnerSizeRHS};

    using lhs_seq = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankLHS-_InnerSizeLHS-1>,1>;
    using rhs_seq = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankRHS-_InnerSizeRHS-1>, _RankLHS-_InnerSizeLHS+1>;
    using result_seq = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankArgument-1>, _RankLHS-_InnerSizeLHS + _RankRHS-_InnerSizeRHS + 1>;
    using final_seq = tmech::detail::append_sequence_end_t<tmech::detail::append_sequence_end_t<lhs_seq,result_seq>,rhs_seq>;

    using _lhs = typename squeezer<tensor_basis_change_wrapper<typename squeezer<tensor_inner_product_wrapper<_dL, _RHS, _SeqLHS, _SeqRHS>>::squeezedType, final_seq>>::squeezedType;
    using _rhs = typename squeezer<tensor_inner_product_wrapper<_LHS, _dR, _SeqLHS, _SeqRHS>>::squeezedType;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_lhs, _rhs, op_add>>::squeezedType;
};

//trans(F)*F = inner<2,1>(otimesl(I,I)*F) + inner<2,1>(trans(F)*otimesu(I,I))
//basis  change
//basis_change<<2,1>>()
//dA^T/dA = basis_change<<2,1>>(dA_{ij}/dA_{kl})
//        = basis_change<<2,1>>(I_{ik}I_{jl})
template <typename _Variable, bool _isVariableTensor, typename _Expr, typename _Sequence>
class get_tensor_basis_change_differentiation
{
    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using type = typename squeezer<tensor_basis_change_wrapper<_dExpr, _Sequence> >::squeezedType;
};

template <typename _Variable, typename _Expr, typename _Sequence>
class get_tensor_basis_change_differentiation<_Variable, true, _Expr, _Sequence>
{
    using _expr_data_type   = typename _Expr::data_type;
    using _var_data_type    = typename _Variable::data_type;
    using _expr_tensor_info = get_tensor_info<_expr_data_type>;
    using _var_tensor_info  = get_tensor_info<_var_data_type>;

    static constexpr auto _RankExpr{_expr_tensor_info::rank()};
    static constexpr auto _RankArgument{_var_tensor_info::rank()};
    static constexpr auto _RankResult{_RankArgument + _RankExpr};

    using _res_seq  = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_RankArgument-1>, _RankResult-1>;
    using _expr_seq = tmech::detail::append_sequence_end_t<_Sequence, _res_seq>;

    using _dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;

public:
    using type = typename squeezer< tensor_basis_change_wrapper<_dExpr, _expr_seq> >::squeezedType;
};

template <typename _Variable, typename _Expr, typename _Sequence>
class diff_wrapper<_Variable, tensor_basis_change_wrapper<_Expr, _Sequence>>
{
public:
    using deriv_type = typename get_tensor_basis_change_differentiation<_Variable, is_tensor<typename _Variable::data_type>::value, _Expr, _Sequence>::type;
};


//determinat
//det(X)' = det(X)*trans(inv(X)) = cof(X)
template <typename _Variable>
class diff_wrapper<_Variable, function_wrapper<_Variable, det_wrapper>>
{
public:
using deriv_type = typename squeezer< tensor_function_wrapper<_Variable, cof_wrapper> >::squeezedType;
};

template <typename _Variable>
class diff_wrapper<_Variable, function_wrapper<as_sym_wrapper<_Variable>, det_wrapper>>
{
public:
using deriv_type = typename squeezer< tensor_function_wrapper<as_sym_wrapper<_Variable>, cof_wrapper> >::squeezedType;
};

//determinat
//det(A[X])' = inner<<1,2>,<1,2>>(det(A[X])',A[X]')
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, det_wrapper>>
{
    using _dExpr    = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type>::squeezedType;
    using _ddetExpr = typename squeezer<typename diff_wrapper<_Expr, function_wrapper<_Expr, det_wrapper>>::deriv_type>::squeezedType;
public:
    using deriv_type = typename squeezer<tensor_inner_product_wrapper<_ddetExpr, _dExpr, tmech::sequence<1,2>,tmech::sequence<1,2>>>::squeezedType;
};

//cofactor
//cof(X)' = (det(X)*trans(inv(X)))'
//        = otimes(trans(inv(X)), cof(X)) - det(X)*basis_change<2,1,3,4>(otimesu(inv(X),inv(X)))
template <typename _Variable>
class diff_wrapper<_Variable, tensor_function_wrapper<_Variable, cof_wrapper>>
{
    using invT = typename squeezer< decltype (trans(inv_tensor<_Variable>())) >::squeezedType;
    using function = binary_expression_wrapper<function_wrapper<_Variable, det_wrapper>, invT, op_mul>;
public:
using deriv_type = typename squeezer< typename diff_wrapper<_Variable, function>::deriv_type >::squeezedType;
};


//f(F[X])'_X = func(F)*F[X]'_X


//f(F[X])'_X = inner_product<>(f(F[X])'_F[X], F[X]'_X)
//sequence<>::size() == F[X].rank()
//df(F[X])_{ij...}/dF[X]_{kl...} * dF[X]_{kl...}/dX{mn....}
template <typename _Variable, typename _Expr, typename _Func>
class diff_wrapper<_Variable, tensor_function_wrapper<_Expr, _Func>>
{
    using _dFunc = typename squeezer< typename diff_wrapper<_Expr, tensor_function_wrapper<_Expr, _Func>>::deriv_type >::squeezedType;
    using _dExpr = typename squeezer< typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
    using _FuncTensorInfo = get_tensor_info<typename tensor_function_wrapper<_Expr, _Func>::data_type>;
    using _VarTensorInfo  = get_tensor_info<typename _Variable::data_type>;
    using _SeqFuncLHS = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_FuncTensorInfo::rank()-1>, _FuncTensorInfo::rank()>;
    using _SeqFuncRHS = tmech::detail::sequence_t<_FuncTensorInfo::rank()-1>;
public:
    using deriv_type = typename squeezer<tensor_inner_product_wrapper<_dFunc, _dExpr, _SeqFuncLHS, _SeqFuncRHS>>::squeezedType;
};



////cof(A[X])' = (det(A[X])*trans(inv(A[X])))'
////           = inner_product<3,4,1,2>(cof(A)'_A, A'_X)
//template <typename _Variable, typename _Expr>
//class diff_wrapper<_Variable, tensor_function_wrapper<_Expr, cof_wrapper>>
//{
//    using dCof  = typename diff_wrapper<_Expr, tensor_function_wrapper<_Expr, cof_wrapper>>::deriv_type;
//    using dExpr = typename diff_wrapper<_Variable, tensor_function_wrapper<_Expr, cof_wrapper>>::deriv_type;
//public:
//using deriv_type = typename squeezer< typename diff_wrapper<_Variable, function>::deriv_type >::squeezedType;
//};


//cofactor symmetric
//cof(as_sym(X))' = (det(as_sym(X))*inv(as_sym(X)))'
//det(as_sym(X)) = det(X)*inv(X)
//inv(as_sym(X)) = -0.5*(otimes(inv(X),inv(X)) + inv(X),inv(X))

//adjoint
//adj(X)' = trans(cof(X))'
template <typename _Variable>
class diff_wrapper<_Variable, tensor_function_wrapper<_Variable, adj_wrapper>>
{
    using function = decltype (trans(tensor_function_wrapper<_Variable, cof_wrapper>()));
public:
using deriv_type = typename squeezer< typename diff_wrapper<_Variable, function>::deriv_type >::squeezedType;
};

//trace
//trace(X)' = I
template <typename _Variable>
class diff_wrapper<_Variable, function_wrapper<_Variable, trace_wrapper>>
{
public:
    using deriv_type = tensor_one<typename _Variable::data_type>;
};

//trace(A[X])' = (A[X]:I)' = partial(A[X]_{kl}*I_{kl})/partial X_{ij}
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<_Expr, trace_wrapper>>
{
    using I      = tensor_one<typename _Variable::data_type>;
    using _dExpr = typename squeezer< typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer< tensor_inner_product_wrapper<_dExpr, I, tmech::sequence<1,2>, tmech::sequence<1,2>>>::squeezedType;
};

template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, function_wrapper<as_sym_wrapper<_Expr>, trace_wrapper>>
{
    using I      = tensor_one<typename _Variable::data_type>;
    using _dExpr = typename squeezer< typename diff_wrapper<_Variable, _Expr>::deriv_type >::squeezedType;
public:
    using deriv_type = typename squeezer< tensor_inner_product_wrapper<_dExpr, I, tmech::sequence<1,2>, tmech::sequence<1,2>>>::squeezedType;
};

//deviatoric
//dev(X) = X - trace(X)*I
//dev(X)' = IIdev
template <typename Variable>
class diff_wrapper<Variable, tensor_dev_wrapper<Variable>>
{
public:
    using deriv_type = tensor_dev_identity_wrapper<Variable>;
};

//dev(A[X]) = A[X] - trace(A[X])*I
//dev(X)' = IIdev:A,X
template <typename Variable, typename _Expr>
class diff_wrapper<Variable, tensor_dev_wrapper<_Expr>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<Variable, _Expr>::deriv_type>::squeezedType;
    using _IIdev = tensor_dev_identity_wrapper<Variable>;
    using _SeqR = tmech::sequence<1,2>;
    using _SeqL = tmech::sequence<3,4>;
public:
    using deriv_type = typename squeezer<tensor_inner_product_wrapper<_IIdev, _dExpr, _SeqL, _SeqR>>::squeezedType;
};


//volumetric
//vol(X) = trace(X)*I
//vol(X)' = IIvol
template <typename Variable>
class diff_wrapper<Variable, tensor_vol_wrapper<Variable>>
{
public:
    using deriv_type = tensor_vol_identity_wrapper<Variable>;
};

//vol(A[X]) = trace(A[X])*I
//vol(X)' = IIvol:A,X
template <typename Variable, typename _Expr>
class diff_wrapper<Variable, tensor_vol_wrapper<_Expr>>
{
    using _dExpr = typename squeezer<typename diff_wrapper<Variable, _Expr>::deriv_type>::squeezedType;
    using _IIvol = tensor_vol_identity_wrapper<Variable>;
    using _SeqR = tmech::sequence<1,2>;
    using _SeqL = tmech::sequence<3,4>;
public:
    using deriv_type = typename squeezer<tensor_inner_product_wrapper<_IIvol, _dExpr, _SeqL, _SeqR>>::squeezedType;
};

//inverse
//inv(X)′=-otimes(inv(X),inv(X))
//inv(X)_ik X_kj = I_ij
//inv(X)_ik/X_mn X_kj + inv(X)_ik X_kj/X_mn = 0_ijmn
//inv(X)_ik/X_mn = - inv(X)_ik X_kj/X_mn inv(X)_kj
//inv(X)_ik/X_mn = - otimesu(inv(X), trans(inv(X)))
template <typename Variable>
class diff_wrapper<Variable, inv_tensor<Variable>>
{
public:
    using deriv_type = negative<tensor_outer_product_wrapper<inv_tensor<Variable>, decltype (trans(inv_tensor<Variable>())), tmech::sequence<1,3>, tmech::sequence<2,4>>>;
};

//inv(G(X))′= - inner<<3,4><1,2>>(otimesu(inv(X), inv(X)), G(X)')
template <typename _Variable, typename _Expr>
class diff_wrapper<_Variable, inv_tensor<_Expr>>
{
    using _dinvExpr = typename squeezer<typename diff_wrapper<_Expr, inv_tensor<_Expr>>::deriv_type>::squeezedType;
    using _dExpr    = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type>::squeezedType;
public:
    using deriv_type = typename squeezer<tensor_inner_product_wrapper<_dinvExpr, _dExpr, tmech::sequence<3,4>,tmech::sequence<1,2>>>::squeezedType;
};

//isotropic tensor functions
template <typename _Variable, typename _Func>
class diff_wrapper<_Variable, tensor_isotropic_function_wrapper<_Variable, _Func>>
{
public:
    using deriv_type = tensor_isotropic_function_wrapper_derivative<tensor_isotropic_function_wrapper<_Variable, _Func>>;
};

//power function
//pow(X,n) = pow(X,n)'
template <typename _Variable, typename _RealExpo>
class diff_wrapper<_Variable, tensor_pow_wrapper<_Variable, _RealExpo>>
{
public:
    using deriv_type = tensor_pow_wrapper_derivative<tensor_pow_wrapper<_Variable, _RealExpo>>;
};

//pow(A[X],n) = pow(A[X],n),A[X] : A,X
template <typename _Variable, typename _Expr, typename _RealExpo>
class diff_wrapper<_Variable, tensor_pow_wrapper<_Expr, _RealExpo>>
{
    using SeqL = tmech::sequence<3,4>;
    using SeqR = tmech::sequence<1,2>;
    using dExpr = typename squeezer<typename diff_wrapper<_Variable, _Expr>::deriv_type>::squeezedType;
public:
    using deriv_type = typename squeezer<tensor_inner_product_wrapper<tensor_pow_wrapper_derivative<tensor_pow_wrapper<_Expr, _RealExpo>>, dExpr, SeqL, SeqR>>::squeezedType;
};

//trace(pow(A,n))' = n*pow(trace(trans(A)),n-1)
template <typename _Variable, typename _RealExpo>
class diff_wrapper<_Variable,  function_wrapper<tensor_pow_wrapper<_Variable, _RealExpo>, trace_wrapper>>
{
    using n_1 = typename squeezer<binary_expression_wrapper<_RealExpo, scalar_one<typename _RealExpo::data_type>, op_sub>>::squeezedType;
    using varT = tensor_basis_change_wrapper<_Variable, tmech::sequence<2,1>>;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<_RealExpo, tensor_pow_wrapper<varT, n_1>, op_mul>>::squeezedType;
};

template <typename _Variable, typename _T, long long int _L, long long int _R>
class diff_wrapper<_Variable,  function_wrapper<tensor_pow_wrapper<_Variable, real<_T, _L, _R, 1>>, trace_wrapper>>
{
    using n_1 = real<_T, _L-1, _R, 1>;
    using varT = tensor_basis_change_wrapper<_Variable, tmech::sequence<2,1>>;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<real<_T, _L, _R, 1>, tensor_pow_wrapper<varT, n_1>, op_mul>>::squeezedType;
};

template <typename _Variable, typename _T>
class diff_wrapper<_Variable, function_wrapper<tensor_pow_wrapper<_Variable, real<_T, 2, 0, 1>>, trace_wrapper>>
{
    using varT = tensor_basis_change_wrapper<_Variable, tmech::sequence<2,1>>;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<real<_T, 2, 0, 1>, varT, op_mul>>::squeezedType;
};


template <typename _Variable, template<class> class wrapper, typename _T, long long int _L, long long int _R>
class diff_wrapper<_Variable, function_wrapper<tensor_pow_wrapper<wrapper<as_sym_wrapper<_Variable>>, real<_T, _L, _R, 1>>, trace_wrapper>>
{
    using n_1 = real<_T, _L-1, _R, 1>;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<real<_T, _L, _R, 1>, tensor_pow_wrapper<wrapper<as_sym_wrapper<_Variable>>, n_1>, op_mul>>::squeezedType;
};

template <typename _Variable, template<class> class wrapper, typename _T>
class diff_wrapper<_Variable, function_wrapper<tensor_pow_wrapper<wrapper<as_sym_wrapper<_Variable>>, real<_T, 2, 0, 1>>, trace_wrapper>>
{
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<real<_T, 2, 0, 1>, wrapper<as_sym_wrapper<_Variable>>, op_mul>>::squeezedType;
};

template <typename _Variable, typename _T, long long int _L, long long int _R>
class diff_wrapper<_Variable, function_wrapper<tensor_pow_wrapper<as_sym_wrapper<_Variable>, real<_T, _L, _R, 1>>, trace_wrapper>>
{
    using n_1 = real<_T, _L-1, _R, 1>;
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<real<_T, _L, _R, 1>, tensor_pow_wrapper<as_sym_wrapper<_Variable>, n_1>, op_mul>>::squeezedType;
};

template <typename _Variable, typename _T>
class diff_wrapper<_Variable, function_wrapper<tensor_pow_wrapper<as_sym_wrapper<_Variable>, real<_T, 2, 0, 1>>, trace_wrapper>>
{
public:
    using deriv_type = typename squeezer<binary_expression_wrapper<real<_T, 2, 0, 1>, as_sym_wrapper<_Variable>, op_mul>>::squeezedType;
};

//pow(trace(A),n)' = n*pow(A,n-1)*I
template <typename _Variable, typename _RealExpo>
class diff_wrapper<_Variable, diff_pow<function_wrapper<_Variable, trace_wrapper>, _RealExpo>>
{
    using n_1 = typename squeezer<binary_expression_wrapper<_RealExpo, scalar_one<typename _RealExpo::data_type>, op_sub>>::squeezedType;
    using I = tensor_one<typename _Variable::data_type>;
public:
    using deriv_type = binary_expression_wrapper<binary_expression_wrapper<_RealExpo, diff_pow<function_wrapper<_Variable, trace_wrapper>, n_1>, op_mul>, I, op_mul>;
};


//implicit functions
template <typename _Variable, typename _Func, typename _Var1, typename _Var2>
class diff_wrapper<_Variable, implicit_function<std::tuple<_Func>, std::tuple<_Var1, _Var2>>>
{
public:
    using deriv_type = typename get_fundamental_zero< typename _Variable::data_type, typename _Func::data_type >::type;
};

template <typename _Variable, typename _Func, typename _Var2>
class diff_wrapper<_Variable, implicit_function<std::tuple<_Func>, std::tuple<_Variable, _Var2>>>
{
    using Rx   = typename squeezer<typename diff_wrapper<_Variable, _Func>::deriv_type>::squeezedType;
    using Ry   = typename squeezer<typename diff_wrapper<_Var2,     _Func>::deriv_type>::squeezedType;
    using RxRy = typename squeezer<negative<typename squeezer<binary_expression_wrapper<Rx, Ry, op_div>>::squeezedType>>::squeezedType;
public:
    using deriv_type = implicit_function<std::tuple<RxRy>, std::tuple<_Variable, _Var2>>;
};

template <typename _Variable, typename _Func, typename _Var1>
class diff_wrapper<_Variable, implicit_function<std::tuple<_Func>, std::tuple<_Var1, _Variable>>>
{
    using Rx   = typename squeezer<typename diff_wrapper<_Var1,     _Func>::deriv_type>::squeezedType;
    using Ry   = typename squeezer<typename diff_wrapper<_Variable, _Func>::deriv_type>::squeezedType;
    using RxRy = typename squeezer<negative<typename squeezer<binary_expression_wrapper<Rx, Ry, op_div>>::squeezedType>>::squeezedType;
public:
    using deriv_type = implicit_function<std::tuple<RxRy>, std::tuple<_Var1, _Variable>>;
};

//cross
//dot
//isotropic tensor function
//exp
//polar decomposition
//norm
//pow




//n-th derivative
template <std::size_t N, typename Variable, typename T>
class diff_wrapper_n
{
public:
    using deriv_type = typename diff_wrapper_n<N-1, Variable, typename diff_wrapper<Variable, T>::deriv_type>::deriv_type;
};

//first derivative
template <typename Variable, typename T>
class diff_wrapper_n<1, Variable, T>
{
public:
    using deriv_type = typename diff_wrapper<Variable, T>::deriv_type;
};


template <typename Expression>
class derivative_wrapper : public variable_base<derivative_wrapper<Expression>>
{
public:
    using expression = Expression;
    using data_type  = typename Expression::data_type;

    derivative_wrapper() = delete;

    derivative_wrapper(Expression const& expr)noexcept;

    template<typename _Data>
    constexpr inline decltype(auto) operator()(_Data const& __data)noexcept;

    constexpr inline std::ostream& print(std::ostream & os)const noexcept;

    Expression _data;
};

} // NAMESPACE DETAIL

#endif // DERIVATIVE_WRAPPER_BONES_H
