/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SQUEEZER_H
#define SQUEEZER_H

namespace detail {
using op_mul = tmech::detail::operator_mul;
using op_add = tmech::detail::operator_add;
using op_div = tmech::detail::operator_div;
using op_sub = tmech::detail::operator_sub;

template <typename _Expr>
class squeezer
{
public:
    using squeezedType = _Expr;
};

template <typename _LHS, typename _RHS>
struct add_real_constant{
    using type = binary_expression_wrapper<_LHS, _RHS, op_add>;
};

template <typename _T, long long int _L_LHS, long long int _R_LHS, long long int _L_RHS, long long int _R_RHS, long long int _E>
struct add_real_constant<real<_T, _L_LHS, _R_LHS, _E>, real<_T, _L_RHS, _R_RHS, _E>>
{
    using type = real<_T, _L_LHS+_L_RHS, _R_LHS+_R_RHS, _E>;
};


//real<T,L,R,E>*Expr + Expr = real<T,L,R,E>
template <typename _T, long long int _L, long long int _R, long long int _E, typename _Expr>
class squeezer<binary_expression_wrapper<binary_expression_wrapper<real<_T, _L, _R, _E>, _Expr, op_mul>, _Expr, op_add>>
{
    using _real = typename add_real_constant<real<_T, _L, _R, _E>, real<_T, 1, 0, 1>>::type;
public:
    using squeezedType = binary_expression_wrapper<_real, _Expr, op_mul>;
};

//real<T,0,0,E> * Expr --> 0
template <typename _T, long long int _E, typename _Expr>
class squeezer<binary_expression_wrapper<real<_T, 0, 0, _E>, _Expr, op_mul>>
{
    using data_type = typename _Expr::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

//Expr * real<T,0,0,E> --> 0
template <typename _T, long long int _E, typename _Expr>
class squeezer<binary_expression_wrapper<_Expr, real<_T, 0, 0, _E>, op_mul>>
{
    using data_type = typename _Expr::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

//Expr^0 --> 1
template <typename _T, long long int _E, typename _Expr>
class squeezer<diff_pow<_Expr, real<_T, 0, 0, _E>, true>>
{
    using data_type = typename _Expr::data_type;
public:
    using squeezedType = typename get_one<data_type>::type;
};

//Expr^1 --> Expr
template <typename _T, typename _Expr>
class squeezer<diff_pow<_Expr, real<_T, 1, 0, 1>, true>>
{
public:
    using squeezedType = _Expr;
};

//--Expr --> Expr
template <typename _Expr>
class squeezer<negative<negative<_Expr>>>
{
public:
    using squeezedType = _Expr;
};

//0 / Expr --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, _Expr, op_div>>
{
public:
    using squeezedType = scalar_zero<_T>;
};

//Expr + 0 --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, scalar_zero<_T>, op_add>>
{
public:
    using squeezedType = _Expr;
};

//0 + Expr --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, _Expr, op_add>>
{
public:
    using squeezedType = _Expr;
};

//0 + 0 --> 0
template <typename _T>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, scalar_zero<_T>, op_add>>
{
public:
    using squeezedType = scalar_zero<_T>;
};

//Expr + 0 --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, tensor_zero<_T>, op_add>>
{
public:
    using squeezedType = _Expr;
};

//0 + Expr --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<tensor_zero<_T>, _Expr, op_add>>
{
public:
    using squeezedType = _Expr;
};

//0 + 0 --> 0
template <typename _T>
class squeezer<binary_expression_wrapper<tensor_zero<_T>, tensor_zero<_T>, op_add>>
{
public:
    using squeezedType = tensor_zero<_T>;
};



//Expr - 0 --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, scalar_zero<_T>, op_sub>>
{
public:
    using squeezedType = _Expr;
};

//0 - Expr --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, _Expr, op_sub>>
{
public:
    using squeezedType = negative<_Expr>;
};

//0 - 0 --> 0
template <typename _T>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, scalar_zero<_T>, op_sub>>
{
public:
    using squeezedType = scalar_zero<_T>;
};

//Expr - 0 --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, tensor_zero<_T>, op_sub>>
{
public:
    using squeezedType = _Expr;
};

//0 - Expr --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<tensor_zero<_T>, _Expr, op_sub>>
{
public:
    using squeezedType = negative<_Expr>;
};

//0 - 0 --> 0
template <typename _T>
class squeezer<binary_expression_wrapper<tensor_zero<_T>, tensor_zero<_T>, op_sub>>
{
public:
    using squeezedType = tensor_zero<_T>;
};

//1 * Expr --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<scalar_one<_T>, _Expr, op_mul>>
{
public:
    using squeezedType = _Expr;
};

//Expr * 1 --> Expr
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, scalar_one<_T>, op_mul>>
{
public:
    using squeezedType = _Expr;
};

//1 * 1 --> 1
template <typename _T>
class squeezer<binary_expression_wrapper<scalar_one<_T>, scalar_one<_T>, op_mul>>
{
public:
    using squeezedType = scalar_one<_T>;
};

//Expr + Expr --> 2*Expr
template <typename _Expr>
class squeezer<binary_expression_wrapper<_Expr, _Expr, op_add>>
{
    using _data_type = typename _Expr::data_type;
    using _real = real<typename get_value_type<_data_type>::value_type, 2, 0, 1>;
public:
    using squeezedType = typename squeezer<binary_expression_wrapper<_real, _Expr, op_mul>>::squeezedType;
};

//Expr / Expr --> 1
template <typename _Expr>
class squeezer<binary_expression_wrapper<_Expr, _Expr, op_div>>
{
    using _data_type = typename _Expr::data_type;
    static_assert (is_tensor<_data_type>::value, "Tensor expression divided by a tensor expression; something is wrong here");
public:
    using squeezedType = scalar_one<_data_type>;
};


//Expr - Expr --> 0 check if expression is tensor expression
template <typename _Expr>
class squeezer<binary_expression_wrapper<_Expr, _Expr, op_sub>>
{
    using data_type = typename _Expr::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

//0 * Expr    --> 0 check if expression is tensor expression
template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, _Expr, op_mul>>
{
    using data_type = typename _Expr::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, scalar_zero<_T>, op_mul>>
{
    using data_type = typename _Expr::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<tensor_zero<_T>, _Expr, op_mul>>
{
public:
    using squeezedType = tensor_zero<_T>;
};

template <typename _Expr, typename _T>
class squeezer<binary_expression_wrapper<_Expr, tensor_zero<_T>, op_mul>>
{
public:
    using squeezedType = tensor_zero<_T>;
};


//LHS * RHS + RHS * LHS = 2*LHS*RHS
template <typename _LHS, typename _RHS>
class squeezer<binary_expression_wrapper<
        binary_expression_wrapper<_LHS, _RHS, op_mul>,
        binary_expression_wrapper<_RHS, _LHS, op_mul>,
        op_add>>
{
    using data_type = typename binary_expression_wrapper<
    binary_expression_wrapper<_LHS, _RHS, op_mul>,
    binary_expression_wrapper<_RHS, _LHS, op_mul>,
    op_add>::data_type;
    using value_type = typename get_value_type<data_type>::value_type;
public:
    using squeezedType = binary_expression_wrapper<real<value_type,2,0,1>, binary_expression_wrapper<_LHS, _RHS, op_mul>, op_mul>;
};

//sqrt(x)*sqrt(x) --> x
template <typename _Expr>
class squeezer<binary_expression_wrapper<function_wrapper<_Expr, sqrt_wrapper>, function_wrapper<_Expr, sqrt_wrapper>, op_mul>>
{
public:
    using squeezedType = _Expr;
};


//outer_product<>(0, Expr) --> 0
template <typename _Type, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class squeezer<tensor_outer_product_wrapper<tensor_zero<_Type>, _ExprRHS, _SeqLHS, _SeqRHS>>
{
    using data_type = typename tensor_outer_product_wrapper<tensor_zero<_Type>, _ExprRHS, _SeqLHS, _SeqRHS>::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

//outer_product<>(Expr, 0) --> 0
template <typename _Type, typename _ExprLHS, typename _SeqLHS, typename _SeqRHS>
class squeezer<tensor_outer_product_wrapper<_ExprLHS, tensor_zero<_Type>, _SeqLHS, _SeqRHS>>
{
    using data_type = typename tensor_outer_product_wrapper<_ExprLHS, tensor_zero<_Type>, _SeqLHS, _SeqRHS>::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

//outer_product<>(0, 0) --> 0
template <typename _TypeLHS, typename _TypeRHS, typename _SeqLHS, typename _SeqRHS>
class squeezer<tensor_outer_product_wrapper<tensor_zero<_TypeLHS>, tensor_zero<_TypeRHS>, _SeqLHS, _SeqRHS>>
{
    using data_type = typename tensor_outer_product_wrapper<tensor_zero<_TypeLHS>, tensor_zero<_TypeRHS>, _SeqLHS, _SeqRHS>::data_type;
public:
    using squeezedType = typename get_zero<data_type>::type;
};

//inner_product<<1,2><1,2>>(0.5*(otimesu(I,I) + otimesl(I,I)), I) = I
template <typename _Type, typename _T>
class squeezer<
        tensor_inner_product_wrapper<
        binary_expression_wrapper<
        real<_T,5,0,0>,
        binary_expression_wrapper<
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,3>, tmech::sequence<2,4>>,
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,4>, tmech::sequence<2,3>>,
        op_add>,
        op_mul>,
        tensor_one<_Type>,
        tmech::sequence<1,2>,
        tmech::sequence<1,2>>>
{
public:
    using squeezedType = tensor_one<_Type>;
};

//inner_product<<1,2><1,2>>(I, 0.5*(otimesu(I,I) + otimesl(I,I))) = I
template <typename _Type, typename _T>
class squeezer<
        tensor_inner_product_wrapper<
        tensor_one<_Type>,
        binary_expression_wrapper<
        real<_T,5,0,0>,
        binary_expression_wrapper<
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,3>, tmech::sequence<2,4>>,
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,4>, tmech::sequence<2,3>>,
        op_add>,
        op_mul>,
        tmech::sequence<1,2>,
        tmech::sequence<1,2>>>
{
public:
    using squeezedType = tensor_one<_Type>;
};

//inner_product<<1,2><1,2>>(0.5*(otimesu(I,I) + otimesl(I,I)), as_sym_wrapper<_type>) = as_sym_wrapper
template <typename _Type, typename _T>
class squeezer<
        tensor_inner_product_wrapper<
        binary_expression_wrapper<
        real<_T,5,0,0>,
        binary_expression_wrapper<
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,3>, tmech::sequence<2,4>>,
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,4>, tmech::sequence<2,3>>,
        op_add>,
        op_mul>,
        as_sym_wrapper<_Type>,
        tmech::sequence<1,2>,
        tmech::sequence<1,2>>>
{
public:
    using squeezedType = tensor_one<_Type>;
};

//inner_product<<1,2><1,2>>(as_sym_wrapper<_Type>, 0.5*(otimesu(I,I) + otimesl(I,I))) = as_sym_wrapper
template <typename _Type, typename _T>
class squeezer<
        tensor_inner_product_wrapper<
        as_sym_wrapper<_Type>,
        binary_expression_wrapper<
        real<_T,5,0,0>,
        binary_expression_wrapper<
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,3>, tmech::sequence<2,4>>,
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,4>, tmech::sequence<2,3>>,
        op_add>,
        op_mul>,
        tmech::sequence<1,2>,
        tmech::sequence<1,2>>>
{
public:
    using squeezedType = tensor_one<_Type>;
};


//inner_product<<1,2><1,2>>(outer<<1,3><2,4>>(I,I),X) = X
template <typename _Type, typename _ExprRHS>
class squeezer<
        tensor_inner_product_wrapper<
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,3>, tmech::sequence<2,4>>,
        _ExprRHS, tmech::sequence<1,2>, tmech::sequence<1,2>>>
{
public:
    using squeezedType = _ExprRHS;
};

//inner_product<<1,2><1,2>>(X,outer<<1,3><2,4>>(I,I)) = X
template <typename _Type, typename _ExprLHS>
class squeezer<
        tensor_inner_product_wrapper<
        _ExprLHS,
        tensor_outer_product_wrapper<tensor_one<_Type>, tensor_one<_Type>, tmech::sequence<1,3>, tmech::sequence<2,4>>,
        tmech::sequence<1,2>, tmech::sequence<1,2>>>
{
public:
    using squeezedType = _ExprLHS;
};

//inner_product<<...><..>>(X,0) = 0
template <typename _Type, typename _Expr, typename _SequenceLHS, typename _SequenceRHS>
class squeezer<
        tensor_inner_product_wrapper<
        _Expr,
        tensor_zero<_Type>,
        _SequenceLHS, _SequenceRHS>>
{
    using data_type = typename tensor_inner_product_wrapper<_Expr, tensor_zero<_Type>, _SequenceLHS, _SequenceRHS>::data_type;
public:
    using squeezedType = tensor_zero<data_type>;
};

//inner_product<<...><..>>(0,0) = 0
template <typename _Type, typename _Expr, typename _SequenceLHS, typename _SequenceRHS>
class squeezer<
        tensor_inner_product_wrapper<
        tensor_zero<_Type>,
        _Expr,
        _SequenceLHS, _SequenceRHS>>
{
    using data_type = typename tensor_inner_product_wrapper<tensor_zero<_Type>, _Expr, _SequenceLHS, _SequenceRHS>::data_type;
public:
    using squeezedType = tensor_zero<data_type>;
};

////basis_change<2,1,3,4>(otimesu(LHS, RHS)) = otimesl(RHS, LHS)
//template <typename _LHS, typename _RHS>
//class squeezer<
//        tensor_basis_change_wrapper<
//        tensor_outer_product_wrapper<_LHS, _RHS, tmech::sequence<1,3>, tmech::sequence<2,4>>,
//        tmech::sequence<2,1,3,4>>>
//{
//public:
//    using squeezedType = tensor_outer_product_wrapper<_LHS, _RHS, tmech::sequence<1,4>, tmech::sequence<2,3>>;
//};

template <typename _Expr, template<class ...> class _Wrapper, typename ..._Args>
class squeezer<_Wrapper<negative<_Expr>, _Args...>>
{
public:
    using squeezedType = negative<_Wrapper<_Expr, _Args...>>;
};

// negative(LHS) + 0 --> negative(LHS)
template <typename _LHS, typename _T>
class squeezer<binary_expression_wrapper<negative<_LHS>, scalar_zero<_T>, op_add>>
{
public:
    using squeezedType = negative<_LHS>;
};

// 0 + negative(RHS) --> negative(RHS)
template <typename _T, typename _RHS>
class squeezer<binary_expression_wrapper<scalar_zero<_T>, negative<_RHS>, op_add>>
{
public:
    using squeezedType = negative<_RHS>;
};

// negative(LHS) + RHS--> RHS - LHS
template <typename _LHS, typename _RHS>
class squeezer<binary_expression_wrapper<negative<_LHS>, _RHS, op_add>>
{
public:
    using squeezedType = binary_expression_wrapper<_RHS, _LHS, op_sub>;
};


// LHS + negative(RHS) --> LHS - RHS
template <typename _LHS, typename _RHS>
class squeezer<binary_expression_wrapper<_LHS, negative<_RHS>, op_add>>
{
public:
    using squeezedType = binary_expression_wrapper<_LHS, _RHS, op_sub>;
};

// LHS * negative(RHS) --> LHS - RHS
template <typename _LHS, typename _RHS>
class squeezer<binary_expression_wrapper<_LHS, negative<_RHS>, op_mul>>
{
public:
    using squeezedType = negative<binary_expression_wrapper<_LHS, _RHS, op_mul>>;
};

// negative(LHS) * RHS --> LHS - RHS
template <typename _LHS, typename _RHS>
class squeezer<binary_expression_wrapper<negative<_LHS>, _RHS, op_mul>>
{
public:
    using squeezedType = negative<binary_expression_wrapper<_LHS, _RHS, op_mul>>;
};
}
#endif // SQUEEZER_H
