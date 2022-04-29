/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef META_INNER_PRODUCT_WRAPPER_BONES_H
#define META_INNER_PRODUCT_WRAPPER_BONES_H

namespace detail {

struct meta_inner_product{

    template <typename SequenceLHS, typename SequenceRHS, typename LHS, typename RHS, typename Result>
    static constexpr inline auto evaluate(LHS const& lhs, RHS const& rhs, Result const & res)noexcept{
        using data_type_RHS  = typename std::remove_const<typename std::remove_reference<RHS>::type>::type;
        using data_type_LHS  = typename std::remove_const<typename std::remove_reference<LHS>::type>::type;
        using value_type_LHS = typename data_type_LHS::value_type;
        using value_type_RHS = typename data_type_RHS::value_type;
        using value_type_RES = typename Result::value_type;
        using sequence_inner_lhs   = min_value_squence_t<SequenceLHS, 1>;
        using sequence_inner_rhs   = min_value_squence_t<SequenceRHS, 1>;
        constexpr auto RankLHS{LHS::rank()};
        constexpr auto RankRHS{RHS::rank()};
        constexpr auto SizeLHS{SequenceLHS::size()};
        constexpr auto SizeRHS{SequenceRHS::size()};
        constexpr auto RHSOuterIndices{RankRHS - SizeRHS};
        //constexpr auto RHSInnerIndices{SizeRHS};
        constexpr auto LHSOuterIndices{RankLHS - SizeLHS};
        //constexpr auto LHSInnerIndices{SizeLHS};
        constexpr auto SizeInnerLoop{SizeLHS};
        constexpr auto SizeOuterLoop{(RankLHS + RankRHS) - (SizeLHS + SizeRHS)};

        using sequence_inner            = typename seq_gen<SizeOuterLoop, SizeOuterLoop+SizeInnerLoop>::type;
        using sequence_outer_lhs        = typename seq_gen<0, LHSOuterIndices>::type;
        using sequence_outer_rhs        = typename seq_gen<LHSOuterIndices, LHSOuterIndices+RHSOuterIndices>::type;
        using sequence_start_lhs        = typename seq_gen<0, RankLHS>::type;
        using sequence_start_rhs        = typename seq_gen<0, RankRHS>::type;
        using sequence_swaped_inner_lhs = swape_elements_squence_t<sequence_start_lhs, sequence_inner_lhs, sequence_inner>;
        using sequence_swaped_inner_rhs = swape_elements_squence_t<sequence_start_rhs, sequence_inner_rhs, sequence_inner>;
        using outer_position_lhs        = set_difference_sequence_t<sequence_start_lhs, sequence_inner_lhs>;
        using outer_position_rhs        = set_difference_sequence_t<sequence_start_rhs, sequence_inner_rhs>;
        using sequence_lhs              = change_values_at_positions_t<sequence_swaped_inner_lhs, outer_position_lhs, sequence_outer_lhs>;
        using sequence_rhs              = change_values_at_positions_t<sequence_swaped_inner_rhs, outer_position_rhs, sequence_outer_rhs>;
        using sequence_result           = typename seq_gen<0, SizeOuterLoop>::type;

        using type = typename meta_for_loop_deep<LHS::dimension(), SizeOuterLoop-1>::type;

        if constexpr (std::is_same_v<sequence<3,4>, SequenceLHS> && std::is_same_v<sequence<1,2>, SequenceRHS> && RankLHS == 4 && RankRHS == 4 && LHS::dimension() >= 2){
            constexpr std::size_t Size{LHS::dimension()*LHS::dimension()};
            gemm_wrapper<value_type_LHS, value_type_RHS, typename Result::value_type, Size, Size, Size, Size>::evaluate(lhs.raw_data(), rhs.raw_data(), res.raw_data());
            return ;
        }

//        if constexpr (std::is_same_v<sequence<3,4>, SequenceLHS> && std::is_same_v<sequence<1,2>, SequenceRHS> && RankLHS == 4 && RankRHS == 2 && LHS::dimension() >= 3){
//            constexpr std::size_t Size{LHS::dimension()*LHS::dimension()};
//            gemm_wrapper<value_type_LHS, value_type_RHS, typename Result::value_type, Size, Size, Size, 1>::evaluate(lhs.raw_data(), rhs.raw_data(), res.raw_data());
//            return ;
//        }

//        if constexpr (std::is_same_v<sequence<2>, SequenceLHS> && std::is_same_v<sequence<1>, SequenceRHS> && RankLHS == 2 && RankRHS == 2 && LHS::dimension() >= 3){
//            constexpr std::size_t Size{LHS::dimension()};
//            gemm_wrapper<value_type_LHS, value_type_RHS, typename Result::value_type, Size, Size, Size, Size>::evaluate(lhs.raw_data(), rhs.raw_data(), res.raw_data());
//            return ;
//        }


//        if constexpr (std::is_same_v<sequence<1,2>, SequenceLHS> && std::is_same_v<sequence<3,4>, SequenceRHS> && RankLHS == 2 && RankRHS == 4 && LHS::dimension() >= 3){
//            constexpr std::size_t Size{LHS::dimension()*LHS::dimension()};
//            gemm_wrapper<value_type_LHS, value_type_RHS, typename Result::value_type, Size, 1, Size, Size>::evaluate(lhs.raw_data(), rhs.raw_data(), res.raw_data());
//            return ;
//        }

        //not needed
//        if constexpr (std::is_same_v<sequence<1,2>, SequenceLHS> && std::is_same_v<sequence<1,2>, SequenceRHS> && RankLHS == 2 && RankRHS == 4){
//            constexpr std::size_t Size{LHS::dimension()};
//            for(std::size_t i{0}; i<Size; ++i){
//                for(std::size_t j{0}; j<Size; ++j){
//                    //b_{ij} = a_{kl}*A_{klij}
//                    typename Result::value_type sum{0};
//                    for(std::size_t k{0}; k<Size; ++k){
//                        for(std::size_t l{0}; l<Size; ++l){
//                            sum += lhs(k,l)*rhs(k,l,i,j);
//                        }
//                    }
//                    const_cast<Result&>(res)(i,j) = sum;
//                }
//            }
//            return ;
//        }


        const auto outer_loop = [&](auto&&... numbersOuter){
            constexpr auto SizeInnerLoop{SequenceLHS::size()};

            if constexpr (SizeInnerLoop == 1){
                if constexpr (std::is_fundamental_v<Result>){
                    for(std::size_t i{0}; i<LHS::dimension(); ++i){
                        const auto tuple = std::make_tuple(numbersOuter..., i);
                        res += tuple_call(lhs, tuple, sequence_lhs()) * tuple_call(rhs, tuple, sequence_rhs());
                    }
                }else{
                    typename Result::value_type sum{0};
                    for(std::size_t i{0}; i<LHS::dimension(); ++i){
                        const auto tuple = std::make_tuple(numbersOuter..., i);
                        sum += tuple_call(lhs, tuple, sequence_lhs()) * tuple_call(rhs, tuple, sequence_rhs());
                    }
                    tuple_call(const_cast<Result&>(res), std::make_tuple(numbersOuter...), sequence_result()) += sum;
                }
            }else{
                using type = typename meta_for_loop_deep<LHS::dimension(), SizeInnerLoop-2>::type;
                if constexpr (std::is_fundamental_v<Result>){
                    const auto inner_loop = [&](auto&&... numbersInner){
                        for(std::size_t i{0}; i<LHS::dimension(); ++i){
                            const auto tuple = std::make_tuple(numbersOuter..., numbersInner..., i);
                            res += tuple_call(lhs, tuple, sequence_lhs()) * tuple_call(rhs, tuple, sequence_rhs());
                        }
                    };

                    type::loop(inner_loop);
                }else{
                    typename Result::value_type sum{0};
                    const auto inner_loop = [&](auto&&... numbersInner){
                        for(std::size_t i{0}; i<LHS::dimension(); ++i){
                            const auto tuple = std::make_tuple(numbersOuter..., numbersInner..., i);
                            sum += tuple_call(lhs, tuple, sequence_lhs()) * tuple_call(rhs, tuple, sequence_rhs());
                        }
                    };
                    type::loop(inner_loop);
                    tuple_call(const_cast<Result&>(res), std::make_tuple(numbersOuter...), sequence_result()) += sum;
                }
            }
        };

        type::loop(outer_loop);
    }

private:
};

} // NAMESPACE DETAIL

#endif // META_INNER_PRODUCT_WRAPPER_BONES_H
