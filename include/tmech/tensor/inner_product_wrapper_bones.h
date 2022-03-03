// Copyright 2021 Peter Lenz
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

    inner_product_wrapper(LHS __lhs, RHS __rhs):
        _lhs(__lhs),
        _rhs(__rhs),
        _data(),
        _lhs_temp(),
        _rhs_temp()
    {}

    inner_product_wrapper(inner_product_wrapper const& __data):
        _lhs(__data._lhs),
        _rhs(__data._rhs),
        _data(),
        _lhs_temp(),
        _rhs_temp()
    {}

    template<typename ...Indicies>
    constexpr inline auto operator ()(Indicies ... __indicies)const noexcept{
        return _data(__indicies...);
    };

    static constexpr inline auto dimension()noexcept{
        return data_type_RHS::dimension();
    }

    static constexpr inline auto rank()noexcept{
        return SizeOuterLoop;
    }

    constexpr inline auto evaluate()noexcept{
        if(!this->_is_init){
            evaluate_imp(_data);
            this->_is_init = true;
        }
    }

    template <typename _Result>
    constexpr inline auto evaluate(_Result & __result)noexcept{
        evaluate_imp(__result);
    }

    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result const& __result)noexcept{
        constexpr bool basis_change_lhs{!std::is_same_v<add_value_sequence_t<sequence_t<RankLHS-1>,1>, new_basis_lhs>};
        constexpr bool basis_change_rhs{!std::is_same_v<add_value_sequence_t<sequence_t<RankRHS-1>,1>, new_basis_rhs>};
        constexpr bool raw_data_lhs{std::experimental::is_detected<has_raw_data, data_type_LHS>::value};
        constexpr bool raw_data_rhs{std::experimental::is_detected<has_raw_data, data_type_RHS>::value};
        constexpr bool evaluate_lhs{std::experimental::is_detected<has_evaluate, data_type_LHS>::value};
        constexpr bool evaluate_rhs{std::experimental::is_detected<has_evaluate, data_type_RHS>::value};

        if constexpr (raw_data_lhs && evaluate_lhs && !basis_change_lhs){
            if constexpr (std::is_reference_v<LHS>){
                const_cast<data_type_LHS&>(_lhs).evaluate();
            }else{
                _lhs.evaluate();
            }
        }

        if constexpr (raw_data_rhs && evaluate_rhs && !basis_change_rhs){
            if constexpr (std::is_reference_v<RHS>){
                const_cast<data_type_RHS&>(_rhs).evaluate();
            }else{
                _rhs.evaluate();
            }
        }

        if constexpr (basis_change_lhs || !raw_data_lhs){
            _lhs_temp.template change_basis_view<new_basis_lhs>() = _lhs;
        }


        if constexpr (basis_change_rhs || !raw_data_rhs){
            _rhs_temp.template change_basis_view<new_basis_rhs>() = _rhs;
        }

        if constexpr ((basis_change_lhs && basis_change_rhs) || (!raw_data_lhs && !raw_data_rhs)){
            evaluate_implementation(__result, _lhs_temp, _rhs_temp);
        }else if constexpr (!(basis_change_lhs || !raw_data_lhs) && (basis_change_rhs || !raw_data_rhs)){
            evaluate_implementation(__result, _lhs, _rhs_temp);
        }else if constexpr ((basis_change_lhs || !raw_data_lhs) && !(basis_change_rhs || !raw_data_rhs)){
            evaluate_implementation(__result, _lhs_temp, _rhs);
        }else if constexpr (!(basis_change_lhs || !raw_data_lhs) && !(basis_change_rhs || !raw_data_rhs)){
            evaluate_implementation(__result, _lhs, _rhs);
        }
    }

    constexpr inline auto const& data()const{
        return _data.data();
    }

private:
    template<typename _LHS, typename _RHS, typename _RESULT>
    constexpr inline auto evaluate_implementation(_RESULT const& __result, _LHS const& __lhs, _RHS const& __rhs) const noexcept {
        constexpr auto RowsLHS{get_size<sequence_outer_lhs::size()>()};
        constexpr auto ColsLHS{get_size<sequence_inner_lhs::size()>()};
        constexpr auto RowsRHS{get_size<sequence_inner_rhs::size()>()};
        constexpr auto ColsRHS{get_size<sequence_outer_rhs::size()>()};
        gemm_wrapper<typename _LHS::value_type, typename _RHS::value_type, typename _RESULT::value_type, RowsLHS, ColsLHS, RowsRHS, ColsRHS>::evaluate(__lhs.raw_data(), __rhs.raw_data(), __result.raw_data());
    }

    template<size_type _Size>
    static constexpr inline auto get_size(){
        if constexpr (_Size == 0){
            return 1ul;
        }else{
            return get_tensor_size<dimension(), _Size>::size;
        }
    }


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
