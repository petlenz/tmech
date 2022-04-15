/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef OUTER_PRODUCT_WRAPPER_BONES_H
#define OUTER_PRODUCT_WRAPPER_BONES_H

namespace detail {

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
class outer_product_wrapper : public tensor_base<outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>>
{
    using data_type_RHS  = typename std::remove_const<typename std::remove_reference<_RHS>::type>::type;
    using data_type_LHS  = typename std::remove_const<typename std::remove_reference<_LHS>::type>::type;
    using value_type_LHS = typename data_type_LHS::value_type;
    using value_type_RHS = typename data_type_RHS::value_type;
    using sequence_lhs   = min_value_squence_t<_SequenceLHS, 1>;
    using sequence_rhs   = min_value_squence_t<_SequenceRHS, 1>;

public:
    using value_type = typename result_type<value_type_LHS, value_type_RHS>::value_type;
    using size_type  = std::size_t;

    static_assert (!tmech::detail::check_duplicated_in_sequence_v<sequence_lhs>, "outer_product_wrapper: duplicated values in lhs sequence");
    static_assert (!tmech::detail::check_duplicated_in_sequence_v<sequence_rhs>, "outer_product_wrapper: duplicated values in rhs sequence");
    static_assert (sequence_lhs::size() != 0, "outer_product_wrapper: lhs sequence is empty");
    static_assert (sequence_rhs::size() != 0, "outer_product_wrapper: rhs sequence is empty");
    static_assert (data_type_LHS::dimension() == data_type_RHS::dimension(), "outer_product_wrapper: dimensions does not match");

    constexpr outer_product_wrapper(data_type_LHS const& __lhs, data_type_RHS const& __rhs);

    constexpr outer_product_wrapper(outer_product_wrapper const& __data);

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies ... __indicies)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto evaluate();

private:
    _LHS lhs;
    _RHS rhs;
};


} // NAMESPACE DETAIL

#endif // OUTER_PRODUCT_WRAPPER_BONES_H
