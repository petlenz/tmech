/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef OUTER_PRODUCT_WRAPPER_MEAT_H
#define OUTER_PRODUCT_WRAPPER_MEAT_H


namespace detail {

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::outer_product_wrapper(data_type_LHS const& __lhs, data_type_RHS const& __rhs):
    lhs(__lhs),
    rhs(__rhs)
{}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::outer_product_wrapper(outer_product_wrapper const& __data):
    lhs(__data.lhs),
    rhs(__data.rhs)
{}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
template<typename ...Indicies>
constexpr inline auto outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::operator()(Indicies ... __indicies)const{
    return tuple_call(lhs,std::make_tuple(__indicies...),sequence_lhs())*tuple_call(rhs,std::make_tuple(__indicies...),sequence_rhs());
};

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::dimension(){
    return data_type_RHS::dimension();
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::rank(){
    return data_type_RHS::rank()+data_type_LHS::rank();
}

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
constexpr inline auto outer_product_wrapper<_LHS, _RHS, _SequenceLHS, _SequenceRHS>::evaluate(){
    if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_LHS>::value){
        if constexpr (std::is_reference_v<_LHS>){
            const_cast<data_type_LHS&>(lhs).evaluate();
        }else{
            lhs.evaluate();
        }
    }
    if constexpr(std::experimental::is_detected<detail::has_evaluate, data_type_RHS>::value){
        if constexpr (std::is_reference_v<_RHS>){
            const_cast<data_type_RHS&>(rhs).evaluate();
        }else{
            rhs.evaluate();
        }
    }
}

} // NAMESPACE DETAIL

#endif // OUTER_PRODUCT_WRAPPER_MEAT_H
