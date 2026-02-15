/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef INVERSE_WRAPPER_MEAT_H
#define INVERSE_WRAPPER_MEAT_H

namespace detail {

//==========================================================================
// inverse_wrapper_base — shared LU / direct inversion routines
//==========================================================================

template <typename _ValueType>
template <std::size_t Rows>
constexpr auto
inverse_wrapper_base<_ValueType>::lu_detail(value_type const *__A) noexcept {
  for (std::size_t i{0}; i < Rows; ++i) {
    const value_type Akk = static_cast<value_type>(1.0) / __A[i * Rows + i];
    for (std::size_t j{i + 1}; j < Rows; ++j) {
      const_cast<value_type &>(__A[j * Rows + i]) *= Akk;
    }

    for (std::size_t j{i + 1}; j < Rows; ++j) {
      for (std::size_t k{i + 1}; k < Rows; ++k) {
        const_cast<value_type &>(__A[j * Rows + k]) -=
            __A[j * Rows + i] * __A[i * Rows + k];
      }
    }
  }
}

template <typename _ValueType>
template <std::size_t Rows>
constexpr auto inverse_wrapper_base<_ValueType>::inv_lu(
    value_type *__Ainv, value_type const *const __Afac) noexcept {

  for (std::size_t i{0}; i < Rows * Rows; ++i) {
    __Ainv[i] = static_cast<value_type>(0);
  }

  for (std::size_t i{0}; i < Rows; ++i) {
    __Ainv[i * Rows + i] = static_cast<value_type>(1.0);
  }

  value_type temp_data[Rows];

  for (std::size_t j{0}; j < Rows; ++j) {
    for (std::size_t i{0}; i < Rows; ++i) {
      temp_data[i] = __Ainv[i * Rows + j];
    }
    for (std::size_t i{0}; i < Rows; ++i) {
      for (std::size_t k{0}; k < i; ++k) {
        temp_data[i] -= __Afac[i * Rows + k] * temp_data[k];
      }
    }
    for (std::size_t i{Rows}; i >= 1; --i) {
      const std::size_t ii{i - 1};
      for (std::size_t k{ii + 1}; k < Rows; ++k) {
        temp_data[ii] -= __Afac[ii * Rows + k] * temp_data[k];
      }
      temp_data[ii] = temp_data[ii] / __Afac[ii * Rows + ii];
    }
    for (std::size_t i{0}; i < Rows; ++i) {
      __Ainv[i * Rows + j] = temp_data[i];
    }
  }
}

template <typename _ValueType>
constexpr inline auto inverse_wrapper_base<_ValueType>::invert_2_2(
    value_type *result, value_type const A11, value_type const A12,
    value_type const A21, value_type const A22) noexcept {
  const auto det{A11 * A22 - A12 * A21};
  const auto invdet{static_cast<value_type>(1.0) / det};

  result[0] = A22 * invdet;
  result[1] = -A12 * invdet;
  result[2] = -A21 * invdet;
  result[3] = A11 * invdet;
}

template <typename _ValueType>
constexpr inline auto inverse_wrapper_base<_ValueType>::invert_3_3(
    value_type *result, value_type const A0, value_type const A1,
    value_type const A2, value_type const A3, value_type const A4,
    value_type const A5, value_type const A6, value_type const A7,
    value_type const A8) noexcept {
  const auto det{A0 * (A4 * A8 - A5 * A7) + A1 * (A5 * A6 - A3 * A8) +
                 A2 * (A3 * A7 - A4 * A6)};

  const auto invdet{static_cast<value_type>(1.0) / det};

  result[0] = (A4 * A8 - A5 * A7) * invdet;
  result[1] = (A2 * A7 - A1 * A8) * invdet;
  result[2] = (A1 * A5 - A2 * A4) * invdet;

  result[3] = (A5 * A6 - A3 * A8) * invdet;
  result[4] = (A0 * A8 - A2 * A6) * invdet;
  result[5] = (A2 * A3 - A0 * A5) * invdet;

  result[6] = (A3 * A7 - A4 * A6) * invdet;
  result[7] = (A1 * A6 - A0 * A7) * invdet;
  result[8] = (A0 * A4 - A1 * A3) * invdet;
}

//==========================================================================
// PRIMARY TEMPLATE: inverse_wrapper<_Tensor, _Sequences...>
// Identical to original — _Sequences... guaranteed non-empty.
//==========================================================================

template <typename _Tensor, typename ..._Sequences>
constexpr inverse_wrapper<_Tensor, _Sequences...>::inverse_wrapper(_Tensor __data)noexcept:
    data(),
    tensor_data(__data)
{}

template <typename _Tensor, typename... _Sequences>
constexpr inverse_wrapper<_Tensor, _Sequences...>::inverse_wrapper(
    inverse_wrapper const &__data) noexcept
    : basetype(__data), data(), tensor_data(__data.tensor_data) {}

template <typename _Tensor, typename ..._Sequences>
template<typename ...Indicies>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::operator ()(Indicies... __indices)const noexcept{
    return data(__indices...);
}

template <typename _Tensor, typename ..._Sequences>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::dimension()noexcept{
    return data_type_tensor::dimension();
}

template <typename _Tensor, typename ..._Sequences>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::rank()noexcept{
    return data_type_tensor::rank();
}

template <typename _Tensor, typename ..._Sequences>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::raw_data()const noexcept{
    return data.raw_data();
}

template <typename _Tensor, typename ..._Sequences>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::evaluate()noexcept{
    if(!this->_is_init){
        evaluate_imp(data);
        this->_is_init = true;
    }
}

template <typename _Tensor, typename ..._Sequences>
template<typename _Result>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::evaluate(_Result const& __result)noexcept{
    evaluate_imp(__result);
}

template <typename _Tensor, typename ..._Sequences>
template<typename _Result>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::evaluate_imp(_Result const& __result)noexcept{
    if constexpr (rank() == 4){
        if constexpr (std::tuple_size_v<_Tuple> == 2){
            voigt_rank_4(__result);
        }else{
            //full
            //convert to <1,2,3,4>
            tmech::tensor<value_type, dimension(), rank()> data_local;
            if constexpr (std::tuple_size_v<_Tuple> == 1){
                using sequence = std::tuple_element_t<0, _Tuple>;
                if constexpr (std::is_same_v<tmech::sequence<1,2,3,4>, sequence>){
                    data_local = tensor_data;
                }else{
                    data_local = basis_change<sequence>(tensor_data);
                }
            }else{
                data_local = tensor_data;
            }

            //determine inv
            evaluate_imp(__result.raw_data(), data_local.raw_data());

            if constexpr (std::tuple_size_v<_Tuple> == 1){
                using sequence = std::tuple_element_t<0, _Tuple>;
                if constexpr (!std::is_same_v<tmech::sequence<1,2,3,4>, sequence>){
                    const_cast<_Result&>(__result).template change_basis_view<sequence>() = eval(__result);
                }
            }
        }
    }else if constexpr (rank() == 2){
        constexpr auto HasRawData{is_detected<has_raw_data, data_type_tensor>::value};
        if constexpr (HasRawData){
            evaluate::apply(tensor_data);
            evaluate_imp(__result.raw_data(), tensor_data.raw_data());
        }else{
            data = tensor_data;
            evaluate_imp(__result.raw_data(), data.raw_data());
        }
    }
}

template <typename _Tensor, typename ..._Sequences>
template<typename _Result>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::voigt_rank_4(_Result const& __result)noexcept{
    using tuple1 = std::tuple_element_t<0, _Tuple>;
    using tuple2 = std::tuple_element_t<1, _Tuple>;
    using sequence = decltype(tmech::detail::make_single_sequence(tuple1(), tuple2()));

    constexpr auto SIZE{(dimension() == 2 ? 3 : 6)};
    value_type _ptr[SIZE*SIZE], _ptr_inv[SIZE*SIZE]{0};
    //convert to voigt
    convert_tensor_to_voigt<std::tuple<tuple1, tuple2>>(tensor_data, _ptr);

    //last three columns due to symmetry
    for (std::size_t i{0}; i < SIZE; ++i) {
      for (std::size_t j{dimension()}; j < SIZE; ++j) {
        _ptr[i * SIZE + j] *= static_cast<value_type>(2);
      }
    }

    //determine inv
    evaluate_imp(_ptr_inv, _ptr);

    //convert back last three columns due to symmetry
    for(std::size_t i{0}; i<SIZE; ++i){
        for(std::size_t j{dimension()}; j<SIZE; ++j){
          _ptr_inv[i * SIZE + j] *= static_cast<value_type>(0.5);
        }
    }

    //convert back to a tensor
    const adaptor<value_type, dimension(), 4, voigt<dimension(), false>> adap(_ptr_inv);
    if constexpr (std::is_same_v<tmech::sequence<1,2,3,4>, sequence>){
        const_cast<_Result&>(__result) = adap;
    }else{
        const_cast<_Result&>(__result).template change_basis_view<sequence>() = adap;
    }
}

template <typename _Tensor, typename ..._Sequences>
constexpr inline auto inverse_wrapper<_Tensor, _Sequences...>::evaluate_imp(value_type const* __result, value_type const * __data)noexcept{
    constexpr auto DIM{dimension()};
    constexpr auto RANK{rank()};

    if constexpr (DIM == 1){
        const_cast<value_type&>(__result[0]) = static_cast<value_type>(1.)/__data[0];
        return ;
    }

    if constexpr (RANK == 2) {
        if constexpr (DIM == 2) {
          inv_base::invert_2_2(const_cast<value_type *>(__result), __data[0],
                               __data[1], __data[2], __data[3]);
          return;
        }
        if constexpr (DIM == 3) {
          inv_base::invert_3_3(const_cast<value_type *>(__result), __data[0],
                               __data[1], __data[2], __data[3], __data[4],
                               __data[5], __data[6], __data[7], __data[8]);
          return;
        }
    }

    if constexpr (RANK == 4){
        if constexpr (std::tuple_size_v<_Tuple> != 2){
            //full
            inv_base::template lu_detail<DIM * DIM>(__data);
            inv_base::template inv_lu<DIM * DIM>(
                const_cast<value_type *>(__result), __data);
        }else{
          // minor symmetric
          constexpr size_type SIZE{(dimension() == 2 ? 3 : 6)};
          inv_base::template lu_detail<SIZE>(__data);
          inv_base::template inv_lu<SIZE>(const_cast<value_type *>(__result),
                                          __data);
        }
    }
}

//==========================================================================
// PARTIAL SPECIALIZATION: inverse_wrapper<_Tensor> (no sequences)
// No std::tuple, no std::tuple_element_t anywhere.
//==========================================================================

template <typename _Tensor>
constexpr inverse_wrapper<_Tensor>::inverse_wrapper(_Tensor __data) noexcept
    : data(), tensor_data(__data) {}

template <typename _Tensor>
constexpr inverse_wrapper<_Tensor>::inverse_wrapper(
    inverse_wrapper const &__data) noexcept
    : basetype(__data), data(), tensor_data(__data.tensor_data) {}

template <typename _Tensor>
template <typename... Indicies>
constexpr inline auto
inverse_wrapper<_Tensor>::operator()(Indicies... __indices) const noexcept {
  return data(__indices...);
}

template <typename _Tensor>
constexpr inline auto inverse_wrapper<_Tensor>::dimension() noexcept {
  return data_type_tensor::dimension();
}

template <typename _Tensor>
constexpr inline auto inverse_wrapper<_Tensor>::rank() noexcept {
  return data_type_tensor::rank();
}

template <typename _Tensor>
constexpr inline auto inverse_wrapper<_Tensor>::raw_data() const noexcept {
  return data.raw_data();
}

template <typename _Tensor>
constexpr inline auto inverse_wrapper<_Tensor>::evaluate() noexcept {
  if (!this->_is_init) {
    evaluate_imp(data);
    this->_is_init = true;
  }
}

template <typename _Tensor>
template <typename _Result>
constexpr inline auto
inverse_wrapper<_Tensor>::evaluate(_Result const &__result) noexcept {
  evaluate_imp(__result);
}

template <typename _Tensor>
template <typename _Result>
constexpr inline auto
inverse_wrapper<_Tensor>::evaluate_imp(_Result const &__result) noexcept {
  if constexpr (rank() == 4) {
    // Full rank-4 inverse (no Voigt, no basis change)
    tmech::tensor<value_type, dimension(), rank()> data_local;
    data_local = tensor_data;
    evaluate_imp(__result.raw_data(), data_local.raw_data());
  } else if constexpr (rank() == 2) {
    constexpr auto HasRawData{
        is_detected<has_raw_data, data_type_tensor>::value};
    if constexpr (HasRawData) {
      evaluate::apply(tensor_data);
      evaluate_imp(__result.raw_data(), tensor_data.raw_data());
    } else {
      data = tensor_data;
      evaluate_imp(__result.raw_data(), data.raw_data());
    }
  }
}

template <typename _Tensor>
constexpr inline auto
inverse_wrapper<_Tensor>::evaluate_imp(value_type const *__result,
                                       value_type const *__data) noexcept {
  constexpr auto DIM{dimension()};
  constexpr auto RANK{rank()};

  if constexpr (DIM == 1) {
    const_cast<value_type &>(__result[0]) =
        static_cast<value_type>(1.) / __data[0];
    return;
  }

  if constexpr (RANK == 2) {
    if constexpr (DIM == 2) {
      inv_base::invert_2_2(const_cast<value_type *>(__result), __data[0],
                           __data[1], __data[2], __data[3]);
      return;
    }
    if constexpr (DIM == 3) {
      inv_base::invert_3_3(const_cast<value_type *>(__result), __data[0],
                           __data[1], __data[2], __data[3], __data[4],
                           __data[5], __data[6], __data[7], __data[8]);
      return;
    }
  }

  if constexpr (RANK == 4) {
    // Always full LU — no sequences means no Voigt
    inv_base::template lu_detail<DIM * DIM>(__data);
    inv_base::template inv_lu<DIM * DIM>(const_cast<value_type *>(__result),
                                         __data);
  }
}

} // NAMESPACE DETAIL

#endif // INVERSE_WRAPPER_MEAT_H
