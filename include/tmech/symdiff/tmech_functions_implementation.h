#ifndef TMECH_FUNCTIONS_IMPLEMENTATION_H
#define TMECH_FUNCTIONS_IMPLEMENTATION_H

template<typename _VariableLHS, typename _VariableRHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableLHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableRHS>::value> * = nullptr>
constexpr inline auto otimes(_VariableLHS, _VariableRHS){
    using _lhs_data_type = typename _VariableLHS::data_type;
    using _rhs_data_type = typename _VariableRHS::data_type;
    using _lhs_info = symdiff::detail::get_tensor_info<_lhs_data_type>;
    using _rhs_info = symdiff::detail::get_tensor_info<_rhs_data_type>;

    static_assert (symdiff::detail::is_tensor<_lhs_data_type>::value, "otimes(): left hand argument is not a tensor");
    static_assert (symdiff::detail::is_tensor<_rhs_data_type>::value, "otimes(): right hand argument is not a tensor");
    static_assert (_lhs_info::dimension() == _rhs_info::dimension(), "otimes(): no matching dimensions");
    using LHS_Seq = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_lhs_info::rank()-1>, 1>;
    using RHS_Seq = tmech::detail::add_value_sequence_t<tmech::detail::sequence_t<_rhs_info::rank()-1>, _lhs_info::rank()+1>;
    return symdiff::detail::tensor_outer_product_wrapper<_VariableLHS, _VariableRHS, LHS_Seq, RHS_Seq>();
}

template<typename _VariableLHS, typename _VariableRHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableLHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableRHS>::value> * = nullptr>
constexpr inline auto otimesl(_VariableLHS, _VariableRHS){
    using _lhs_data_type = typename _VariableLHS::data_type;
    using _rhs_data_type = typename _VariableRHS::data_type;
    using _lhs_info = symdiff::detail::get_tensor_info<_lhs_data_type>;
    using _rhs_info = symdiff::detail::get_tensor_info<_rhs_data_type>;

    static_assert (symdiff::detail::is_tensor<_lhs_data_type>::value, "otimesl(): left hand argument is not a tensor");
    static_assert (symdiff::detail::is_tensor<_rhs_data_type>::value, "otimesl(): right hand argument is not a tensor");
    static_assert (_lhs_info::dimension() == _rhs_info::dimension(), "otimesl(): no matching dimensions");
    using LHS_Seq = tmech::sequence<1,4>;
    using RHS_Seq = tmech::sequence<2,3>;
    return symdiff::detail::tensor_outer_product_wrapper<_VariableLHS, _VariableRHS, LHS_Seq, RHS_Seq>();
}

template<typename _VariableLHS, typename _VariableRHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableLHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableRHS>::value> * = nullptr>
constexpr inline auto otimesu(_VariableLHS, _VariableRHS){
    using _lhs_data_type = typename _VariableLHS::data_type;
    using _rhs_data_type = typename _VariableRHS::data_type;
    using _lhs_info = symdiff::detail::get_tensor_info<_lhs_data_type>;
    using _rhs_info = symdiff::detail::get_tensor_info<_rhs_data_type>;

    static_assert (symdiff::detail::is_tensor<_lhs_data_type>::value, "otimesu(): left hand argument is not a tensor");
    static_assert (symdiff::detail::is_tensor<_rhs_data_type>::value, "otimesu(): right hand argument is not a tensor");
    static_assert (_lhs_info::dimension() == _rhs_info::dimension(), "otimesu(): no matching dimensions");
    using LHS_Seq = tmech::sequence<1,3>;
    using RHS_Seq = tmech::sequence<2,4>;
    return symdiff::detail::tensor_outer_product_wrapper<_VariableLHS, _VariableRHS, LHS_Seq, RHS_Seq>();
}

template<typename _SequenceLHS, typename _SequenceRHS, typename _VariableLHS, typename _VariableRHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableLHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableRHS>::value> * = nullptr>
constexpr inline auto outer_product(_VariableLHS, _VariableRHS){
    return symdiff::detail::tensor_outer_product_wrapper<_VariableLHS, _VariableRHS, _SequenceLHS, _SequenceRHS>();
}

template<typename _VariableLHS, typename _VariableRHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableLHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableRHS>::value> * = nullptr>
constexpr inline auto dcontract(_VariableLHS, _VariableRHS){
    using _lhs_data_type = typename _VariableLHS::data_type;
    using _rhs_data_type = typename _VariableRHS::data_type;
    using _lhs_info = symdiff::detail::get_tensor_info<_lhs_data_type>;
    using _rhs_info = symdiff::detail::get_tensor_info<_rhs_data_type>;

    static_assert (symdiff::detail::is_tensor<_lhs_data_type>::value, "otimesu(): left hand argument is not a tensor");
    static_assert (symdiff::detail::is_tensor<_rhs_data_type>::value, "otimesu(): right hand argument is not a tensor");
    static_assert (_lhs_info::dimension() == _rhs_info::dimension(), "otimesu(): no matching dimensions");
    if constexpr (_lhs_info::rank() == 2 && _rhs_info::rank() == 2){
        return symdiff::detail::tensor_inner_product_wrapper<_VariableLHS, _VariableRHS, tmech::sequence<1,2>, tmech::sequence<1,2>>();
    }else if constexpr (_lhs_info::rank() == 4 && _rhs_info::rank() == 2){
        return symdiff::detail::tensor_inner_product_wrapper<_VariableLHS, _VariableRHS, tmech::sequence<3,4>, tmech::sequence<1,2>>();
    }else if constexpr (_lhs_info::rank() == 4 && _rhs_info::rank() == 4){
        return symdiff::detail::tensor_inner_product_wrapper<_VariableLHS, _VariableRHS, tmech::sequence<3,4>, tmech::sequence<1,2>>();
    }else if constexpr (_lhs_info::rank() == 2 && _rhs_info::rank() == 4){
        return symdiff::detail::tensor_inner_product_wrapper<_VariableLHS, _VariableRHS, tmech::sequence<1,2>, tmech::sequence<1,2>>();
    }
}

template<typename _SequenceLHS, typename _SequenceRHS, typename _VariableLHS, typename _VariableRHS,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableLHS>::value> * = nullptr,
         typename std::enable_if_t<symdiff::is_symdiff_type<_VariableRHS>::value> * = nullptr>
constexpr inline auto inner_product(_VariableLHS, _VariableRHS){
    return symdiff::detail::tensor_inner_product_wrapper<_VariableLHS, _VariableRHS, _SequenceLHS, _SequenceRHS>();
}

template<typename _Sequence, typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto basis_change(_Expression){
    return symdiff::detail::tensor_basis_change_wrapper<_Expression, _Sequence>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr auto trans(_Expression){
    using _data_type = typename _Expression::data_type;
    using _tensor_info = symdiff::detail::get_tensor_info<_data_type>;
    static_assert (symdiff::detail::is_tensor<_data_type>::value, "trans: argument is not a tensor type");
    static_assert (_tensor_info::rank() != 4 || _tensor_info::rank() != 2, "trans(): transposition is only valid for 2th or 4th order tensors");
    if constexpr (_tensor_info::rank() == 4){
        return symdiff::detail::tensor_basis_change_wrapper<_Expression, tmech::sequence<3,4,1,2>>();
    }else if constexpr (_tensor_info::rank() == 2) {
        return symdiff::detail::tensor_basis_change_wrapper<_Expression, tmech::sequence<2,1>>();
    }
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto trace(_Expression){
    using _data_type = typename _Expression::data_type;
    using _tensor_info = symdiff::detail::get_tensor_info<_data_type>;
    static_assert (symdiff::detail::is_tensor<_data_type>::value, "trace: argument is not a tensor type");
    static_assert (_tensor_info::rank() == 2, "trace: only for second order tensors");
    return symdiff::detail::function_wrapper<_Expression, symdiff::detail::trace_wrapper>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto det(_Expression){
    using _data_type = typename _Expression::data_type;
    using _tensor_info = symdiff::detail::get_tensor_info<_data_type>;
    static_assert (symdiff::detail::is_tensor<_data_type>::value, "det: argument is not a tensor type");
    static_assert (_tensor_info::rank() == 2, "det: only for second order tensors");
    return symdiff::detail::function_wrapper<_Expression, symdiff::detail::det_wrapper>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto dev(_Expression){
    using data_type   = typename _Expression::data_type;
    using tensor_info = symdiff::detail::get_tensor_info<data_type>;
    using value_type = typename tensor_info::value_type;
    static_assert (symdiff::detail::is_tensor<data_type>::value, "dev: argument is not a tensor type");
    static_assert (tensor_info::rank() == 2, "dev: only for second order tensors");
    //ugly...
    constexpr auto third{static_cast<long long int>(1e10/tensor_info::dimension())};
    return _Expression() - symdiff::real<value_type, 0, third, 1>()*trace(_Expression())*symdiff::tensor_one<data_type>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto sym(_Expression){
    using _data_type   = typename _Expression::data_type;
    using _tensor_info = symdiff::detail::get_tensor_info<_data_type>;
    static_assert (symdiff::detail::is_tensor<_data_type>::value, "sym: argument is not a tensor type");
    static_assert (_tensor_info::rank() == 2, "sym: only for second order tensors");
    return symdiff::real<typename _tensor_info::value_type,0, 5, 1>()*(_Expression() + trans(_Expression()));
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto skew(_Expression){
    using _data_type   = typename _Expression::data_type;
    using _tensor_info = symdiff::detail::get_tensor_info<_data_type>;
    static_assert (symdiff::detail::is_tensor<_data_type>::value, "skew: argument is not a tensor type");
    static_assert (_tensor_info::rank() == 2, "skew: only for second order tensors");
    return symdiff::real<typename _tensor_info::value_type,0, 5, 1>()*(_Expression() - trans(_Expression()));
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto vol(_Expression){
    using data_type   = typename _Expression::data_type;
    using tensor_info = symdiff::detail::get_tensor_info<data_type>;
    using value_type = typename tensor_info::value_type;
    static_assert (symdiff::detail::is_tensor<data_type>::value, "vol: argument is not a tensor type");
    static_assert (tensor_info::rank() == 2, "vol: only for second order tensors");
    //ugly...
    constexpr auto third{static_cast<long long int>(1e10/tensor_info::dimension())};
    return symdiff::real<value_type, 0, third, 1>()*trace(_Expression())*symdiff::tensor_one<data_type>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto adj(_Expression){
    using data_type   = typename _Expression::data_type;
    using tensor_info = symdiff::detail::get_tensor_info<data_type>;
    static_assert (symdiff::detail::is_tensor<data_type>::value, "cof: argument is not a tensor type");
    static_assert (tensor_info::rank() == 2, "cof: only for second order tensors");
    return symdiff::detail::tensor_function_wrapper<_Expression, symdiff::detail::adj_wrapper>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto cof(_Expression){
    using data_type   = typename _Expression::data_type;
    using tensor_info = symdiff::detail::get_tensor_info<data_type>;
    static_assert (symdiff::detail::is_tensor<data_type>::value, "cof: argument is not a tensor type");
    static_assert (tensor_info::rank() == 2, "cof: only for second order tensors");
    return symdiff::detail::tensor_function_wrapper<_Expression, symdiff::detail::cof_wrapper>();
}

template<typename _Expression,
         typename std::enable_if_t<symdiff::is_symdiff_type<_Expression>::value> * = nullptr>
constexpr inline auto inv(_Expression){
    using data_type   = typename _Expression::data_type;
    using tensor_info = symdiff::detail::get_tensor_info<data_type>;
    static_assert (symdiff::detail::is_tensor<data_type>::value, "inv: argument is not a tensor type");
    static_assert (tensor_info::rank() == 2, "inv: only for second order tensors");
    return symdiff::detail::inv_tensor<_Expression>();
}
#endif // TMECH_FUNCTIONS_IMPLEMENTATION_H
