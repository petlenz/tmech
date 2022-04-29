/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef UTILITY_H
#define UTILITY_H


template <typename _T>
struct is_tensor_type
{
    using type = std::false_type;
    static constexpr bool value = false;
};

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct is_tensor_type<tensor<_T, _Dim, _Rank>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _T>
struct is_tensor_type<detail::abs_tensor_wrapper<_T>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _LHS, typename _RHS, typename _OP>
struct is_tensor_type<detail::tensor_binary_expression_wrapper<_LHS, _RHS, _OP>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor, typename _Sequence>
struct is_tensor_type<detail::basis_change_wrapper<_Tensor, _Sequence>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Father>
struct is_tensor_type<detail::sign_S_wrapper<_Father>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Father>
struct is_tensor_type<detail::sign_N_wrapper<_Father>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct is_tensor_type<randn<_T, _Dim, _Rank>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct is_tensor_type<randu<_T, _Dim, _Rank>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::adjoint_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor, typename ..._Sequences>
struct is_tensor_type<detail::inverse_wrapper<_Tensor, _Sequences...>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
struct is_tensor_type<detail::inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename T, std::size_t Dim>
struct is_tensor_type<levi_civita<T, Dim>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::eval_tensor_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
struct is_tensor_type<detail::outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::symmetric_part_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename T, std::size_t Dim, std::size_t Rank>
struct is_tensor_type<eye<T, Dim, Rank>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename T, std::size_t Dim, std::size_t Rank>
struct is_tensor_type<zeros<T, Dim, Rank>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename T, std::size_t Dim, std::size_t Rank, typename _Type>
struct is_tensor_type<adaptor<T, Dim, Rank, _Type>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::cofactor_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template<typename _LHS, typename _RHS>
struct is_tensor_type<detail::cross_product_wrapper<_LHS, _RHS>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::skew_symmetric_part_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::deviatoric_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::volumetric_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor, typename _Function>
struct is_tensor_type<detail::isotropic_tensor_function<_Tensor, _Function>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::polar_decomposition_R_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::polar_decomposition_U_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::polar_decomposition_V_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::polar_decomposition_dR_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::polar_decomposition_dU_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::polar_decomposition_dV_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::exp_tensor_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Father, typename _Tensor>
struct is_tensor_type<detail::exp_derivative_tensor_wrapper<_Father, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::positive_negative_decomposition_neg_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::positive_negative_decomposition_pos_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::positive_negative_decomposition_dneg_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::positive_negative_decomposition_dpos_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::negative_tensor_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Tensor>
struct is_tensor_type<detail::pow_tensor_wrapper<_Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};

template <typename _Base, typename _Tensor>
struct is_tensor_type<detail::pow_tensor_derivative_wrapper<_Base, _Tensor>>
{
    using type = std::true_type;
    static constexpr bool value = true;
};


// This is the type which holds sequences
template <std::size_t ... _Idx> struct sequence {
    static constexpr size_t size() noexcept { return sizeof...(_Idx); }
};



namespace detail {

template <typename T, std::size_t Size, bool Enable>
struct get_tensor_array_type;

template <typename T, std::size_t Size>
struct get_tensor_array_type<T, Size, false>
{
    using data_type = std::array<T, Size>;
};

template <typename T, std::size_t Size>
struct get_tensor_array_type<T, Size, true>
{
    using data_type = std::vector<T>;
};


template <typename T>
struct get_complex_entry_type
{
    using type = T;
};

template <typename T>
struct get_complex_entry_type<std::complex<T>>
{
    using type = T;
};

template <std::size_t DIM, std::size_t RANK, std::size_t Start=1>
struct get_tensor_size{
    static_assert (RANK != 0, "tensor");
    static constexpr std::size_t size = DIM * get_tensor_size<DIM, RANK, Start+1>::size;
};

template <std::size_t DIM, std::size_t RANK>
struct get_tensor_size<DIM, RANK, RANK>{
    static constexpr std::size_t size = DIM;
};


template<typename Type>
using has_raw_data = decltype(std::declval<Type>().raw_data());

template<typename Type, typename... Arguments>
using has_evaluate = decltype(std::declval<Type>().evaluate(std::declval<Arguments>()...));


//numerical differentiation
template <typename Function, typename Direction, bool Fundamental>
struct get_rank_result
{
    static constexpr std::size_t rank{Function::data_type::rank() + Direction::rank()};
};

template <typename Function, typename Direction>
struct get_rank_result<Function, Direction, true>
{
    static constexpr std::size_t rank{Direction::rank()};
};



// First define the template signature
template <std::size_t ... Ns> struct seq_gen;

// Recursion case
// End Start
template <std::size_t End, std::size_t Start, std::size_t ... Ns>
struct seq_gen<Start, End, Ns...>
{
    // Take front most number of sequence,
    // decrement it, and prepend it twice.
    using type = typename seq_gen<
        Start, End - 1, End - 1, Ns...>::type;
};

// Recursion abort
template <std::size_t Start, std::size_t ... Ns>
struct seq_gen<Start, Start, Ns...>
{
    using type = sequence<Ns...>;
};

template <std::size_t End, std::size_t Start=0>
using sequence_t = typename seq_gen<Start, End+1>::type;


// First define the template signature
template <std::size_t ... Ns> struct seq_gen_transpose;

// Recursion case
// End Start
template <std::size_t End, std::size_t Start, std::size_t ... Ns>
struct seq_gen_transpose<Start, End, Ns...>
{
    // Take front most number of sequence,
    // decrement it, and prepend it twice.
    using type = typename seq_gen_transpose<
        Start, End - 2, End, End - 1, Ns...>::type;
};

template <std::size_t ... Ns>
struct seq_gen_transpose<0, 1, Ns...>
{
    using type = sequence<1,0, Ns...>;
};

template <std::size_t End, std::size_t Start=0>
using sequence_transpose = typename seq_gen_transpose<Start, End-1>::type;

template <typename F, typename TUP, std::size_t ... INDICES>
static constexpr inline auto & tuple_call(F & f, TUP && tup, sequence<INDICES...>){
    return f(std::get<INDICES>(tup)...);
}

template <typename F, typename TUP, std::size_t ... INDICES>
static constexpr inline auto tuple_call(F const& f, TUP && tup, sequence<INDICES...>){
    return f(std::get<INDICES>(tup)...);
}

template<std::size_t ...Numbers, std::size_t ...Ns>
static constexpr inline auto make_single_sequence_detail(sequence<Ns...>){
    return sequence<Numbers..., Ns...>();
}

template<std::size_t ...Number, std::size_t ...Ns, typename ...Args>
static constexpr inline auto make_single_sequence_detail(sequence<Ns...>, Args... args){
    return make_single_sequence_detail<Number..., Ns...>(args...);
}

template<typename ...Args>
static constexpr inline auto make_single_sequence(Args... args){
    return make_single_sequence_detail(args...);
}


//template<std::size_t ...NumbersResult>
//static constexpr inline auto make_single_boxtimes_sequence_detail(sequence<NumbersResult...>, sequence<> , sequence<>){
//    return sequence<NumbersResult...>();
//}

//template<std::size_t ...NumbersResult, std::size_t FirstRHS, std::size_t LastRHS>
//static constexpr inline auto make_single_boxtimes_sequence_detail(sequence<NumbersResult...>, sequence<>, sequence<FirstRHS, LastRHS>){
//    return sequence<NumbersResult..., FirstRHS, LastRHS>();
//}

//template<std::size_t ...NumbersResult, std::size_t FirstLHS, std::size_t LastLHS, std::size_t FirstRHS, std::size_t LastRHS, std::size_t ...NumbersLHS, std::size_t ...NumbersRHS>
//static constexpr inline auto make_single_boxtimes_sequence_detail(sequence<NumbersResult...>, sequence<FirstLHS, LastLHS, NumbersLHS...>, sequence<FirstRHS, LastRHS, NumbersRHS...>){
//    return make_single_boxtimes_sequence_detail(sequence<NumbersResult..., FirstLHS, FirstRHS, LastLHS, LastRHS>(), sequence<NumbersLHS...>(), sequence<NumbersRHS...>());
//}


//template<std::size_t ...NumbersLHS, std::size_t ...NumbersRHS>
//static constexpr inline auto make_single_boxtimes_sequence(sequence<NumbersLHS...> , sequence<NumbersRHS...>){
//    return make_single_boxtimes_sequence_detail(sequence<>(), sequence<NumbersLHS...>(), sequence<NumbersRHS...>());
//}


template<std::size_t Iter, std::size_t End>
struct get_sequence_detail
{
    template<std::size_t ...NumbersResult, std::size_t Next,  std::size_t ...Numbers>
    static constexpr inline auto apply(sequence<NumbersResult...>, sequence<Next, Numbers...>){
        return get_sequence_detail<Iter+1, End>::apply(sequence<NumbersResult..., Next>(),  sequence<Numbers...>());
    }

};

template<std::size_t End>
struct get_sequence_detail<End, End>
{
    template<std::size_t ...NumbersResult, std::size_t ...Numbers>
    static constexpr inline auto apply(sequence<NumbersResult...>, sequence<Numbers...>){
        return sequence<NumbersResult...>();
    }
};

template<std::size_t Iter, std::size_t Start, std::size_t End>
struct get_sequence_iter_start_detail
{
    template<std::size_t Next,  std::size_t ...Numbers>
    static constexpr inline auto apply(sequence<Next, Numbers...>){
        return get_sequence_iter_start_detail<Iter+1, Start, End>::apply(sequence<Numbers...>());
    }

};

template<std::size_t Start, std::size_t End>
struct get_sequence_iter_start_detail<Start, Start, End>
{
    template<std::size_t ...Numbers>
    static constexpr inline auto apply(sequence<Numbers...>){
        return get_sequence_detail<Start, End>::apply(sequence<>(), sequence<Numbers...>());
    }
};

template<std::size_t Start, std::size_t End, std::size_t ...Numbers>
static constexpr inline auto get_sequence(sequence<Numbers...>){
    return get_sequence_iter_start_detail<0, Start, End>::apply(sequence<Numbers...>());
}



template<std::size_t...Numbers>
void print_sequence(sequence<Numbers...>){
    ((std::cout<<Numbers<<" "),...)<<std::endl;
}

void print_sequence(sequence<>){

}

template<std::size_t Size, std::size_t First, std::size_t ...Numbers>
struct get_tensor_array_size
{
    static constexpr inline std::size_t size(){
        return First * get_tensor_array_size<Size-1, Numbers...>::size();
    }
};

template<std::size_t Last>
struct get_tensor_array_size<0, Last>
{
    static constexpr inline std::size_t size(){
        return Last;
    }
};

template<std::size_t Dim, std::size_t Size, std::size_t Index>
struct get_tensor_index
{
    static constexpr inline auto get(){
        return Dim*get_tensor_index<Dim,Size,Index+1>::get();
    }
};

template<std::size_t Dim, std::size_t Index>
struct get_tensor_index<Dim, Index, Index>
{
    static constexpr inline auto get(){
        return Dim;
    }
};

template<std::size_t Dim, std::size_t Size, std::size_t Index>
struct get_tensor_array_index{

    template<typename ...Indicies>
    static constexpr inline std::size_t index(std::tuple<Indicies...> const&  indicies){
        if constexpr(Index < Size-1){
            //std::cout<<"Index "<<get_tensor_index<Dim, Size-2, Index>::get()<<" "<<Size-1<<" "<<Index<<std::endl;
            return get_tensor_index<Dim, Size-2, Index>::get()*std::get<Index>(indicies) + get_tensor_array_index<Dim, Size, Index+1>::index(indicies);
        }//4-1=3 4-2=2 4-3=1
        return std::get<Index>(indicies);
    }
};

template <std::size_t Counter, typename Sequence, std::size_t ValueToCheck>
struct counter_index_sequence_h
{
    static constexpr auto value = Counter;
};

template <std::size_t Counter, std::size_t First, std::size_t ...Args, std::size_t ValueToCheck>
struct counter_index_sequence_h<Counter, sequence<First, Args...>, ValueToCheck>
        : public counter_index_sequence_h<Counter, sequence<Args...>, ValueToCheck>
{};

template <std::size_t Counter, std::size_t ...Args, std::size_t ValueToCheck>
struct counter_index_sequence_h<Counter, sequence<ValueToCheck, Args...>, ValueToCheck>
        : public counter_index_sequence_h<Counter+1, sequence<Args...>, ValueToCheck>
{};

template <typename Sequence, std::size_t ValueToCheck>
struct counter_index_sequence
        : public counter_index_sequence_h<0, Sequence, ValueToCheck>
{};

template <typename Sequence, std::size_t ValueToCheck>
constexpr auto counter_index_sequence_v = counter_index_sequence<Sequence, ValueToCheck>::value;



template <bool Result, typename Sequence, std::size_t ValueToCheck>
struct find_in_sequence_h
{
    static constexpr auto value = Result;
};

template <bool Result, std::size_t First, std::size_t ...Args, std::size_t ValueToCheck>
struct find_in_sequence_h<Result, sequence<First, Args...>, ValueToCheck>
        : public find_in_sequence_h<false, sequence<Args...>, ValueToCheck>
{};

template <bool Result, std::size_t ...Args, std::size_t ValueToCheck>
struct find_in_sequence_h<Result, sequence<ValueToCheck, Args...>, ValueToCheck>
        : public find_in_sequence_h<true, sequence<>, ValueToCheck>
{};


template <typename Sequence, std::size_t ValueToCheck>
struct find_in_sequence;

template <std::size_t First, std::size_t ...Args, std::size_t ValueToCheck>
struct find_in_sequence<sequence<First, Args...>, ValueToCheck>
        : public find_in_sequence_h<false, sequence<Args...>, ValueToCheck>
{};

template <std::size_t ...Args, std::size_t ValueToCheck>
struct find_in_sequence<sequence<ValueToCheck, Args...>, ValueToCheck>
        : public find_in_sequence_h<true, sequence<>, ValueToCheck>
{};

template <typename Sequence, std::size_t ValueToCheck>
struct check_duplicated_value_in_sequence{
    static constexpr bool value = (counter_index_sequence_v<Sequence, ValueToCheck> > 1);
};

template <typename Sequence, std::size_t ValueToCheck>
constexpr auto check_duplicated_value_in_sequence_v = check_duplicated_value_in_sequence<Sequence, ValueToCheck>::value;




template <bool Result, typename SequenceInput, typename Sequence>
struct check_duplicated_in_sequence_h{
    static constexpr bool value = Result;
};

template <std::size_t First, std::size_t ...ArgsInput, typename Sequence>
struct check_duplicated_in_sequence_h<false, sequence<First, ArgsInput...>, Sequence>
        : public check_duplicated_in_sequence_h<check_duplicated_value_in_sequence_v<Sequence, First>, sequence<ArgsInput...>, Sequence>
{};


template <typename Sequence>
struct check_duplicated_in_sequence;

template <std::size_t First, std::size_t ...ArgsInput>
struct check_duplicated_in_sequence<sequence<First, ArgsInput...>>
        : public check_duplicated_in_sequence_h<check_duplicated_value_in_sequence_v<sequence<First, ArgsInput...>, First>, sequence<ArgsInput...>, sequence<First, ArgsInput...>>
{};

template <typename Sequence>
constexpr auto check_duplicated_in_sequence_v = check_duplicated_in_sequence<Sequence>::value;


template <std::size_t Index, std::size_t ...ArgsInner>
struct make_outer_sequence_find;

template <std::size_t Index, std::size_t First, std::size_t ...ArgsInner>
struct make_outer_sequence_find<Index, First, ArgsInner...>
{
    static constexpr bool value = make_outer_sequence_find<Index, ArgsInner...>::value;
};

//Index is inside ArgsInner
template <std::size_t Index, std::size_t ...ArgsInner>
struct make_outer_sequence_find<Index, Index, ArgsInner...>
{
    static constexpr bool value = true;
};

template <std::size_t Index>
struct make_outer_sequence_find<Index, Index>
{
    static constexpr bool value = true;
};


//Index is not inside ArgsInner
template <std::size_t Index, std::size_t Last>
struct make_outer_sequence_find<Index, Last>
{
    static constexpr bool value = false;
};


template <bool Type, std::size_t Size, std::size_t Index, typename Inner, std::size_t ...ArgsOuter>
struct make_outer_sequence;

//Index is not inside ArgsInner
template <std::size_t Size, std::size_t Index, std::size_t ...ArgsInner, std::size_t ...ArgsOuter>
struct make_outer_sequence<false, Size, Index, sequence<ArgsInner...>, ArgsOuter...>
{
    using type = typename make_outer_sequence<make_outer_sequence_find<Index+1, ArgsInner...>::value,
    Size, Index+1, sequence<ArgsInner...>, ArgsOuter..., Index>::type;

    using sequence_outer = sequence<ArgsOuter..., Index>;
};

//Index is inside ArgsInner
template <std::size_t Size, std::size_t Index, std::size_t ...ArgsInner, std::size_t ...ArgsOuter>
struct make_outer_sequence<true, Size, Index, sequence<ArgsInner...>, ArgsOuter...>
{
    using type = typename make_outer_sequence<make_outer_sequence_find<Index+1, ArgsInner...>::value,
    Size, Index+1, sequence<ArgsInner...>, ArgsOuter...>::type;

    using sequence_outer = sequence<ArgsOuter...>;
};

//Index is not inside ArgsInner
template <std::size_t Size, std::size_t ...ArgsInner, std::size_t ...ArgsOuter>
struct make_outer_sequence<false, Size, Size, sequence<ArgsInner...>, ArgsOuter...>
{
    using type = make_outer_sequence<false, Size, Size, sequence<ArgsInner...>, ArgsOuter...>;
    using sequence_outer = sequence<ArgsOuter...>;
};


template <std::size_t Size, std::size_t Index, typename Inner>
struct make_outer_sequence_test;

template <std::size_t Size, std::size_t Index, std::size_t ...ArgsInner>
struct make_outer_sequence_test<Size, Index, sequence<ArgsInner...>>
{
    using type = typename make_outer_sequence<make_outer_sequence_find<Index, ArgsInner...>::value,
    Size, Index, sequence<ArgsInner...>>::type;
};

template<std::size_t Rank, typename SequenceInner>
using sequence_outer_t = typename make_outer_sequence_test<Rank, 1, SequenceInner>::type::sequence_outer;



template <typename Result, typename Input1, typename Input2>
struct set_difference_sequence_h
{
    using type = Result;
};


template <std::size_t ...Result, std::size_t FirstInput1, std::size_t ...ArgsInput1, std::size_t FirstInput2, std::size_t ...ArgsInput2>
struct set_difference_sequence_h<sequence<Result...>, sequence<FirstInput1, ArgsInput1...>, sequence<FirstInput2, ArgsInput2...>>
        : public set_difference_sequence_h<sequence<Result..., FirstInput1>, sequence<ArgsInput1...>, sequence<FirstInput2, ArgsInput2...>>
{};

template <std::size_t ...Result, std::size_t FirstInput, std::size_t ...ArgsInput1, std::size_t ...ArgsInput2>
struct set_difference_sequence_h<sequence<Result...>, sequence<FirstInput, ArgsInput1...>, sequence<FirstInput, ArgsInput2...>>
        : public set_difference_sequence_h<sequence<Result...>, sequence<ArgsInput1...>, sequence<ArgsInput2...>>
{};

template <std::size_t ...Result, std::size_t ...ArgsInput1>
struct set_difference_sequence_h<sequence<Result...>, sequence<ArgsInput1...>, sequence<>>
        : public set_difference_sequence_h<sequence<Result..., ArgsInput1...>, void, void>
{};

template <typename Input1, typename Input2>
struct set_difference_sequence
        : public set_difference_sequence_h<sequence<>, Input1, Input2>
{};

template <typename Input1, typename Input2>
using set_difference_sequence_t = typename set_difference_sequence<Input1, Input2>::type;




template <typename Result, typename Input, std::size_t IDX, std::size_t Position, std::size_t Value>
struct change_value_at_position_h
{
    using type = Result;
};

template <std::size_t ...Result, std::size_t First, std::size_t ...Args, std::size_t IDX, std::size_t Positions, std::size_t Value>
struct change_value_at_position_h<sequence<Result...>, sequence<First, Args...>, IDX, Positions, Value>
        : public change_value_at_position_h<sequence<Result..., First>, sequence<Args...>, IDX+1, Positions, Value>
{};

template <std::size_t ...Result, std::size_t First, std::size_t ...Args, std::size_t Positions, std::size_t Value>
struct change_value_at_position_h<sequence<Result...>, sequence<First, Args...>, Positions, Positions, Value>
        : public change_value_at_position_h<sequence<Result..., Value, Args...>, sequence<>, Positions, Positions, Value>
{};

template <typename Input, std::size_t Position, std::size_t Value>
struct change_value_at_position
        : public change_value_at_position_h<sequence<>, Input, 0, Position, Value>
{};

template <typename Input, std::size_t Position, std::size_t Value>
using change_value_at_position_t = typename change_value_at_position<Input, Position, Value>::type;



template <typename Input, typename Position, typename Value>
struct change_values_at_positions
{
    using type = Input;
};

template <typename Input, std::size_t FirstPos, std::size_t ...Position, std::size_t FirstValue, std::size_t ...Value>
struct change_values_at_positions<Input, sequence<FirstPos, Position...>, sequence<FirstValue, Value...>>
        : public change_values_at_positions<change_value_at_position_t<Input, FirstPos, FirstValue>, sequence<Position...>, sequence<Value...>>
{};

template <typename Input, typename Position, typename Value>
using change_values_at_positions_t = typename change_values_at_positions<Input, Position, Value>::type;


template <typename Result, typename Input, std::size_t Value>
struct add_value_sequence_h
{
    using type = Result;
};

template <std::size_t ...ArgsResult, std::size_t FirstInput, std::size_t ...ArgsInput, std::size_t Value>
struct add_value_sequence_h<sequence<ArgsResult...>, sequence<FirstInput, ArgsInput...>, Value>
        : public add_value_sequence_h<sequence<ArgsResult..., FirstInput + Value>, sequence<ArgsInput...>, Value>
{};

template <typename T, std::size_t Value>
struct add_value_sequence;

template <std::size_t ...Args, std::size_t Value>
struct add_value_sequence<sequence<Args...>, Value>
        : public add_value_sequence_h<sequence<>, sequence<Args...>, Value>
{};

template <typename T, std::size_t Value>
using add_value_sequence_t = typename add_value_sequence<T, Value>::type;


template <typename Result, typename Input, std::size_t Value>
struct min_value_squence_h
{
    using type = Result;
};

template <std::size_t ...ArgsResult, std::size_t FirstInput, std::size_t ...ArgsInput, std::size_t Value>
struct min_value_squence_h<sequence<ArgsResult...>, sequence<FirstInput, ArgsInput...>, Value>
        : public min_value_squence_h<sequence<ArgsResult..., FirstInput - Value>, sequence<ArgsInput...>, Value>
{};


template <typename T, std::size_t Value>
struct min_value_squence;


template <std::size_t ...Args, std::size_t Value>
struct min_value_squence<sequence<Args...>, Value>
        : public min_value_squence_h<sequence<>, sequence<Args...>, Value>
{};

template <typename T, std::size_t Value>
using min_value_squence_t = typename min_value_squence<typename std::remove_reference<T>::type, Value>::type;



template <typename Result, typename Input, std::size_t Arg1, std::size_t Arg2>
struct swape_element_squence_h
{
    using type = Result;
};

template <std::size_t ...ArgsResult, std::size_t FirstInput, std::size_t ...ArgsInput, std::size_t Arg1, std::size_t Arg2>
struct swape_element_squence_h<sequence<ArgsResult...>, sequence<FirstInput, ArgsInput...>, Arg1, Arg2>
        : public swape_element_squence_h<sequence<ArgsResult..., FirstInput>, sequence<ArgsInput...>, Arg1, Arg2>
{};

template <std::size_t ...ArgsResult, std::size_t FirstInput, std::size_t ...ArgsInput, std::size_t Arg2>
struct swape_element_squence_h<sequence<ArgsResult...>, sequence<FirstInput, ArgsInput...>, FirstInput, Arg2>
        : public swape_element_squence_h<sequence<ArgsResult..., Arg2>, sequence<ArgsInput...>, FirstInput, Arg2>
{};

//template <std::size_t ...ArgsResult, std::size_t FirstInput, std::size_t ...ArgsInput, std::size_t Arg1>
//struct swape_element_squence_h<squence<ArgsResult...>, squence<FirstInput, ArgsInput...>, Arg1, FirstInput>
//        : public swape_element_squence_h<squence<ArgsResult..., Arg1>, squence<ArgsInput...>, Arg1, FirstInput>
//{};

template <std::size_t ...ArgsResult, std::size_t FirstInput, std::size_t ...ArgsInput>
struct swape_element_squence_h<sequence<ArgsResult...>, sequence<FirstInput, ArgsInput...>, FirstInput, FirstInput>
        : public swape_element_squence_h<sequence<ArgsResult..., FirstInput>, sequence<ArgsInput...>, FirstInput, FirstInput>
{};


template <typename Input, std::size_t Arg1, std::size_t Arg2>
struct swape_element_squence;

template <std::size_t ...ArgsInput, std::size_t Arg1, std::size_t Arg2>
struct swape_element_squence<sequence<ArgsInput...>, Arg1, Arg2>
        : public swape_element_squence_h<sequence<>, sequence<ArgsInput...>, Arg1, Arg2>
{};

template <typename Input, std::size_t Arg1, std::size_t Arg2>
using swape_element_squence_t = typename swape_element_squence<Input, Arg1, Arg2>::type;




template <typename Result, typename Input, typename Input1, typename Input2>
struct swape_elements_squence_h
{
    using type = Result;
};

template <std::size_t ...ArgsInput, std::size_t FirstInput1, std::size_t ...ArgsInput1, std::size_t FirstInput2, std::size_t ...ArgsInput2>
struct swape_elements_squence_h<sequence<>, sequence<ArgsInput...>, sequence<FirstInput1, ArgsInput1...>, sequence<FirstInput2, ArgsInput2...>>
        : public swape_elements_squence_h<swape_element_squence_t<sequence<ArgsInput...>, FirstInput1, FirstInput2>, sequence<ArgsInput...>, sequence<ArgsInput1...>, sequence<ArgsInput2...>>
{};

template <std::size_t ...Result, std::size_t ...ArgsInput, std::size_t FirstInput1, std::size_t ...ArgsInput1, std::size_t FirstInput2, std::size_t ...ArgsInput2>
struct swape_elements_squence_h<sequence<Result...>, sequence<ArgsInput...>, sequence<FirstInput1, ArgsInput1...>, sequence<FirstInput2, ArgsInput2...>>
        : public swape_elements_squence_h<swape_element_squence_t<sequence<Result...>, FirstInput1, FirstInput2>, sequence<ArgsInput...>, sequence<ArgsInput1...>, sequence<ArgsInput2...>>
{};

//nothing to swapping
template < std::size_t ...ArgsInput>
struct swape_elements_squence_h<sequence<>, sequence<ArgsInput...>, sequence<>, sequence<>>
        : public swape_elements_squence_h<sequence<ArgsInput...>, void, void, void>
{};


template <typename Input, typename Input1, typename Input2>
struct swape_elements_squence;

template <std::size_t ...Input, std::size_t ...Input1, std::size_t ...Input2>
struct swape_elements_squence<sequence<Input...>, sequence<Input1...>, sequence<Input2...>>
        : public swape_elements_squence_h<sequence<>, sequence<Input...>, sequence<Input1...>, sequence<Input2...>>
{};

template <typename Input, typename Input1, typename Input2>
using swape_elements_squence_t = typename swape_elements_squence<Input, Input1, Input2>::type;


template <typename Result, typename Sequence>
struct reverse_sequence_h
{
    using type = Result;
};

template <std::size_t First, std::size_t ...Args>
struct reverse_sequence_h<void, sequence<First, Args...>>
        : public reverse_sequence_h<sequence<First>, sequence<Args...>>
{};

template <std::size_t ...Numbers, std::size_t First, std::size_t ...Args>
struct reverse_sequence_h<sequence<Numbers...>, sequence<First, Args...>>
        : public reverse_sequence_h<sequence<First, Numbers...>, sequence<Args...>>
{};

template <typename Sequence>
struct reverse_sequence
        : public reverse_sequence_h<void,Sequence>
{};

template <typename Sequence>
using reverse_sequence_t = typename reverse_sequence<Sequence>::type;



template <typename _Result, typename _Sequence>
struct append_sequence_end_h;

template <std::size_t ...Numbers>
struct append_sequence_end_h<sequence<Numbers...>, sequence<>>
{
    using type = sequence<Numbers...>;
};

template <std::size_t ...Numbers, std::size_t First, std::size_t ...Args>
struct append_sequence_end_h<sequence<Numbers...>, sequence<First, Args...>>
        : public append_sequence_end_h<sequence<Numbers..., First>, sequence<Args...>>
{};

template <std::size_t First, std::size_t ...Args>
struct append_sequence_end_h<sequence<>, sequence<First, Args...>>
        : public append_sequence_end_h<sequence<First>, sequence<Args...>>
{};

template <typename _SeqFirst, typename _SeqSecond>
struct append_sequence_end
        : public append_sequence_end_h<_SeqFirst, _SeqSecond>
{};

template <typename _SeqFirst, typename _SeqSecond>
using append_sequence_end_t = typename append_sequence_end<_SeqFirst, _SeqSecond>::type;

template <std::size_t _First, std::size_t _Second, bool>
struct bubble_sort_sequence_swap_h
{
    static constexpr auto first  = _First;
    static constexpr auto second = _Second;
    static constexpr bool sorted  = false;
};

template <std::size_t _First, std::size_t _Second>
struct bubble_sort_sequence_swap_h<_First, _Second, false>
{
    static constexpr auto first  = _Second;
    static constexpr auto second = _First;
    static constexpr bool sorted  = true;
};

template <std::size_t _First, std::size_t _Second>
struct bubble_sort_sequence_swap
        : public bubble_sort_sequence_swap_h<_First, _Second, _First < _Second>
{};


template <typename _Result, typename _Input, bool Sorted>
struct bubble_sort_sequence_h
{
    using type = _Result;
};

template <std::size_t ...Results, std::size_t _First, std::size_t _Second, std::size_t ...Args>
struct bubble_sort_sequence_h<tmech::sequence<Results...>, tmech::sequence<_First, _Second, Args...>, false>
        : public bubble_sort_sequence_h<
        tmech::sequence<
        Results...,
        bubble_sort_sequence_swap<_First, _Second>::first>,
        tmech::sequence<bubble_sort_sequence_swap<_First, _Second>::second, Args...>,
        bubble_sort_sequence_swap<_First, _Second>::sorted>
{};

//last two components
template <std::size_t ...Results, std::size_t _First, std::size_t _Second>
struct bubble_sort_sequence_h<tmech::sequence<Results...>, tmech::sequence<_First, _Second>, false>
        : public bubble_sort_sequence_h<
        tmech::sequence<Results...,
        bubble_sort_sequence_swap<_First, _Second>::first,
        bubble_sort_sequence_swap<_First, _Second>::second>,
        tmech::sequence<>,
        false>
{};

//only one component
template <std::size_t ...Results, std::size_t _First>
struct bubble_sort_sequence_h<tmech::sequence<Results...>, tmech::sequence<_First>, false>
        : public bubble_sort_sequence_h<
        tmech::sequence<Results...,_First>,
        tmech::sequence<>,
        false>
{};

template <std::size_t ...Results, std::size_t _First, std::size_t _Second, std::size_t ...Args>
struct bubble_sort_sequence_h<tmech::sequence<Results...>, tmech::sequence<_First, _Second, Args...>, true>
        : public bubble_sort_sequence_h<
        tmech::sequence<
        Results...,
        bubble_sort_sequence_swap<_First, _Second>::first>,
        tmech::sequence<bubble_sort_sequence_swap<_First, _Second>::second, Args...>,
        true>
{};

template <std::size_t ...Results, std::size_t _First, std::size_t _Second>
struct bubble_sort_sequence_h<tmech::sequence<Results...>, tmech::sequence<_First, _Second>, true>
        : public bubble_sort_sequence_h<
        tmech::sequence<>,
        tmech::sequence<Results...,
        bubble_sort_sequence_swap<_First, _Second>::first,
        bubble_sort_sequence_swap<_First, _Second>::second>,
        false>
{};

template <typename _Input>
struct bubble_sort_sequence
        : public bubble_sort_sequence_h<tmech::sequence<>, _Input, false>
{};

template <typename _Input>
using bubble_sort_sequence_t = typename bubble_sort_sequence<_Input>::type;

template <typename Function, std::size_t End, std::size_t Start=0>
class meta_for_loop
{
public:
    template<typename Func, typename ...Indicies>
    static constexpr inline auto loop(Func const& func, const Indicies &... indicies) {
        if constexpr (Start < End){
            Function::loop(func, indicies..., Start);
            meta_for_loop<Function, End, Start+1>::loop(func, indicies...);
        }
    }
};

template <std::size_t End, std::size_t Start>
class meta_for_loop<void, End, Start>
{
public:
    template<typename Func, typename ...Indicies>
    static constexpr inline auto loop(Func const& func, const  Indicies &... indicies) {
        if constexpr (Start < End){
            func(indicies...,Start);
            meta_for_loop<void, End, Start+1>::loop(func, indicies...);
        }
    }
};

template <std::size_t DIM, std::size_t DEEP, std::size_t Start=0>
struct meta_for_loop_deep
{
    using type = meta_for_loop<typename meta_for_loop_deep<DIM, DEEP, Start+1>::type, DIM>;
};

template <std::size_t DIM, std::size_t DEEP>
struct meta_for_loop_deep<DIM, DEEP, DEEP>
{
    using type = meta_for_loop<void, DIM>;
};



template <typename Function, std::size_t End, std::size_t Start=0>
class meta_for_loop_symmetric;

template <typename Function, std::size_t End, std::size_t Start=0>
struct set_up_new_loop
{
    using type = Function;
};

template <typename Function, std::size_t FuncEnd, std::size_t FuncStart, std::size_t LoopEnd, std::size_t LoopStart>
struct set_up_new_loop<meta_for_loop_symmetric<Function, FuncEnd, FuncStart>, LoopEnd, LoopStart>
{
    using type = meta_for_loop_symmetric<Function, FuncEnd, LoopStart>;
};


template <typename Function, std::size_t End, std::size_t Start>
class meta_for_loop_symmetric
{
public:
    using newFunction = typename set_up_new_loop<Function, End, Start>::type;

    template<typename Func, typename ...Indicies>
    static constexpr inline auto loop(Func const& func, const Indicies &... indicies) {
        if constexpr (Start < End){
            newFunction::loop(func, indicies..., Start);
            meta_for_loop_symmetric<newFunction, End, Start+1>::loop(func, indicies...);
        }
    }
};

template <std::size_t End, std::size_t Start>
class meta_for_loop_symmetric<void, End, Start>
{
public:
    template<typename Func, typename ...Indicies>
    static constexpr inline auto loop(Func const& func, const Indicies &... indicies) {
        if constexpr (Start < End){
            func(indicies..., Start);
            meta_for_loop_symmetric<void, End, Start+1>::loop(func, indicies...);
        }
    }
};


template <std::size_t DIM, typename _Func = void>
struct meta_for_loop_deep_symmetric_pair
        : public meta_for_loop_symmetric<meta_for_loop_symmetric<_Func, DIM>, DIM>
{};


template <std::size_t DIM, std::size_t DEEP, std::size_t Start=0>
struct meta_for_loop_deep_symmetric
{
    using type = meta_for_loop_deep_symmetric_pair<DIM,typename meta_for_loop_deep_symmetric<DIM, DEEP, Start+1>::type>;
};

template <std::size_t DIM, std::size_t DEEP>
struct meta_for_loop_deep_symmetric<DIM, DEEP, DEEP>
{
    using type = meta_for_loop_deep_symmetric_pair<DIM>;
};

struct evaluate
{
    template<typename _Tensor>
    static constexpr inline auto apply(_Tensor const& __data){
        if constexpr(is_detected<has_evaluate, _Tensor>::value){
            const_cast<_Tensor&>(__data).evaluate();
        }
    }

    template<typename _Tensor, typename _Result>
    static constexpr inline auto apply(_Tensor const& __data, _Result & __result){
        if constexpr(is_detected<has_evaluate, _Tensor, _Result>::value){
            const_cast<_Tensor&>(__data).evaluate(__result);
        }
    }
};



struct assign_tensors{
    template<typename LHS, typename RHS>
    static constexpr inline auto apply(LHS & lhs, RHS const& rhs){
        constexpr auto RankLHS{LHS::rank()};
        constexpr auto Dim{LHS::dimension()};
        constexpr auto Deep{RankLHS};

        //meta for loop
        using type = typename meta_for_loop_deep<Dim, Deep-1>::type;

        //lambda function
        const auto assign = [&](auto&&... numbers){
            lhs(numbers...) = rhs(numbers...);
        };
        type::loop(assign);
    }
};


struct print_tensor{

    template<typename Tensor>
    static constexpr inline auto apply(Tensor const& lhs, std::ostream & os){
        using value_type = typename Tensor::value_type;
        constexpr auto RankLHS{Tensor::rank()};
        os<<std::scientific;

        if constexpr (RankLHS == 1){
            constexpr auto Dim{Tensor::dimension()};
            for(std::size_t k{0};k<Dim;++k){
                os<<lhs(k)<<" ";
            }
            os<<"\n";
        }else{
            constexpr auto Dim{Tensor::dimension()};
            //meta for loop
            using type = typename meta_for_loop_deep<Dim, RankLHS-2>::type;

            //lambda function
            const auto assign = [&](auto&&... numbers){
                constexpr auto Dim{Tensor::dimension()};
                for(std::size_t k{0};k<Dim;++k){
                    if constexpr (is_complex<value_type>()){
                        if(lhs(numbers...,k).real() >= static_cast<typename value_type::value_type>(0)){
                            os<<" "<<lhs(numbers...,k)<<" ";
                        }else{
                            os<<lhs(numbers...,k)<<" ";
                        }
                    }else{
                        if(lhs(numbers...,k) >= static_cast<value_type>(0)){
                            os<<" "<<lhs(numbers...,k)<<" ";
                        }else{
                            os<<lhs(numbers...,k)<<" ";
                        }
                    }
                }
                os<<"\n";
            };
            type::loop(assign);
        }
    }
};


//currently only used in google test function
struct double_contraction{
    template<typename Result, typename LHS, typename RHS>
    static constexpr inline auto apply(Result& result, LHS const& lhs, RHS const& rhs){
        constexpr auto RankLHS{LHS::rank()};
        constexpr auto RankRHS{RHS::rank()};
        constexpr auto Dim{LHS::dimension()};
        constexpr auto Deep{RankLHS - 2 + RankRHS - 2};
        typename Result::value_type sum{0};
            //meta for loop
            using type = typename meta_for_loop_deep<Dim, Deep-1>::type;

            const auto double_contraction = [&](auto&&... numbers){

                constexpr auto RankLHS{LHS::rank()};
                constexpr auto RankRHS{RHS::rank()};
                constexpr auto Dim{LHS::dimension()};
                constexpr auto Deep{RankLHS - 2 + RankRHS - 2};

                typename Result::value_type sum{0};
                for(std::size_t k{0};k<Dim;++k){
                    for(std::size_t l{0};l<Dim;++l){
                        const auto tuple{std::make_tuple(numbers...,l,k)};
                        if constexpr (RankRHS == 2){
                            constexpr auto SequenceLHS{(RankLHS == 2 ? 0 : RankLHS-3)};
                            //test_seq(make_single_sequence(sequence_t<SequenceLHS>(),sequence_t<Deep+1, Deep>()));
                            //test_seq(make_single_sequence(sequence_t<Deep+1, Deep>()));

                            sum += tuple_call(lhs,tuple,make_single_sequence(sequence_t<SequenceLHS>(),sequence_t<Deep+1, Deep>()))
                                   *tuple_call(rhs,tuple,make_single_sequence(sequence_t<Deep+1, Deep>()));
                        }else if constexpr (RankLHS == 2){
                            //test_seq(sequence_t<Deep+1, Deep>());
                            //test_seq(make_single_sequence(sequence_t<Deep+1, Deep>(),sequence_t<Deep-1,0>()));
                            sum += tuple_call(lhs,tuple,make_single_sequence(sequence_t<Deep+1, Deep>()))
                                   *tuple_call(rhs,tuple,make_single_sequence(sequence_t<Deep+1, Deep>(),sequence_t<Deep-1,0>()));
                        }else{
                            constexpr auto SequenceLHS{(RankLHS == 2 ? 0 : RankLHS-3)};
                            sum += tuple_call(lhs,tuple,make_single_sequence(sequence_t<SequenceLHS>(),sequence_t<Deep+1, Deep>()))
                                   *tuple_call(rhs,tuple,make_single_sequence(sequence_t<Deep+1, Deep>(),sequence_t<Deep-1,RankLHS-2>()));
                        }
                    }
                }

                if constexpr (RankRHS == 2){
                    constexpr auto SequenceLHS{(RankLHS == 2 ? 0 : RankLHS-3)};
                    tuple_call(result,std::make_tuple(numbers...),make_single_sequence(sequence_t<SequenceLHS>())) = sum;
                }else if constexpr (RankLHS == 2){
                    tuple_call(result,std::make_tuple(numbers...),make_single_sequence(sequence_t<Deep-1,RankLHS-2>())) = sum;
                }else{
                    constexpr auto SequenceLHS{(RankLHS == 2 ? 0 : RankLHS-3)};
                    tuple_call(result,std::make_tuple(numbers...),make_single_sequence(sequence_t<SequenceLHS>(),sequence_t<Deep-1,RankLHS-2>())) = sum;
                }
            };

            type::loop(double_contraction);
        //}
    }
};

//currently only used in google test function
struct single_contraction{
    template<typename Result, typename LHS, typename RHS>
    static constexpr inline auto apply(Result& result, LHS const& lhs, RHS const& rhs){
        constexpr auto RankLHS{LHS::rank()};
        constexpr auto RankRHS{RHS::rank()};
        constexpr auto Dim{LHS::dimension()};
        constexpr auto Deep{RankLHS - 1 + RankRHS - 1};

        //meta for loop
        using type = typename meta_for_loop_deep<Dim, Deep-1>::type;

        const auto single_contraction = [&](auto&&... numbers){

            constexpr auto RankLHS{LHS::rank()};
            constexpr auto RankRHS{RHS::rank()};
            constexpr auto Dim{LHS::dimension()};
            constexpr auto Deep{RankLHS - 1 + RankRHS - 1};

            typename Result::value_type sum{0};
            for(std::size_t k{0};k<Dim;++k){
                const auto tuple{std::make_tuple(numbers...,k)};
                if constexpr (RankLHS == 1){
                    sum += lhs(k)
                           *tuple_call(rhs,tuple,make_single_sequence(sequence_t<Deep,Deep>(),sequence_t<RHS::rank()-2>()));
                }else if constexpr (RankRHS == 1){
                    sum += tuple_call(lhs,tuple,make_single_sequence(sequence_t<LHS::rank()-2>(), sequence_t<Deep,Deep>()))
                           *rhs(k);
                }else{
                    sum += tuple_call(lhs,tuple,make_single_sequence(sequence_t<LHS::rank()-2>(), sequence_t<Deep,Deep>()))
                           *tuple_call(rhs,tuple,make_single_sequence(sequence_t<Deep,Deep>(),sequence_t<Deep-1, LHS::rank()-1>()));
                }
            }

            if constexpr (Result::rank() == 1){
                result(std::get<0>(std::make_tuple(numbers...))) = sum;
            }else{
                tuple_call(result,std::make_tuple(numbers...),sequence_t<Result::rank()-1>()) = sum;
            }
        };
        type::loop(single_contraction);
    }
};

} // NAMESPACE DETAIL

#endif // UTILITY_H
