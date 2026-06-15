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

// LU decomposition with partial (row) pivoting. The permutation
// vector __P stores the row that landed at position i after swaps —
// so __P[i] is the source row in the original matrix. Doolittle
// layout: the lower triangle of __A stores L (unit diagonal implicit),
// upper triangle stores U.
//
// Pivoting addresses two distinct failure modes (see issue #14):
//
//   (a) Platform-divergent fuzz on fuzz-generated inputs: without
//       pivoting, the accumulation order during elimination is at the
//       mercy of small leading pivots, and the resulting FP rounding
//       chain differs between Linux/macOS/Windows. The inverse is then
//       valid on one platform and garbage on another.
//
//   (b) Conditioning of well-conditioned matrices that happen to have
//       a small Voigt[0,0] entry: e.g. 100·IIsym + 50·I⊗I with the
//       (0,0,0,0) entry dropped to 1e-9. No-pivot LU multiplies row k
//       by Voigt[k,0]/ε ≈ 5e10, then subtracts, losing ~3 digits in
//       the resulting Schur entries — propagating to ~1e-3 error in
//       the inverse. With pivoting the largest |Voigt[k,0]| row is
//       swapped in first and the multiplier collapses to O(1).
//
// Singular matrix: if no nonzero pivot exists in column i from row i
// down, piv stays at i and the division by __A[i*Rows+i] below
// produces +Inf / NaN. Callers must ensure the matrix is invertible;
// there is no in-band error signal.
template <typename _ValueType>
template <std::size_t Rows>
constexpr auto inverse_wrapper_base<_ValueType>::lu_detail(
    value_type const *__A, std::size_t *__P) noexcept {
  for (std::size_t i{0}; i < Rows; ++i) {
    __P[i] = i;
  }
  for (std::size_t i{0}; i < Rows; ++i) {
    // Find the row k in i..Rows-1 with the largest |__A[k,i]| and
    // swap it into position i.
    std::size_t piv = i;
    value_type maxv =
        __A[i * Rows + i] >= safe_cast<value_type>(0)
            ? __A[i * Rows + i]
            : -__A[i * Rows + i];
    for (std::size_t k{i + 1}; k < Rows; ++k) {
      value_type v = __A[k * Rows + i] >= safe_cast<value_type>(0)
                         ? __A[k * Rows + i]
                         : -__A[k * Rows + i];
      if (v > maxv) {
        maxv = v;
        piv = k;
      }
    }
    if (piv != i) {
      for (std::size_t k{0}; k < Rows; ++k) {
        value_type t = __A[i * Rows + k];
        const_cast<value_type &>(__A[i * Rows + k]) = __A[piv * Rows + k];
        const_cast<value_type &>(__A[piv * Rows + k]) = t;
      }
      std::size_t pt = __P[i];
      __P[i] = __P[piv];
      __P[piv] = pt;
    }

    const value_type Akk = safe_cast<value_type>(1.0) / __A[i * Rows + i];
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

// Solve A · X = I for X = inv(A), where PA = LU with __P from
// lu_detail. The j-th column of X is the solution to LU · x = P · e_j,
// which is the unit vector e_k where k = Pinv[j] (the row that
// landed at position j after the permutation).
template <typename _ValueType>
template <std::size_t Rows>
constexpr auto inverse_wrapper_base<_ValueType>::inv_lu(
    value_type *__Ainv, value_type const *const __Afac,
    std::size_t const *__P) noexcept {
  // Inverse permutation: Pinv[__P[i]] = i.
  std::size_t Pinv[Rows];
  for (std::size_t i{0}; i < Rows; ++i) {
    Pinv[__P[i]] = i;
  }

  // No __Ainv zero-init: the column-by-column loop below writes
  // every entry of __Ainv exactly once (via the final store loop
  // over i for fixed j), so any prior content of __Ainv is dead.
  value_type temp_data[Rows];

  for (std::size_t j{0}; j < Rows; ++j) {
    // Permuted identity column j: 1 at row Pinv[j], 0 elsewhere.
    for (std::size_t i{0}; i < Rows; ++i) {
      temp_data[i] = safe_cast<value_type>(0);
    }
    temp_data[Pinv[j]] = safe_cast<value_type>(1.0);

    // Forward substitution: L · y = b.
    for (std::size_t i{0}; i < Rows; ++i) {
      for (std::size_t k{0}; k < i; ++k) {
        temp_data[i] -= __Afac[i * Rows + k] * temp_data[k];
      }
    }
    // Back substitution: U · x = y.
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
  const auto invdet{safe_cast<value_type>(1.0) / det};

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

  const auto invdet{safe_cast<value_type>(1.0) / det};

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
    evaluate_imp(data);
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

    // Bring the indices specified by (tuple1, tuple2) into the natural
    // <1,2,3,4> order via a basis_change so the convert_tensor_to_voigt
    // call below always operates on a tensor whose indices 1,2 are the
    // row pair and 3,4 are the col pair. The unpack at the end applies
    // the inverse permutation via change_basis_view. This makes custom
    // <SeqL, SeqR> work correctly for Major-asymmetric inputs; without
    // the pre-pack basis_change the index_tuple in convert silently
    // collapsed wrong permutations for non-default tuples.
    if constexpr (std::is_same_v<tmech::sequence<1,2,3,4>, sequence>){
        convert_tensor_to_voigt<std::tuple<tmech::sequence<1,2>, tmech::sequence<3,4>>>(tensor_data, _ptr);
    }else{
        tmech::tensor<value_type, dimension(), rank()> data_local;
        data_local = basis_change<sequence>(tensor_data);
        convert_tensor_to_voigt<std::tuple<tmech::sequence<1,2>, tmech::sequence<3,4>>>(data_local, _ptr);
    }

    //last three columns due to symmetry
    for (std::size_t i{0}; i < SIZE; ++i) {
      for (std::size_t j{dimension()}; j < SIZE; ++j) {
        _ptr[i * SIZE + j] *= safe_cast<value_type>(2);
      }
    }

    //determine inv
    evaluate_imp(_ptr_inv, _ptr);

    //convert back last three columns due to symmetry
    for(std::size_t i{0}; i<SIZE; ++i){
        for(std::size_t j{dimension()}; j<SIZE; ++j){
          _ptr_inv[i * SIZE + j] *= safe_cast<value_type>(0.5);
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
      const_cast<value_type &>(__result[0]) =
          safe_cast<value_type>(1.) / __data[0];
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

    if constexpr (RANK == 4){
        if constexpr (std::tuple_size_v<_Tuple> != 2){
            //full
            std::size_t P_perm[DIM * DIM];
            inv_base::template lu_detail<DIM * DIM>(__data, P_perm);
            inv_base::template inv_lu<DIM * DIM>(
                const_cast<value_type *>(__result), __data, P_perm);
        }else{
          // minor symmetric
          constexpr size_type SIZE{(dimension() == 2 ? 3 : 6)};
          std::size_t P_perm[SIZE];
          inv_base::template lu_detail<SIZE>(__data, P_perm);
          inv_base::template inv_lu<SIZE>(const_cast<value_type *>(__result),
                                          __data, P_perm);
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
  evaluate_imp(data);
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
        safe_cast<value_type>(1.) / __data[0];
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
    std::size_t P_perm[DIM * DIM];
    inv_base::template lu_detail<DIM * DIM>(__data, P_perm);
    inv_base::template inv_lu<DIM * DIM>(const_cast<value_type *>(__result),
                                         __data, P_perm);
  }
}

} // NAMESPACE DETAIL

#endif // INVERSE_WRAPPER_MEAT_H
