/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef INNER_PRODUCT_WRAPPER_BONES_H
#define INNER_PRODUCT_WRAPPER_BONES_H


namespace detail {

template <typename LHS, typename RHS, typename SequenceLHS, typename SequenceRHS>
class inner_product_wrapper : public tensor_base<inner_product_wrapper<LHS, RHS, SequenceLHS, SequenceRHS>>
{
    using data_type_RHS      = typename std::remove_const<typename std::remove_reference<RHS>::type>::type;
    using data_type_LHS      = typename std::remove_const<typename std::remove_reference<LHS>::type>::type;
    using value_type_LHS     = typename data_type_LHS::value_type;
    using value_type_RHS     = typename data_type_RHS::value_type;
    using sequence_inner_lhs = min_value_squence_t<SequenceLHS, 1>;
    using sequence_inner_rhs = min_value_squence_t<SequenceRHS, 1>;
public:
    using value_type = typename result_type<value_type_LHS, value_type_RHS>::value_type;
    using size_type  = std::size_t;

    static_assert (!tmech::detail::check_duplicated_in_sequence_v<sequence_inner_lhs>, "inner_product_wrapper: duplicated values in lhs sequence");
    static_assert (!tmech::detail::check_duplicated_in_sequence_v<sequence_inner_rhs>, "inner_product_wrapper: duplicated values in rhs sequence");
    static_assert (sequence_inner_lhs::size() != 0, "inner_product_wrapper: lhs sequence is empty");
    static_assert (sequence_inner_rhs::size() != 0, "inner_product_wrapper: rhs sequence is empty");
    static_assert (data_type_LHS::dimension() == data_type_RHS::dimension(), "inner_product_wrapper: dimensions does not match");
    static_assert (sequence_inner_rhs::size() == sequence_inner_rhs::size(), "inner_product_wrapper: SequenceLHS::size != SequenceRHS::size");

    inner_product_wrapper(LHS __lhs, RHS __rhs);

    inner_product_wrapper(inner_product_wrapper const& __data);

    template<typename ...Indicies>
    constexpr inline auto operator ()(Indicies ... __indicies)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

    template <typename _Result>
    constexpr inline auto evaluate(_Result & __result)noexcept;

    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result & __result)noexcept;

    constexpr inline auto raw_data()const noexcept;

private:
    template<typename _LHS, typename _RHS, typename _RESULT>
    constexpr inline auto evaluate_implementation(_RESULT & __result, _LHS const& __lhs, _RHS const& __rhs) const noexcept;

    template<size_type _Size>
    static constexpr inline auto get_size();


private:
    LHS _lhs;
    RHS _rhs;
    tensor<value_type, dimension(), rank()> _data;
    tensor<value_type, dimension(), data_type_LHS::rank()> _lhs_temp;
    tensor<value_type, dimension(), data_type_RHS::rank()> _rhs_temp;

    static constexpr auto RankLHS{data_type_LHS::rank()};
    static constexpr auto RankRHS{data_type_RHS::rank()};
    static constexpr auto SizeLHS{SequenceLHS::size()};
    static constexpr auto SizeRHS{SequenceRHS::size()};
    static constexpr auto SizeOuterLoop{(RankLHS + RankRHS) - (SizeLHS + SizeRHS)};

    using sorted_sequence_lhs = bubble_sort_sequence_t<sequence_inner_lhs>;
    using sorted_sequence_rhs = bubble_sort_sequence_t<sequence_inner_rhs>;
    using sequence_outer_lhs  = set_difference_sequence_t<sequence_t<RankLHS-1>, sorted_sequence_lhs>;
    using sequence_outer_rhs  = set_difference_sequence_t<sequence_t<RankRHS-1>, sorted_sequence_rhs>;
    using new_basis_lhs = add_value_sequence_t<append_sequence_end_t<sequence_outer_lhs, sequence_inner_lhs>,1>;
    using new_basis_rhs = add_value_sequence_t<append_sequence_end_t<sequence_inner_rhs, sequence_outer_rhs>,1>;
};

} // NAMESPACE DETAIL

#endif // INNER_PRODUCT_WRAPPER_BONES_H
