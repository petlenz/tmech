/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef COFACTOR_WRAPPER_MEAT_H
#define COFACTOR_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _Tensor>
cofactor_wrapper<_Tensor>::cofactor_wrapper(data_type_tensor const& data)noexcept:
    _data(),
    _data_basis(data)
{}
/**
 * Copy constructor
 */
template <typename _Tensor>
cofactor_wrapper<_Tensor>::cofactor_wrapper(cofactor_wrapper const& data)noexcept:
    basetype(data),
    _data(),
    _data_basis(data._data_basis)
{}
//@}


template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::operator()(size_type const i, size_type const j)const noexcept{
    return _data(i,j);
}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::dimension()noexcept{
    return data_type_tensor::dimension();
}

/**
 * Returns the rank.
 */
template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::rank()noexcept{
    return data_type_tensor::rank();
}
//@}


template <typename _Tensor>
template<typename _Result>
constexpr inline auto cofactor_wrapper<_Tensor>::evaluate(_Result & result)noexcept{
    this->_is_init = false;
    evaluate_imp(result);
}

template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::evaluate()noexcept{
    evaluate_imp(_data);
}

template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::raw_data()const noexcept{
    return _data.raw_data();
}

template <typename _Tensor>
template<typename _Result>
constexpr inline auto cofactor_wrapper<_Tensor>::evaluate_imp(_Result & result)noexcept{
    if(!this->_is_init){
        if constexpr(is_detected<has_raw_data, data_type_tensor>::value){
            evaluate::apply(_data_basis);
            evaluate_details(result.raw_data(), _data_basis.raw_data());
        }else{
            result = _data_basis;
            evaluate_details(result.raw_data(), result.raw_data());
        }
        this->_is_init = true;
    }
}

template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::evaluate_details(value_type * result, value_type const*const data)noexcept{
    if constexpr (dimension() == 1){
        result[0] = static_cast<value_type>(1.)/data[0];
        return ;
    }

    if constexpr (rank() == 2) {
        if constexpr (dimension() == 2) {
            cofactors_details_22(result, data[0], data[1],
                    data[2], data[3]);
        }else if constexpr (dimension() == 3) {
            cofactors_details_33(result, data[0], data[1], data[2],
                    data[3], data[4], data[5],
                    data[6], data[7], data[8]);
        }else{
            assert(0 && "cofactor_wrapper: no matching dimension");
        }
    }
}

template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::cofactors_details_22(value_type * result,
                                                                     value_type const A11, value_type const A12,
                                                                     value_type const A21, value_type const A22)noexcept{
    //cof(A) = adj(A)^T
    result[0] =  A22;
    result[1] = -A21;
    result[2] = -A12;
    result[3] =  A11;
}

template <typename _Tensor>
constexpr inline auto cofactor_wrapper<_Tensor>::cofactors_details_33(value_type * result,
                                                  value_type const A0, value_type const A1, value_type const A2,
                                                  value_type const A3, value_type const A4, value_type const A5,
                                                  value_type const A6, value_type const A7, value_type const A8)noexcept{
    //0 1 2
    //3 4 5
    //6 7 8
    //1 -> 3
    //2 -> 6
    //5 -> 7
    //cof(A) = adj(A)^T
    result[0] = (A4*A8-A5*A7);
    result[1] = (A5*A6-A3*A8);
    result[2] = (A3*A7-A4*A6);

    result[3] = (A2*A7-A1*A8);
    result[4] = (A0*A8-A2*A6);
    result[5] = (A1*A6-A0*A7);

    result[6] = (A1*A5-A2*A4);
    result[7] = (A2*A3-A0*A5);
    result[8] = (A0*A4-A1*A3);
}

} // NAMESPACE DETAIL
#endif // COFACTOR_WRAPPER_MEAT_H
