/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef INNER_PRODUCT_WRAPPER_MEAT_H
#define INNER_PRODUCT_WRAPPER_MEAT_H


namespace detail {

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::inner_product_wrapper(_LHS __lhs, _RHS __rhs):
    _lhs(__lhs),
    _rhs(__rhs),
    _data(),
    _lhs_temp(),
    _rhs_temp()
{}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::inner_product_wrapper(inner_product_wrapper const& __data):
    _lhs(__data._lhs),
    _rhs(__data._rhs),
    _data(),
    _lhs_temp(),
    _rhs_temp()
{}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
template<typename ...Indicies>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::operator()(Indicies ... __indicies)const noexcept{
    return _data(__indicies...);
};

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::dimension()noexcept{
    return data_type_RHS::dimension();
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::rank()noexcept{
    return SizeOuterLoop;
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::evaluate()noexcept{
    if(!this->_is_init){
        evaluate_imp(_data);
        this->_is_init = true;
    }
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
template <typename _Result>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::evaluate(_Result & __result)noexcept{
    evaluate_imp(__result);
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
template<typename _Result>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::evaluate_imp(_Result & __result)noexcept{
    constexpr bool basis_change_lhs{!std::is_same_v<add_value_sequence_t<sequence_t<RankLHS-1>,1>, new_basis_lhs>};
    constexpr bool basis_change_rhs{!std::is_same_v<add_value_sequence_t<sequence_t<RankRHS-1>,1>, new_basis_rhs>};
    constexpr bool raw_data_lhs{is_detected<has_raw_data, data_type_LHS>::value};
    constexpr bool raw_data_rhs{is_detected<has_raw_data, data_type_RHS>::value};

    if constexpr (raw_data_lhs && !basis_change_lhs){
        evaluate::apply(_lhs);
    }

    if constexpr (raw_data_rhs && !basis_change_rhs){
        evaluate::apply(_rhs);
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

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::raw_data()const noexcept{
    return _data.raw_data();
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
template<typename _LHS_IN, typename _RHS_IN, typename _RESULT>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::evaluate_implementation(_RESULT & __result, _LHS_IN const& __lhs, _RHS_IN const& __rhs) const noexcept {
    constexpr auto RowsLHS{get_size<sequence_outer_lhs::size()>()};
    constexpr auto ColsLHS{get_size<sequence_inner_lhs::size()>()};
    constexpr auto RowsRHS{get_size<sequence_inner_rhs::size()>()};
    constexpr auto ColsRHS{get_size<sequence_outer_rhs::size()>()};
    gemm_wrapper<value_type_LHS, value_type_RHS, typename _RESULT::value_type, RowsLHS, ColsLHS, RowsRHS, ColsRHS>::evaluate(__lhs.raw_data(), __rhs.raw_data(), __result.raw_data());
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
template<std::size_t _Size>
constexpr inline auto inner_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::get_size(){
    if constexpr (_Size == 0){
        return 1ul;
    }else{
        return get_tensor_size<dimension(), _Size>::size;
    }
}
} // NAMESPACE DETAIL

#endif // INNER_PRODUCT_WRAPPER_MEAT_H
