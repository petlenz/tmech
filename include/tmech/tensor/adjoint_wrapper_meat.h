/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ADJOINT_WRAPPER_MEAT_H
#define ADJOINT_WRAPPER_MEAT_H

namespace detail {

/**
 * @name Constructors
*/
//@{
/**
 * @brief Default constructor
 * @param data A tensor expression, from which the adjoint is computed.
*/
template <typename _Tensor>
adjoint_wrapper<_Tensor>::adjoint_wrapper(data_type_tensor const& data)noexcept:
    _data(),
    _data_basis(data)
{}

/**
 * @brief Copy constructor
 * @param data A %adjoint_wrapper. Note, that only the
 * underlying tensor expression in copied.
 */
template <typename _Tensor>
adjoint_wrapper<_Tensor>::adjoint_wrapper(adjoint_wrapper const& data)noexcept:
    basetype(data),
    _data(data._data),
    _data_basis(data._data_basis)
{}
//@}

/**
 * @name Data
*/
/**
 * @brief Returns a constant reference of the element at the specified position in the adjoint tensor.
 * @param i Index specifying the position in the tensor expression.
 * @param j Index specifying the position in the tensor expression.
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::operator()(size_type const i, size_type const j)const noexcept{
    return _data(i,j);
}

/**
 * @brief  Memory access to the data contained in the %numerical_differentiation_backward_sym_wrapper.
 * @return Read pointer to the underlying memory.
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::raw_data()const noexcept{
    return _data.raw_data();
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
* @return dimension
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::dimension()noexcept{
    return data_type_tensor::dimension();
}

/**
 * @brief Returns the rank.
 * @return rank
 */
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::rank()noexcept{
    return data_type_tensor::rank();
}
//@}

/**
* @name Evaluation of expression
*/
//@{
/**
* @brief Uses %result as container for the computation.
* The finial result is saved in %result. No internal memory is used.
*
* @param result External tensor to save the result.
*/
template <typename _Tensor>
template<typename _Result>
constexpr inline auto adjoint_wrapper<_Tensor>::evaluate(_Result & result)noexcept{
    this->_is_init = false;
    evaluate_imp(result);
}

/**
* @brief Internal memory is used to compute the result.
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::evaluate()noexcept{
    evaluate_imp(_data);
}

/**
* @brief Implementation of the evaluation function.
* The final result is computed and saved in result.
*
* @param result External or internal tensor to save the result.
*/
template <typename _Tensor>
template<typename _Result>
constexpr inline auto adjoint_wrapper<_Tensor>::evaluate_imp(_Result & result)noexcept{
    constexpr bool raw_data{std::experimental::is_detected<has_raw_data, data_type_tensor>::value};

    if(!this->_is_init){
        if constexpr (raw_data){
            evaluate::apply(_data);
            evaluate_details(result.raw_data(), _data_basis.raw_data());
        }else{
            result = _data_basis;
            evaluate_details(result.raw_data(), result.raw_data());
        }
        this->_is_init = true;
    }
}
//@}

/**
* @name Evaluation implementation
*/
//@{
/**
* @brief Computation of the adjoint of %_data_basis.
*
* @param result Pointer to the result memory.
* @param data Pointer to the data memory.
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::evaluate_details(value_type * result, value_type const* data)noexcept{
    if constexpr (dimension() == 1){
      result[0] = safe_cast<value_type>(1.) / data[0];
      return;
    }

    if constexpr (rank() == 2) {
        if constexpr (dimension() == 2) {
            adjoint_details_22(result, data[0], data[1],
                    data[2], data[3]);
        }else if constexpr (dimension() == 3) {
            adjoint_details_33(result, data[0], data[1], data[2],
                    data[3], data[4], data[5],
                    data[6], data[7], data[8]);
        }else{
            static_assert(dimension() == 2 || dimension() == 3, "adjoint_wrapper::evaluate_details(): no matching dimension");
        }
    }
}

/**
* @brief Adjoint computation of a second order tensor
* in 2D.
*
* @param result Pointer to the result memory.
* @param A11 Tensor coefficient
* @param A12 Tensor coefficient
* @param A21 Tensor coefficient
* @param A22 Tensor coefficient
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::adjoint_details_22(value_type * result,
                                                value_type const A11, value_type const A12,
                                                value_type const A21, value_type const A22)noexcept{
    result[0] =  A22;
    result[1] = -A12;
    result[2] = -A21;
    result[3] =  A11;
}

/**
* @brief Adjoint computation of a second-order tensor
* in 3D.
*
* @param result Pointer to the result memory.
* @param A11 Tensor coefficient
* @param A12 Tensor coefficient
* @param A13 Tensor coefficient
* @param A21 Tensor coefficient
* @param A22 Tensor coefficient
* @param A23 Tensor coefficient
* @param A31 Tensor coefficient
* @param A32 Tensor coefficient
* @param A33 Tensor coefficient
*/
template <typename _Tensor>
constexpr inline auto adjoint_wrapper<_Tensor>::adjoint_details_33(value_type * result,
                                                value_type const A11, value_type const A12, value_type const A13,
                                                value_type const A21, value_type const A22, value_type const A23,
                                                value_type const A31, value_type const A32, value_type const A33)noexcept{
    result[0] = (A22*A33-A23*A32);
    result[1] = (A13*A32-A12*A33);
    result[2] = (A12*A23-A13*A22);

    result[3] = (A23*A31-A21*A33);
    result[4] = (A11*A33-A13*A31);
    result[5] = (A13*A21-A11*A23);

    result[6] = (A21*A32-A22*A31);
    result[7] = (A12*A31-A11*A32);
    result[8] = (A11*A22-A12*A21);
}
//@}

} // NAMESPACE DETAIL

#endif // ADJOINT_WRAPPER_MEAT_H
