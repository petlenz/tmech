/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EIGEN_DECOMPOSITION_MEAT_H
#define EIGEN_DECOMPOSITION_MEAT_H

namespace detail {

#ifndef TMECH_LAPACK_GEEV_DECLARED
#define TMECH_LAPACK_GEEV_DECLARED
extern "C" {
extern int dgeev_(char *, char *, int *, double *, int *, double *, double *,
                  double *, int *, double *, int *, double *, int *, int *);
extern int sgeev_(char *, char *, int *, float *, int *, float *, float *,
                  float *, int *, float *, int *, float *, int *, int *);
}
#endif // TMECH_LAPACK_GEEV_DECLARED

//==========================================================================
//
//  eigen_decomposition_base — shared implementation
//
//==========================================================================

template <typename _Tensor, typename _Derived>
eigen_decomposition_base<_Tensor, _Derived>::eigen_decomposition_base(
    _Tensor const &data) noexcept
    : _data(), _eigval(), _eigvectors(), _eig_basis(),
      _non_repeated_eigenvalues({0}), _permut(),
      _all_repeated_eigenvalues(false), _pair_repeated_eigenvalues(false),
      _is_init(false), _data_base(data) {}

template <typename _Tensor, typename _Derived>
eigen_decomposition_base<_Tensor, _Derived>::eigen_decomposition_base(
    eigen_decomposition_base const &data) noexcept
    : _data(), _eigval(), _eigvectors(), _eig_basis(),
      _non_repeated_eigenvalues({0}), _permut(),
      _all_repeated_eigenvalues(false), _pair_repeated_eigenvalues(false),
      _is_init(false), _data_base(data._data_base) {}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::decompose() noexcept {
  std::iota(_permut.begin(), _permut.end(), 0);
  _data = _data_base;
  static_cast<_Derived *>(this)->compute_decomposition(true);
  return std::make_tuple(eigenvalues(), eigenvectors());
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::decompose_eigenbasis() noexcept {
  if (!_is_init) {
    _all_repeated_eigenvalues = false;
    _pair_repeated_eigenvalues = false;
    _data = _data_base;
    std::iota(_non_repeated_eigenvalues.begin(),
              _non_repeated_eigenvalues.end(), 0);
    static_cast<_Derived *>(this)->compute_decomposition(false);
    get_repeated_eigenvalues();
    evaluate_eigenbasis();
    _is_init = true;
  }
  return std::make_tuple(eigenvalues(), eigenbasis());
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::eigenvalues() const noexcept {
  return eigen_decomposition_values_wrapper<_Derived>(
      *static_cast<const _Derived *>(this));
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::eigenvectors() const noexcept {
  return eigen_decomposition_vectors_wrapper<_Derived>(
      *static_cast<const _Derived *>(this));
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::eigenbasis() const noexcept {
  return eigen_decomposition_basis_wrapper<_Derived>(
      *static_cast<const _Derived *>(this));
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::all_eigenvalues_repeated()
    const noexcept {
  return _all_repeated_eigenvalues;
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::pair_eigenvalues_repeated()
    const noexcept {
  return _pair_repeated_eigenvalues;
}

template <typename _Tensor, typename _Derived>
constexpr inline typename eigen_decomposition_base<_Tensor, _Derived>::size_type
eigen_decomposition_base<_Tensor, _Derived>::number_non_repeated_eigenvalues()
    const noexcept {
  if constexpr (Dim == 2) {
    return (all_eigenvalues_repeated() ? 1 : 2);
  } else if constexpr (Dim == 3) {
    return (all_eigenvalues_repeated()    ? 1
            : pair_eigenvalues_repeated() ? 2
                                          : 3);
  }
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::permutation() const noexcept {
  return _permut;
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::non_repeated_eigenvalues_index()
    const noexcept {
  switch (number_non_repeated_eigenvalues()) {
  case 1:
    return std::vector<size_type>{_non_repeated_eigenvalues[0]};
  case 2:
    return std::vector<size_type>{_non_repeated_eigenvalues[0],
                                  _non_repeated_eigenvalues[1]};
  case 3:
    return std::vector<size_type>{_non_repeated_eigenvalues[0],
                                  _non_repeated_eigenvalues[1],
                                  _non_repeated_eigenvalues[2]};
  default:
    return std::vector<size_type>(0);
  }
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor,
                         _Derived>::get_repeated_eigenvalues() noexcept {
  if constexpr (Dim == 2) {
    const auto x1{_eigval[0]}, x2{_eigval[1]};
    const auto max_val{std::max(safe_cast<value_type>(1.0), std::max(x1, x2))};
    const auto eps{std::numeric_limits<value_type>::epsilon() * max_val};
    if (std::fabs(x1 - x2) <= eps) {
      _all_repeated_eigenvalues = true;
    }
  } else if constexpr (Dim == 3) {
    const auto x1{_eigval[0]}, x2{_eigval[1]}, x3{_eigval[2]};
    const auto max_val{
        std::max(safe_cast<value_type>(1.0), std::max(x1, std::max(x2, x3)))};
    const auto eps{std::numeric_limits<value_type>::epsilon() * max_val};
    const auto equal_12{std::fabs(x1 - x2) <= eps};
    const auto equal_13{std::fabs(x1 - x3) <= eps};
    const auto equal_23{std::fabs(x2 - x3) <= eps};
    _all_repeated_eigenvalues = false;
    _pair_repeated_eigenvalues = false;
    if (equal_12 && equal_13 && equal_23) {
      _all_repeated_eigenvalues = true;
      return;
    }
    // Non-repeated eigenvalue always first entry
    if (equal_12) {
      _non_repeated_eigenvalues[0] = 2;
      _non_repeated_eigenvalues[1] = 0;
      _pair_repeated_eigenvalues = true;
    } else if (equal_13) {
      _non_repeated_eigenvalues[0] = 1;
      _non_repeated_eigenvalues[1] = 0;
      _pair_repeated_eigenvalues = true;
    } else if (equal_23) {
      _non_repeated_eigenvalues[0] = 0;
      _non_repeated_eigenvalues[1] = 1;
      _pair_repeated_eigenvalues = true;
    }
  }
}

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::evaluate_eigenbasis() noexcept {
  const tmech::eye<value_type, Dim, 2> I;
  if constexpr (Dim == 2) {
    if (_all_repeated_eigenvalues) {
      _eig_basis[0] = I;
    } else {
      _eig_basis[0] = (_data - _eigval[1] * I) / (_eigval[0] - _eigval[1]);
      _eig_basis[1] = (_data - _eigval[0] * I) / (_eigval[1] - _eigval[0]);
    }
  } else if constexpr (Dim == 3) {
    if (_all_repeated_eigenvalues) {
      _eig_basis[0] = I;
    } else if (_pair_repeated_eigenvalues) {
      _eig_basis[_non_repeated_eigenvalues[0]] =
          (_data - _eigval[_non_repeated_eigenvalues[1]] * I) /
          (_eigval[_non_repeated_eigenvalues[0]] -
           _eigval[_non_repeated_eigenvalues[1]]);
      _eig_basis[_non_repeated_eigenvalues[1]] =
          I - _eig_basis[_non_repeated_eigenvalues[0]];
    } else {
      _eig_basis[0] = (_data - _eigval[1] * I) * (_data - _eigval[2] * I) /
                      ((_eigval[0] - _eigval[1]) * (_eigval[0] - _eigval[2]));
      _eig_basis[1] = (_data - _eigval[0] * I) * (_data - _eigval[2] * I) /
                      ((_eigval[1] - _eigval[0]) * (_eigval[1] - _eigval[2]));
      _eig_basis[2] = (_data - _eigval[0] * I) * (_data - _eigval[1] * I) /
                      ((_eigval[2] - _eigval[0]) * (_eigval[2] - _eigval[1]));
    }
  }
}

// --- Static helpers ---

template <typename _Tensor, typename _Derived>
constexpr inline auto eigen_decomposition_base<_Tensor, _Derived>::mul(
    value_type *data, value_type const *const lhs,
    value_type const *const rhs) noexcept {
  const value_type r0{rhs[0]}, r1{rhs[1]}, r2{rhs[2]};
  data[0] = lhs[0] * r0 + lhs[1] * r1 + lhs[2] * r2;
  data[1] = lhs[3] * r0 + lhs[4] * r1 + lhs[5] * r2;
  data[2] = lhs[6] * r0 + lhs[7] * r1 + lhs[8] * r2;
}

template <typename _Tensor, typename _Derived>
constexpr inline auto eigen_decomposition_base<_Tensor, _Derived>::dot(
    value_type const *const lhs, value_type const *const rhs) noexcept {
  return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template <typename _Tensor, typename _Derived>
constexpr inline auto eigen_decomposition_base<_Tensor, _Derived>::cross(
    value_type *data, value_type const *const lhs,
    value_type const *const rhs) noexcept {
  data[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
  data[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
  data[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

// --- sort_eigenvalues (3D only, called by derived classes) ---

template <typename _Tensor, typename _Derived>
constexpr inline auto
eigen_decomposition_base<_Tensor, _Derived>::sort_eigenvalues(
    int sortType, bool isRotation, bool eigenvectors) noexcept {
  if (sortType != 0) {
    std::array<size_type, 3> index{0, 1, 2};
    if (_eigval[index[0]] > _eigval[index[1]])
      std::swap(index[0], index[1]);
    if (_eigval[index[0]] > _eigval[index[2]])
      std::swap(index[0], index[2]);
    if (_eigval[index[1]] > _eigval[index[2]])
      std::swap(index[1], index[2]);

    // Odd permutation check
    if ((index[0] == 0 && index[1] == 2 && index[2] == 1) ||
        (index[0] == 1 && index[1] == 0 && index[2] == 2) ||
        (index[0] == 2 && index[1] == 1 && index[2] == 0))
      isRotation = !isRotation;

    if (sortType == -1) {
      // Descending: eval[0] >= eval[1] >= eval[2]
      std::swap(index[0], index[2]);
      isRotation = !isRotation;
    }

    const std::array<value_type, 3> unorderedEVal{_eigval};
    for (size_type j = 0; j < 3; ++j) {
      _eigval[j] = unorderedEVal[index[j]];
      _permut[j] = index[j];
    }

    if (eigenvectors) {
      const std::array<tensor1, 3> unorderedEVec{_eigvectors};
      for (size_type j = 0; j < 3; ++j) {
        _eigvectors[j] = unorderedEVec[index[j]];
      }
    }
  }
  if (eigenvectors) {
    // Ensure ordered eigenvectors form a right-handed basis
    if (!isRotation) {
      for (size_type j = 0; j < 3; ++j) {
        _eigvectors[2](j) = -_eigvectors[2](j);
      }
    }
  }
}

//==========================================================================
//
//  eigen_decomposition_inline — analytical implementation
//
//==========================================================================

template <typename _Tensor>
constexpr inline auto
eigen_decomposition_inline<_Tensor>::compute_decomposition(
    bool eigenvectors) noexcept {
  if constexpr (Dim == 2) {
    evaluate_detail_2D(eigenvectors, this->_data(0, 0), this->_data(0, 1),
                       this->_data(1, 0), this->_data(1, 1));
  } else if constexpr (Dim == 3) {
    evaluate_detail_3D(eigenvectors, this->_data(0, 0), this->_data(0, 1),
                       this->_data(0, 2), this->_data(1, 1), this->_data(1, 2),
                       this->_data(2, 2));
  }
}

template <typename _Tensor>
constexpr inline auto eigen_decomposition_inline<_Tensor>::evaluate_detail_2D(
    bool eigenvectors, value_type const _a00, value_type const _a01,
    value_type const _a10, value_type const _a11) noexcept {

  const value_type max_element{
      std::max(_a00, std::max(_a01, std::max(_a10, _a11)))};
  const value_type inv_max_element{safe_cast<value_type>(1) / max_element};
  const value_type a00{_a00 * inv_max_element};
  const value_type a10{_a10 * inv_max_element};
  const value_type a01{_a01 * inv_max_element};
  const value_type a11{_a11 * inv_max_element};
  const value_type eps_zero{std::numeric_limits<value_type>::epsilon() *
                            std::max(safe_cast<value_type>(1), max_element)};

  if (std::abs(max_element) <= eps_zero) {
    // Zero matrix
    this->_eigval[0] = 0;
    this->_eigval[1] = 0;
    if (eigenvectors) {
      this->_eigvectors[0](0) = 1;
      this->_eigvectors[1](1) = 1;
    }
    return;
  } else if ((a10 * a01) <= eps_zero) {
    // Diagonal matrix
    this->_eigval[0] = a00;
    this->_eigval[1] = a11;
    if (eigenvectors) {
      this->_eigvectors[0](0) = 1;
      this->_eigvectors[0](1) = 0;
      this->_eigvectors[1](0) = 0;
      this->_eigvectors[1](1) = 1;
    }
    return;
  }

  const auto I1{a00 + a11};
  const auto I2{a00 * a11 - a01 * a10};
  const auto dis{I1 * I1 - safe_cast<value_type>(4) * I2};

  if (dis <= eps_zero) {
    // Repeated eigenvalues
    this->_eigval[0] = I1 * safe_cast<value_type>(0.5);
    this->_eigval[1] = I1 * safe_cast<value_type>(0.5);
    if (eigenvectors) {
      this->_eigvectors[0](0) = 1;
      this->_eigvectors[0](1) = 0;
      this->_eigvectors[1](0) = 0;
      this->_eigvectors[1](1) = 1;
    }
  } else {
    const auto sqrt_dis{std::sqrt(dis)};
    this->_eigval[0] = (I1 + sqrt_dis) * safe_cast<value_type>(0.5);
    this->_eigval[1] = (I1 - sqrt_dis) * safe_cast<value_type>(0.5);
  }

  this->_eigval[0] *= max_element;
  this->_eigval[1] *= max_element;

  if (this->_eigval[0] > this->_eigval[1]) {
    std::swap(this->_eigval[0], this->_eigval[1]);
    std::swap(this->_permut[0], this->_permut[1]);
  }

  if (eigenvectors) {
    for (size_type i{0}; i < 2; ++i) {
      const value_type temp{safe_cast<value_type>(
          std::sqrt(safe_cast<value_type>(1) +
                    std::pow((this->_eigval[i] - _a00) / _a01, 2)))};
      this->_eigvectors[i](0) = safe_cast<value_type>(1) / temp;
      this->_eigvectors[i](1) = (this->_eigval[i] - _a00) / (temp * _a01);
    }
  }
}

template <typename _Tensor>
constexpr inline auto eigen_decomposition_inline<_Tensor>::evaluate_detail_3D(
    bool eigenvectors, value_type const _a00, value_type const _a01,
    value_type const _a02, value_type const _a11, value_type const _a12,
    value_type const _a22) noexcept {

  // Precondition: factor out maximum absolute value to guard
  // against floating-point overflow in eigenvalue computation.
  const value_type max0{std::max(std::fabs(_a00), std::fabs(_a01))};
  const value_type max1{std::max(std::fabs(_a02), std::fabs(_a11))};
  const value_type max2{std::max(std::fabs(_a12), std::fabs(_a22))};
  const value_type maxAbsElement{std::max(std::max(max0, max1), max2)};

  if (maxAbsElement == 0.) {
    // Zero matrix
    this->_eigval[0] = 0;
    this->_eigval[1] = 0;
    this->_eigval[2] = 0;
    if (eigenvectors) {
      this->_eigvectors[0](0) = 1;
      this->_eigvectors[1](1) = 1;
      this->_eigvectors[2](2) = 1;
    }
    return;
  }

  const value_type invMaxAbsElement{safe_cast<value_type>(1) / maxAbsElement};
  const value_type a00 = _a00 * invMaxAbsElement;
  const value_type a01 = _a01 * invMaxAbsElement;
  const value_type a02 = _a02 * invMaxAbsElement;
  const value_type a11 = _a11 * invMaxAbsElement;
  const value_type a12 = _a12 * invMaxAbsElement;
  const value_type a22 = _a22 * invMaxAbsElement;

  const value_type norm{a01 * a01 + a02 * a02 + a12 * a12};

  if (norm > safe_cast<value_type>(0)) {
    // Non-diagonal: compute eigenvalues via cubic characteristic equation
    const value_type q{(a00 + a11 + a22) / safe_cast<value_type>(3)};

    const value_type b00{a00 - q};
    const value_type b11{a11 - q};
    const value_type b22{a22 - q};

    const value_type p{std::sqrt(
        (b00 * b00 + b11 * b11 + b22 * b22 + norm * safe_cast<value_type>(2)) /
        safe_cast<value_type>(6))};

    const value_type c00{b11 * b22 - a12 * a12};
    const value_type c01{a01 * b22 - a12 * a02};
    const value_type c02{a01 * a12 - b11 * a02};
    const value_type det{(b00 * c00 - a01 * c01 + a02 * c02) / (p * p * p)};

    const value_type halfDet{std::min(
        std::max(det * safe_cast<value_type>(0.5), -safe_cast<value_type>(1)),
        safe_cast<value_type>(1))};

    constexpr value_type twoThirdsPi{safe_cast<value_type>(2) *
                                     safe_cast<value_type>(M_PI) /
                                     safe_cast<value_type>(3)};
    const value_type angle{std::acos(halfDet) / safe_cast<value_type>(3)};
    const value_type beta2{std::cos(angle) * safe_cast<value_type>(2)};
    const value_type beta0{std::cos(angle + twoThirdsPi) *
                           safe_cast<value_type>(2)};
    const value_type beta1{-(beta0 + beta2)};

    // Eigenvalues ordered: alpha0 <= alpha1 <= alpha2
    this->_eigval[0] = q + p * beta0;
    this->_eigval[1] = q + p * beta1;
    this->_eigval[2] = q + p * beta2;

    if (eigenvectors) {
      if (halfDet >= 0.) {
        compute_eigenvector0(a00, a01, a02, a11, a12, a22, this->_eigval[2],
                             this->_eigvectors[2]);
        compute_eigenvector1(a00, a01, a02, a11, a12, a22, this->_eigvectors[2],
                             this->_eigval[1], this->_eigvectors[1]);
        base::cross(this->_eigvectors[0].raw_data(),
                    this->_eigvectors[1].raw_data(),
                    this->_eigvectors[2].raw_data());
      } else {
        compute_eigenvector0(a00, a01, a02, a11, a12, a22, this->_eigval[0],
                             this->_eigvectors[0]);
        compute_eigenvector1(a00, a01, a02, a11, a12, a22, this->_eigvectors[0],
                             this->_eigval[1], this->_eigvectors[1]);
        base::cross(this->_eigvectors[2].raw_data(),
                    this->_eigvectors[0].raw_data(),
                    this->_eigvectors[1].raw_data());
      }
    }
  } else {
    // Diagonal matrix
    this->_eigval[0] = a00;
    this->_eigval[1] = a11;
    this->_eigval[2] = a22;
    if (eigenvectors) {
      this->_eigvectors[0](0) = 1;
      this->_eigvectors[1](1) = 1;
      this->_eigvectors[2](2) = 1;
    }
  }

  // Revert preconditioning scaling
  for (size_type i{0}; i < 3; ++i) {
    this->_eigval[i] *= maxAbsElement;
  }

  this->sort_eigenvalues(-1, true, eigenvectors);
}

template <typename _Tensor>
constexpr inline auto
eigen_decomposition_inline<_Tensor>::compute_orthogonal_complement(
    tensor1 const &W, tensor1 &U, tensor1 &V) const noexcept {
  if (std::fabs(W(0)) > std::fabs(W(1))) {
    const auto invLength =
        safe_cast<value_type>(1) / std::sqrt(W(0) * W(0) + W(2) * W(2));
    U = tensor1(
        {-W(2) * invLength, safe_cast<value_type>(0), +W(0) * invLength});
  } else {
    const auto invLength =
        safe_cast<value_type>(1) / std::sqrt(W(1) * W(1) + W(2) * W(2));
    U = tensor1({0, +W(2) * invLength, -W(1) * invLength});
  }
  base::cross(V.raw_data(), W.raw_data(), U.raw_data());
}

template <typename _Tensor>
constexpr inline auto eigen_decomposition_inline<_Tensor>::compute_eigenvector0(
    value_type const a00, value_type const a01, value_type const a02,
    value_type const a11, value_type const a12, value_type const a22,
    value_type const eval0, tensor1 &evec0) const noexcept {

  const tensor1 row0{a00 - eval0, a01, a02};
  const tensor1 row1{a01, a11 - eval0, a12};
  const tensor1 row2{a02, a12, a22 - eval0};
  tensor1 r0xr1, r0xr2, r1xr2;

  base::cross(r0xr1.raw_data(), row0.raw_data(), row1.raw_data());
  base::cross(r0xr2.raw_data(), row0.raw_data(), row2.raw_data());
  base::cross(r1xr2.raw_data(), row1.raw_data(), row2.raw_data());

  const value_type d0{base::dot(r0xr1.raw_data(), r0xr1.raw_data())};
  const value_type d1{base::dot(r0xr2.raw_data(), r0xr2.raw_data())};
  const value_type d2{base::dot(r1xr2.raw_data(), r1xr2.raw_data())};

  value_type dmax = d0;
  int imax = 0;
  if (d1 > dmax) {
    dmax = d1;
    imax = 1;
  }
  if (d2 > dmax) {
    imax = 2;
  }

  if (imax == 0) {
    evec0 = r0xr1 * (safe_cast<value_type>(1) / std::sqrt(d0));
  } else if (imax == 1) {
    evec0 = r0xr2 * (safe_cast<value_type>(1) / std::sqrt(d1));
  } else {
    evec0 = r1xr2 * (safe_cast<value_type>(1) / std::sqrt(d2));
  }
}

template <typename _Tensor>
constexpr inline auto eigen_decomposition_inline<_Tensor>::compute_eigenvector1(
    value_type const a00, value_type const a01, value_type const a02,
    value_type const a11, value_type const a12, value_type const a22,
    tensor1 const &evec0, value_type const eval1,
    tensor1 &evec1) const noexcept {

  tensor1 U, V;
  compute_orthogonal_complement(evec0, U, V);

  // Reduce 3x3 system to 2x2: M = J^T * (A - eval1*I) * J
  // where J = [U V]
  tensor1 AU{a00 * U(0) + a01 * U(1) + a02 * U(2),
             a01 * U(0) + a11 * U(1) + a12 * U(2),
             a02 * U(0) + a12 * U(1) + a22 * U(2)};
  tensor1 AV{a00 * V(0) + a01 * V(1) + a02 * V(2),
             a01 * V(0) + a11 * V(1) + a12 * V(2),
             a02 * V(0) + a12 * V(1) + a22 * V(2)};

  value_type m00{base::dot(U.raw_data(), AU.raw_data()) - eval1};
  value_type m01{base::dot(U.raw_data(), AV.raw_data())};
  value_type m11{base::dot(V.raw_data(), AV.raw_data()) - eval1};

  const value_type absM00{std::fabs(m00)};
  const value_type absM01{std::fabs(m01)};
  const value_type absM11{std::fabs(m11)};

  if (absM00 >= absM11) {
    const value_type maxAbsComp{std::max(absM00, absM01)};
    if (maxAbsComp > safe_cast<value_type>(0)) {
      if (absM00 >= absM01) {
        m01 /= m00;
        m00 = safe_cast<value_type>(1) /
              std::sqrt(safe_cast<value_type>(1) + m01 * m01);
        m01 *= m00;
      } else {
        m00 /= m01;
        m01 = safe_cast<value_type>(1) /
              std::sqrt(safe_cast<value_type>(1) + m00 * m00);
        m00 *= m01;
      }
      evec1 = U * m01 - V * m00;
    } else {
      evec1 = U;
    }
  } else {
    const value_type maxAbsComp{std::max(absM11, absM01)};
    if (maxAbsComp > safe_cast<value_type>(0)) {
      if (absM11 >= absM01) {
        m01 /= m11;
        m11 = safe_cast<value_type>(1) /
              std::sqrt(safe_cast<value_type>(1) + m01 * m01);
        m01 *= m11;
      } else {
        m11 /= m01;
        m01 = safe_cast<value_type>(1) /
              std::sqrt(safe_cast<value_type>(1) + m11 * m11);
        m11 *= m01;
      }
      evec1 = U * m11 - V * m01;
    } else {
      evec1 = U;
    }
  }
}

//==========================================================================
//
//  eigen_decomposition_lapack — LAPACK-based implementation
//
//==========================================================================

template <typename _Tensor>
constexpr inline auto
eigen_decomposition_lapack<_Tensor>::compute_decomposition(
    bool eigenvectors) noexcept {
  // Make a working copy — LAPACK destroys the input matrix.
  // Note: for symmetric tensors, row-major == column-major,
  // so passing raw_data() directly to LAPACK is valid.
  auto data_copy = this->_data;

  int n = static_cast<int>(Dim);
  int lda = n;
  value_type wi[Dim]{}; // imaginary parts (zero for symmetric)
  value_type vl[1]{};   // left eigenvectors not computed
  int ldvl = 1;
  value_type vr[Dim * Dim]{}; // right eigenvectors (column-major)
  int ldvr = n;
  constexpr int lwork_size = 4 * static_cast<int>(Dim);
  value_type work[lwork_size]{};
  int lwork = lwork_size;
  int info = 0;

  char jobvl = 'N';
  char jobvr = eigenvectors ? 'V' : 'N';

  if constexpr (std::is_same_v<value_type, double>) {
    dgeev_(&jobvl, &jobvr, &n, data_copy.raw_data(), &lda, this->_eigval.data(),
           wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info);
  } else if constexpr (std::is_same_v<value_type, float>) {
    sgeev_(&jobvl, &jobvr, &n, data_copy.raw_data(), &lda, this->_eigval.data(),
           wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info);
  }

  // Sort eigenvalues ascending
  std::array<size_type, Dim> idx;
  std::iota(idx.begin(), idx.end(), 0);
  std::sort(idx.begin(), idx.end(), [&](size_type i1, size_type i2) {
    return this->_eigval[i1] < this->_eigval[i2];
  });

  const auto temp_eigval = this->_eigval;
  for (size_type i = 0; i < Dim; ++i) {
    this->_eigval[i] = temp_eigval[idx[i]];
    this->_permut[i] = idx[i];
    if (eigenvectors) {
      // LAPACK stores eigenvectors as columns (column-major)
      for (size_type j = 0; j < Dim; ++j) {
        this->_eigvectors[i](j) = vr[idx[i] * ldvr + j];
      }
    }
  }
}

} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_MEAT_H
