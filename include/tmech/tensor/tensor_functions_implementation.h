/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_FUNCTIONS_H
#define TENSOR_FUNCTIONS_H

#include <type_traits>
#include "utility.h"

/// @cond

template<typename _TensorLHS, typename _TensorRHS, typename _T,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr,
         std::enable_if_t<std::is_fundamental_v<_T>> * = nullptr>
constexpr inline auto almost_equal(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs, _T const __eps){
    return detail::compare_almost_equal::check(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs), __eps);
}

template<typename _TensorLHS,
         typename _TensorRHS,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr auto otimesu(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "otimesu: no matching dimensions");
    static_assert(TensorTypeLHS::rank() == 2, "otimesu: left hand side tensors rank is not a tensor of rank two");
    static_assert(TensorTypeRHS::rank() == 2, "otimesu: right hand side tensors rank is not a tensor of rank two");
    using _SequenceLHS = sequence<1,3>;
    using _SequenceRHS = sequence<2,4>;
    if constexpr (isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}


template<typename _TensorLHS,
         typename _TensorRHS,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr auto otimesl(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "times: no matching dimensions");
    static_assert(TensorTypeLHS::rank() == 2, "otimesl: left hand side tensors rank is not a tensor of rank two");
    static_assert(TensorTypeRHS::rank() == 2, "otimesl: right hand side tensors rank is not a tensor of rank two");
    using _SequenceLHS = sequence<1,4>;
    using _SequenceRHS = sequence<2,3>;

    if constexpr (isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}

template<typename _TensorLHS,
         typename _TensorRHS,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr auto otimes(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "otimes: no matching dimensions");
    using _SequenceLHS = detail::add_value_sequence_t<detail::sequence_t<TensorTypeLHS::rank()-1>, 1>;
    using _SequenceRHS = detail::add_value_sequence_t<detail::sequence_t<TensorTypeRHS::rank()-1>, TensorTypeLHS::rank()+1>;
    if constexpr (isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}



template<typename _TensorLHS,
         typename _TensorRHS,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr auto inline dcontract(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    if constexpr(TensorTypeLHS::rank() == 2 && TensorTypeRHS::rank() == 2){
        return detail::dot_wrapper::evaluate(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        using _SequenceRHS = sequence<1,2>;
        using _SequenceLHS = sequence<TensorTypeLHS::rank()-1,TensorTypeLHS::rank()>;

        if constexpr (isLvalueLHS && isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else if constexpr (!isLvalueLHS && isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else if constexpr (isLvalueLHS && !isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else{
            return detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }
    }
}

template<typename _TensorLHS,
         typename _TensorRHS,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr auto inline ddcontract(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    if constexpr(TensorTypeLHS::rank() == 4 && TensorTypeRHS::rank() == 4){
        return detail::dot_wrapper::evaluate(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        using _SequenceRHS = sequence<1,2,3,4>;
        using _SequenceLHS = detail::add_value_sequence_t<sequence<0,1,2,3>,TensorTypeLHS::rank()-3>;

        if constexpr (isLvalueLHS && isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else if constexpr (!isLvalueLHS && isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else if constexpr (isLvalueLHS && !isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else{
            return detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }
    }
}

template<typename _SequenceLHS, typename _SequenceRHS, typename _TensorLHS, typename _TensorRHS, std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr, std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto outer_product(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    if constexpr (isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueRHS){
        return detail::outer_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return detail::outer_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}


template<typename _SequenceLHS, typename _SequenceRHS, typename _TensorLHS, typename _TensorRHS, std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr, std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto inner_product(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    if constexpr (_SequenceLHS::size() == TensorTypeLHS::rank() && _SequenceRHS::size() == TensorTypeRHS::rank()){
        return detail::dot_wrapper::evaluate(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        if constexpr (isLvalueLHS && isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else if constexpr (!isLvalueLHS && isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS const&, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else if constexpr (isLvalueLHS && !isLvalueRHS){
            return detail::inner_product_wrapper<TensorTypeLHS const&, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }else{
            return detail::inner_product_wrapper<TensorTypeLHS, TensorTypeRHS, _SequenceLHS, _SequenceRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
        }
    }
}


template<typename _TensorLHS, typename _TensorRHS, std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr, std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto cross(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "cross(): tensor rank mismatch.");
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "cross(): tensor dimension mismatch.");
    static_assert(TensorTypeLHS::rank() == 1, "cross(): only valid for tensors with rank == 1.");
    static_assert(TensorTypeLHS::dimension() == 3, "cross(): only valid for tensors with dimension == 3.");

    constexpr bool isLvalueLHS{std::is_lvalue_reference_v<_TensorLHS>};
    constexpr bool isLvalueRHS{std::is_lvalue_reference_v<_TensorRHS>};

    if constexpr (isLvalueLHS && isLvalueRHS){
        return detail::cross_product_wrapper<TensorTypeLHS const&, TensorTypeRHS const&>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (!isLvalueLHS && isLvalueRHS){
        return detail::cross_product_wrapper<TensorTypeLHS, TensorTypeRHS const&>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else if constexpr (isLvalueLHS && !isLvalueRHS){
        return detail::cross_product_wrapper<TensorTypeLHS const&, TensorTypeRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }else{
        return detail::cross_product_wrapper<TensorTypeLHS, TensorTypeRHS>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
    }
}

template<typename _TensorLHS, typename _TensorRHS, std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr, std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto dot(_TensorLHS && __tensor_lhs, _TensorRHS && __tensor_rhs){
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    static_assert ((TensorTypeLHS::rank() == 1) || (TensorTypeRHS::rank() == 1), "dot: is only available for first-order tensors");
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "dot: no matching dimensions");

    return inner_product<sequence<1>,sequence<1>>(std::forward<_TensorLHS>(__tensor_lhs), std::forward<_TensorRHS>(__tensor_rhs));
}

template<typename _T, typename std::enable_if<std::is_fundamental_v<_T>>::type* = nullptr>
constexpr inline _T sign(_T const __value){
    return (__value == 0 ? 0 : (__value > 0 ? 1 : -1));
}


template<typename _TupleSequence = void,
         typename _Tensor,
         typename _T,
         std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr,
         std::enable_if_t<std::is_fundamental_v<_T>> * = nullptr>
constexpr inline auto convert_tensor_to_voigt(_Tensor && __tensor, _T * __ptr){
    using TensorType = typename std::decay<_Tensor>::type;

    if constexpr (std::is_same_v<void, _TupleSequence> && TensorType::rank() == 2){
        return detail::convert_tensor_to_voigt_wrapper<std::tuple<sequence<1,2>>, TensorType>::convert(std::forward<_Tensor>(__tensor), __ptr);
    }else if constexpr (std::is_same_v<void, _TupleSequence> && TensorType::rank() == 4){
        return detail::convert_tensor_to_voigt_wrapper<std::tuple<sequence<1,2>, sequence<3,4>>, TensorType>::convert(std::forward<_Tensor>(__tensor), __ptr);
    }else if constexpr (!std::is_same_v<void, _TupleSequence>){
        return detail::convert_tensor_to_voigt_wrapper<_TupleSequence, TensorType>::convert(std::forward<_Tensor>(__tensor), __ptr);
    }else{
        static_assert (std::is_same_v<void, _TupleSequence>, "convert_tensor_to_voigt(): missing sequence");
    }
}

template<typename _Sequence, typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto basis_change(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::basis_change_wrapper<const TensorType&, _Sequence>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::basis_change_wrapper<TensorType, _Sequence>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr auto trace(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "trace: only valid for second order tensors");
    using value_type = typename TensorType::value_type;
    constexpr auto Dim{TensorType::dimension()};
    value_type sum{0};
    //mybe a meta programm...
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        TensorType data = std::forward<_Tensor>(__tensor);
        detail::evaluate::apply(data);
        for(std::size_t i{0};i<Dim;++i){sum += data(i,i);}
    }else{
        TensorType const& data = std::forward<_Tensor>(__tensor);
        detail::evaluate::apply(data);
        for(std::size_t i{0};i<Dim;++i){sum += data(i,i);}
    }
    return sum;
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto abs(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::abs_tensor_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::abs_tensor_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto eval(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::eval_tensor_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::eval_tensor_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr auto trans(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert (TensorType::rank() != 4 || TensorType::rank() != 2, "trans(): transposition is only valid for 2th or 4th order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        if constexpr (TensorType::rank() == 4){
            //major transpose
            return detail::basis_change_wrapper<const TensorType&, sequence<3,4,1,2>>(std::forward<_Tensor>(__tensor));
        }else if constexpr (TensorType::rank() == 2) {
            return detail::basis_change_wrapper<const TensorType&, sequence<2,1>>(std::forward<_Tensor>(__tensor));
        }
    }else{
        if constexpr (TensorType::rank() == 4){
            //major transpose
            return detail::basis_change_wrapper<TensorType, sequence<3,4,1,2>>(std::forward<_Tensor>(__tensor));
        }else if constexpr (TensorType::rank() == 2) {
            return detail::basis_change_wrapper<TensorType, sequence<2,1>>(std::forward<_Tensor>(__tensor));
        }
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr auto transl(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 4, "transl(): only valid for fourth order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::basis_change_wrapper<const TensorType&, sequence<2,1,3,4>>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::basis_change_wrapper<TensorType, sequence<2,1,3,4>>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr auto skew(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "skew: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::skew_symmetric_part_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::skew_symmetric_part_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr auto inline sym(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "sym: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::symmetric_part_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::symmetric_part_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto vol(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "vol: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::volumetric_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::volumetric_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto dev(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "dev: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::deviatoric_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::deviatoric_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto cof(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "cof: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::cofactor_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::cofactor_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto adj(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "adj: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::adjoint_wrapper<const TensorType&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::adjoint_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr, typename ..._Sequences>
constexpr inline auto inv(_Tensor && __tensor, _Sequences ...){
    using TensorType = typename std::decay<_Tensor>::type;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        if constexpr (TensorType::rank() == 4 && sizeof...(_Sequences) == 0){
            return detail::inverse_wrapper<TensorType const&, tmech::sequence<1,2>, tmech::sequence<3,4>>(std::forward<_Tensor>(__tensor));
        }else{
            return detail::inverse_wrapper<TensorType const&, _Sequences...>(std::forward<_Tensor>(__tensor));
        }
    }else{
        if constexpr (TensorType::rank() == 4 && sizeof...(_Sequences) == 0){
            return detail::inverse_wrapper<TensorType, tmech::sequence<1,2>, tmech::sequence<3,4>>(std::forward<_Tensor>(__tensor));
        }else{
            return detail::inverse_wrapper<TensorType, _Sequences...>(std::forward<_Tensor>(__tensor));
        }
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr, typename ..._Sequences>
constexpr inline auto invf(_Tensor && __tensor, _Sequences ...){
    using TensorType = typename std::decay<_Tensor>::type;
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::inverse_wrapper<TensorType const&, _Sequences...>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::inverse_wrapper<TensorType, _Sequences...>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto pow(_Tensor && __tensor, T const exp){
    using TensorType = typename std::decay<_Tensor>::type;

    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::pow_tensor_wrapper<TensorType const&>(std::forward<_Tensor>(__tensor), exp);
    }else{
        return detail::pow_tensor_wrapper<TensorType>(std::forward<_Tensor>(__tensor), exp);
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto sign(_Tensor && __tensor, typename std::decay<_Tensor>::type::value_type __eps, std::size_t __max_iter){
    using TensorType = typename std::decay<_Tensor>::type;
    return detail::sign_tensor_wrapper<TensorType>(std::forward<_Tensor>(__tensor), __eps, __max_iter);
}

template<typename _Function, typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto isotropic_tensor_function(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "isotropic_tensor_function: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::isotropic_tensor_function<TensorType const&, _Function>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::isotropic_tensor_function<TensorType, _Function>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto sqrt(_Tensor && __tensor){
    return isotropic_tensor_function<detail::sqrt_>(std::forward<_Tensor>(__tensor));
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto exp(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "exp: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::exp_tensor_wrapper<TensorType const&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::exp_tensor_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto exp_sym(_Tensor && __tensor){
    return isotropic_tensor_function<detail::exp>(std::forward<_Tensor>(__tensor));
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto log(_Tensor && __tensor){
    return isotropic_tensor_function<detail::log>(std::forward<_Tensor>(__tensor));
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto positive(_Tensor && __tensor){
    return isotropic_tensor_function<detail::positive_part>(std::forward<_Tensor>(__tensor));
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto negative(_Tensor && __tensor){
    return isotropic_tensor_function<detail::negative_part>(std::forward<_Tensor>(__tensor));
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto positive_negative_decomposition(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "positive_negative_decomposition: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::positive_negative_decomposition_wrapper<TensorType const&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::positive_negative_decomposition_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto polar_decomposition(_Tensor && __tensor, bool const newton_method = false, typename std::decay<_Tensor>::type::value_type const tol = 1e-10, std::size_t const max_steps = 20){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "polar_decomposition: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::polar_decomposition_wrapper<TensorType const&>(std::forward<_Tensor>(__tensor), newton_method, tol, max_steps);
    }else{
        return detail::polar_decomposition_wrapper<TensorType>(std::forward<_Tensor>(__tensor), newton_method, tol, max_steps);
    }
}

template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
constexpr inline auto eigen_decomposition(_Tensor && __tensor){
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == 2, "eigen_decomposition: only valid for second order tensors");
    if constexpr (std::is_lvalue_reference_v<_Tensor>){
        return detail::eigen_decomposition_wrapper<TensorType const&>(std::forward<_Tensor>(__tensor));
    }else{
        return detail::eigen_decomposition_wrapper<TensorType>(std::forward<_Tensor>(__tensor));
    }
}




/// @endcond

































template<typename _Derived>
constexpr inline auto det(tensor_base<_Derived>const& __A){
    static_assert (_Derived::rank() == 2, "det: only for rank 2 tensors");
    tensor<typename _Derived::value_type, _Derived::dimension(), _Derived::rank()> temp{__A.convert()};
    //det wrapper
    constexpr auto rows{temp.dimension()};
    if constexpr (rows == 2){
        return detail::det_2_2(temp.raw_data());
    }else if constexpr (rows == 3){
        return detail::det_3_3(temp.raw_data());
    }else if constexpr (rows == 4){
        return detail::det_4_4(temp.raw_data());
    }else if constexpr (rows == 5){
        return detail::det_5_5(temp.raw_data());
    }else if constexpr (rows == 6){
        return detail::det_6_6(temp.raw_data());
    }
}




/**
 * @brief The (2)-norm of a tensor is defined for a first-, second- and fourth-order tensor as
 *
 * \f[
 * ||\mathbf{a}|| &= \sqrt{\text{dot}\left(\mathbf{a},\mathbf{a}\right)}  = \sqrt{a_ia_i} \\
 * ||\mathbf{A}|| &= \sqrt{\mathbf{A}:\mathbf{A}}= \sqrt{A_{ij}A_{ij}} \\
 * ||\mathbb{A}||     &= \sqrt{\mathbb{A}::\mathbb{A}}= \sqrt{A_{ijkl}A_{ijkl}}
 * \f]
 * @param data_base Tensor object from which the (2)-norm is to be formed.
 */
template<typename Derived>
constexpr inline auto norm(tensor_base<Derived> const& data_base){
    //static_assert (Derived::rank() == 1 || Derived::rank() == 2 || Derived::rank() == 4,"norm: only valid for a first-, second- or fourth-order tensor");
    using _Sequence = detail::add_value_sequence_t<detail::sequence_t<Derived::rank()-1>, 1>;

//    if constexpr (Derived::rank() == 1){
//        return std::sqrt(dot(data_base.convert(),data_base.convert()));
//    }else if constexpr (Derived::rank() == 2){
//        return std::sqrt(dcontract(data_base.convert(),data_base.convert()));
//    }else if constexpr (Derived::rank() == 4) {
//        return std::sqrt(ddcontract(data_base.convert(),data_base.convert()));
//    }

    return std::sqrt(inner_product<_Sequence,_Sequence>(data_base.convert(),data_base.convert()));
}


/**
 * @brief Numerical differentiation of expressions based on the central differences scheme.
 *
 */
template<typename Position, typename Function, typename Direction>
inline auto num_diff_central(Function func, Direction const& x, double const eps){
    using output = decltype (func(x));

    if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == true){
        static_assert (std::is_same_v<double, Direction>,
                "Numerical differentiation: only for double precision tensors");
        double result{0};
        detail::numdiff_central<Position>::evaluate(func, x, result, eps);
        return result;

    }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == true) {
        static_assert (std::is_same_v<double, typename Direction::value_type>,
                "Numerical differentiation: only for double precision tensors");
        tensor<double, Direction::dimension(), Direction::rank()> result;
        detail::numdiff_central<Position>::evaluate(func, x, result, eps);
        return result;

    }else if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == false) {
        static_assert (std::is_same_v<double, Direction>,
                "Numerical differentiation: only for double precision tensors");
        tensor<double, output::dimension(), output::rank()> result;
        detail::numdiff_central<Position>::evaluate(func, x, result, eps);
        return result;

    }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == false) {
        static_assert (std::is_same_v<double, typename Direction::value_type>,
                "Numerical differentiation: only for double precision tensors");
        tensor<double, output::dimension(), output::rank()+Direction::rank()> result;
        detail::numdiff_central<Position>::evaluate(func, x, result, eps);
        return result;
    }
}


template<typename SymDirection, typename SymResult, typename Function, typename Direction>
inline auto num_diff_sym_central(Function func, Direction const& x, double const eps){
    using output = decltype (func(x));

    if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == true){
        static_assert (std::is_same_v<double, Direction>,
                "Numerical differentiation: only for double precision tensors");
        double result{0};
        detail::numdiff_central_symmetric<SymDirection, SymResult>::evaluate(func, x, result, eps);
        return result;

    }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == true) {
        static_assert (std::is_same_v<double, typename Direction::value_type>,
                "Numerical differentiation: only for double precision tensors");
        tensor<double, Direction::dimension(), Direction::rank()> result;
        detail::numdiff_central_symmetric<SymDirection, SymResult>::evaluate(func, x, result, eps);
        return result;

    }else if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == false) {
        static_assert (std::is_same_v<double, Direction>,
                "Numerical differentiation: only for double precision tensors");
        tensor<double, output::dimension(), output::rank()> result;
        detail::numdiff_central_symmetric<SymDirection, SymResult>::evaluate(func, x, result, eps);
        return result;

    }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == false) {
        static_assert (std::is_same_v<double, typename Direction::value_type>,
                "Numerical differentiation: only for double precision tensors");
        tensor<double, output::dimension(), output::rank()+Direction::rank()> result;
        detail::numdiff_central_symmetric<SymDirection, SymResult>::evaluate(func, x, result, eps);
        return result;
    }
}


template<typename T>
constexpr inline auto convert_3D_to_2D(tensor<T, 3, 2> const& A){
    tensor<T, 2, 2> _A;
    for(std::size_t i{0}; i<2; ++i){
        for(std::size_t j{0}; j<2; ++j){
            _A(i,j) = A(i,j);
        }
    }
    return _A;
}

template<typename T>
constexpr inline auto convert_3D_to_2D(tensor<T, 3, 4> const& A){
    tensor<T, 2, 4> _A;
    for(std::size_t i{0}; i<2; ++i){
        for(std::size_t j{0}; j<2; ++j){
            for(std::size_t k{0}; k<2; ++k){
                for(std::size_t l{0}; l<2; ++l){
                    _A(i,j,k,l) = A(i,j,k,l);
                }
            }
        }
    }
    return _A;
}




/**
 * @brief General Newton-Rapson method for scalar and tensor expressions
 */
template<typename System, typename T, typename ...Data>
inline auto general_newton_raphson_iterate(System A, std::tuple<Data...> & x, T const tol, std::size_t const max_iter){
    std::size_t iter{0};

    //removes references of tuple entries
    //local data
    typename detail::general_newton_raphson_solver::result_type<std::tuple<Data...>> dx;

    for(std::size_t iter{0}; iter<=max_iter; ++iter){
        const auto norm = detail::general_newton_raphson_solver::apply(A, x, dx, tol);
        std::cout<<"iter "<<iter<<" norm "<<norm<<std::endl;
        if(norm < tol){
            return std::make_tuple(iter, norm, x);
        }
    }

    throw std::runtime_error("general_newton_raphson_iterate: no convergenz");
}

#endif // TENSOR_FUNCTIONS_H
