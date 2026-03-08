/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SVD_MEAT_H
#define SVD_MEAT_H

namespace detail {

template <typename _Tensor>
constexpr singular_value_decomposition_wrapper<_Tensor>::
    singular_value_decomposition_wrapper(data_type_tensor const &data,
                                         value_type tol) noexcept
    : _is_init(false), _is_init_deriv(false), _tol(tol), _U(), _S(), _V(),
      _dU(), _dS(), _dV(), _singular_values(), _data(data) {}

template <typename _Tensor>
constexpr singular_value_decomposition_wrapper<_Tensor>::
    singular_value_decomposition_wrapper(
        singular_value_decomposition_wrapper const &data) noexcept
    : _is_init(data._is_init), _is_init_deriv(data._is_init_deriv),
      _tol(data._tol), _U(data._U), _S(data._S), _V(data._V), _dU(data._dU),
      _dS(data._dS), _dV(data._dV), _singular_values(data._singular_values),
      _data(data._data) {}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::U() const noexcept {
  return svd_U_wrapper<this_type, tensor2>(*this, _U);
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::S() const noexcept {
  return svd_S_wrapper<this_type, tensor2>(*this, _S);
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::V() const noexcept {
  return svd_V_wrapper<this_type, tensor2>(*this, _V);
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::singular_values() const noexcept {
  return _singular_values;
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::effective_tol() const noexcept {
  const auto sigma_max{
      *std::max_element(_singular_values.begin(), _singular_values.end())};
  return std::max(_tol, std::numeric_limits<value_type>::epsilon() *
                            std::max(value_type(1), sigma_max) *
                            value_type(100));
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::set_matrix_column(
    tensor2 &matrix, size_type col, tensor1 const &vec) noexcept {
  for (size_type row{0}; row < Dim; ++row) {
    matrix(row, col) = vec(row);
  }
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::get_matrix_column(
    tensor2 const &matrix, size_type col) const noexcept {
  tensor1 vec;
  for (size_type row{0}; row < Dim; ++row) {
    vec(row) = matrix(row, col);
  }
  return vec;
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::normalise_vector(
    tensor1 &vec) const noexcept {
  const auto vec_norm{norm(vec)};
  if (vec_norm > _tol) {
    vec /= vec_norm;
    return true;
  }
  vec.fill(value_type(0));
  return false;
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::orthonormalise_against_previous(
    tensor1 &vec, size_type count) const noexcept {
  for (size_type j{0}; j < count; ++j) {
    const auto base_vec{get_matrix_column(_U, j)};
    vec -= dot(vec, base_vec) * base_vec;
  }
  return normalise_vector(vec);
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::make_orthogonal_vector_2d(
    tensor1 const &vec) const noexcept {
  tensor1 result;
  result(0) = -vec(1);
  result(1) = vec(0);
  return result;
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::make_orthogonal_vector_3d(
    tensor1 const &vec) const noexcept {
  tensor1 trial;
  trial.fill(value_type(0));

  size_type min_idx{0};
  auto min_abs{std::fabs(vec(0))};
  for (size_type i{1}; i < Dim; ++i) {
    const auto abs_val{std::fabs(vec(i))};
    if (abs_val < min_abs) {
      min_abs = abs_val;
      min_idx = i;
    }
  }

  trial(min_idx) = value_type(1);
  trial -= dot(trial, vec) * vec;
  if (!normalise_vector(trial)) {
    trial.fill(value_type(0));
    trial((min_idx + 1) % Dim) = value_type(1);
    trial -= dot(trial, vec) * vec;
    normalise_vector(trial);
  }
  return trial;
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::complete_left_basis(
    std::array<bool, Dim> const &occupied, size_type count) noexcept {
  if constexpr (Dim == 2) {
    if (count == 0) {
      _U = eye<value_type, Dim, 2>();
    } else if (count == 1) {
      size_type existing{0};
      size_type missing{0};
      for (size_type i{0}; i < Dim; ++i) {
        if (occupied[i]) {
          existing = i;
        } else {
          missing = i;
        }
      }
      auto col{make_orthogonal_vector_2d(get_matrix_column(_U, existing))};
      normalise_vector(col);
      set_matrix_column(_U, missing, col);
    }
  } else {
    if (count == 0) {
      _U = eye<value_type, Dim, 2>();
    } else if (count == 1) {
      std::array<size_type, Dim> missing{};
      size_type existing{0};
      size_type miss_count{0};
      for (size_type i{0}; i < Dim; ++i) {
        if (occupied[i]) {
          existing = i;
        } else {
          missing[miss_count++] = i;
        }
      }
      auto col1{make_orthogonal_vector_3d(get_matrix_column(_U, existing))};
      orthonormalise_against_previous(col1, 1);
      set_matrix_column(_U, missing[0], col1);

      tensor1 col2{
          cross(get_matrix_column(_U, existing), get_matrix_column(_U, missing[0]))};
      normalise_vector(col2);
      set_matrix_column(_U, missing[1], col2);
    } else if (count == 2) {
      std::array<size_type, Dim> existing{};
      size_type missing{0};
      size_type exist_count{0};
      for (size_type i{0}; i < Dim; ++i) {
        if (occupied[i]) {
          existing[exist_count++] = i;
        } else {
          missing = i;
        }
      }
      tensor1 col2{
          cross(get_matrix_column(_U, existing[0]), get_matrix_column(_U, existing[1]))};
      if (!normalise_vector(col2)) {
        col2 = make_orthogonal_vector_3d(get_matrix_column(_U, existing[0]));
        orthonormalise_against_previous(col2, 2);
      }
      set_matrix_column(_U, missing, col2);
    }
  }
}

template <typename _Tensor>
constexpr inline auto
singular_value_decomposition_wrapper<_Tensor>::evaluate() noexcept {
  if (_is_init) {
    return;
  }

  auto eig{eigen_decomposition(trans(_data) * _data)};
  const auto [eigenvalues, eigenvectors]{eig.decompose()};
  std::array<size_type, Dim> permutation{};
  std::iota(permutation.begin(), permutation.end(), size_type(0));
  std::sort(permutation.begin(), permutation.end(),
            [&](size_type lhs, size_type rhs) {
              return eigenvalues[lhs] > eigenvalues[rhs];
            });

  for (size_type i{0}; i < Dim; ++i) {
    const auto idx{permutation[i]};
    auto lambda{eigenvalues[idx]};
    if (lambda < value_type(0) && std::fabs(lambda) <= _tol) {
      lambda = value_type(0);
    }

    const auto sigma{
        lambda > value_type(0) ? std::sqrt(lambda) : value_type(0)};
    _singular_values[i] = sigma;
    _S(i, i) = sigma;
    set_matrix_column(_V, i, eigenvectors[idx]);
  }

  const auto tol{effective_tol()};
  std::array<bool, Dim> occupied{};

  size_type rank{0};
  for (size_type i{0}; i < Dim; ++i) {
    if (_singular_values[i] <= tol) {
      continue;
    }

    tensor1 left_vec{(_data * get_matrix_column(_V, i)) / _singular_values[i]};
    if (!orthonormalise_against_previous(left_vec, rank)) {
      continue;
    }
    set_matrix_column(_U, i, left_vec);
    occupied[i] = true;
    ++rank;
  }

  complete_left_basis(occupied, rank);
  _is_init = true;
}

template <typename _Tensor>
inline auto singular_value_decomposition_wrapper<_Tensor>::
    ensure_well_defined_derivatives() const {
  const auto tol{effective_tol()};
  for (size_type i{0}; i < Dim; ++i) {
    if (_singular_values[i] <= tol) {
      throw std::runtime_error(
          "svd derivatives undefined for zero singular values");
    }
  }

  for (size_type i{0}; i < Dim; ++i) {
    for (size_type j{i + 1}; j < Dim; ++j) {
      if (std::fabs(_singular_values[i] - _singular_values[j]) <= tol) {
        throw std::runtime_error(
            "svd derivatives undefined for repeated singular values");
      }
    }
  }
}

template <typename _Tensor>
inline auto singular_value_decomposition_wrapper<_Tensor>::evaluate_derivatives() {
  evaluate();
  if (_is_init_deriv) {
    return;
  }

  ensure_well_defined_derivatives();

  for (size_type i{0}; i < Dim; ++i) {
    for (size_type k{0}; k < Dim; ++k) {
      for (size_type l{0}; l < Dim; ++l) {
        _dS(i, i, k, l) = _U(k, i) * _V(l, i);
      }
    }
  }

  for (size_type i{0}; i < Dim; ++i) {
    for (size_type j{i + 1}; j < Dim; ++j) {
      const auto si{_singular_values[i]};
      const auto sj{_singular_values[j]};
      const auto denom{(sj * sj) - (si * si)};

      for (size_type k{0}; k < Dim; ++k) {
        for (size_type l{0}; l < Dim; ++l) {
          const auto ukivlj{_U(k, i) * _V(l, j)};
          const auto ukjvli{_U(k, j) * _V(l, i)};

          const auto omega_u{(sj * ukivlj + si * ukjvli) / denom};
          const auto omega_v{(si * ukivlj + sj * ukjvli) / denom};

          for (size_type a{0}; a < Dim; ++a) {
            _dU(a, i, k, l) -= omega_u * _U(a, j);
            _dU(a, j, k, l) += omega_u * _U(a, i);
            _dV(a, i, k, l) -= omega_v * _V(a, j);
            _dV(a, j, k, l) += omega_v * _V(a, i);
          }
        }
      }
    }
  }

  _is_init_deriv = true;
}

template <typename _Base, typename _Tensor>
svd_U_wrapper<_Base, _Tensor>::svd_U_wrapper(_Base const &base,
                                             _Tensor const &U) noexcept
    : _base(base), _U(U) {}

template <typename _Base, typename _Tensor>
svd_U_wrapper<_Base, _Tensor>::svd_U_wrapper(svd_U_wrapper const &data) noexcept
    : basetype(data), _base(data._base), _U(data._U) {}

template <typename _Base, typename _Tensor>
constexpr inline auto const &
svd_U_wrapper<_Base, _Tensor>::operator()(size_type i, size_type j) const
    noexcept {
  return _U(i, j);
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_U_wrapper<_Base, _Tensor>::dimension() noexcept {
  return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_U_wrapper<_Base, _Tensor>::rank() noexcept {
  return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_U_wrapper<_Base, _Tensor>::raw_data() const noexcept {
  return _U.raw_data();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_U_wrapper<_Base, _Tensor>::evaluate() noexcept {
  const_cast<_Base &>(_base).evaluate();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_U_wrapper<_Base, _Tensor>::derivative() const noexcept {
  return svd_dU_wrapper<_Base, typename _Base::tensor4>(_base, _base._dU);
}

template <typename _Base, typename _Tensor>
svd_S_wrapper<_Base, _Tensor>::svd_S_wrapper(_Base const &base,
                                             _Tensor const &S) noexcept
    : _base(base), _S(S) {}

template <typename _Base, typename _Tensor>
svd_S_wrapper<_Base, _Tensor>::svd_S_wrapper(svd_S_wrapper const &data) noexcept
    : basetype(data), _base(data._base), _S(data._S) {}

template <typename _Base, typename _Tensor>
constexpr inline auto const &
svd_S_wrapper<_Base, _Tensor>::operator()(size_type i, size_type j) const
    noexcept {
  return _S(i, j);
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_S_wrapper<_Base, _Tensor>::dimension() noexcept {
  return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_S_wrapper<_Base, _Tensor>::rank() noexcept {
  return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_S_wrapper<_Base, _Tensor>::raw_data() const noexcept {
  return _S.raw_data();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_S_wrapper<_Base, _Tensor>::evaluate() noexcept {
  const_cast<_Base &>(_base).evaluate();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_S_wrapper<_Base, _Tensor>::derivative() const noexcept {
  return svd_dS_wrapper<_Base, typename _Base::tensor4>(_base, _base._dS);
}

template <typename _Base, typename _Tensor>
svd_V_wrapper<_Base, _Tensor>::svd_V_wrapper(_Base const &base,
                                             _Tensor const &V) noexcept
    : _base(base), _V(V) {}

template <typename _Base, typename _Tensor>
svd_V_wrapper<_Base, _Tensor>::svd_V_wrapper(svd_V_wrapper const &data) noexcept
    : basetype(data), _base(data._base), _V(data._V) {}

template <typename _Base, typename _Tensor>
constexpr inline auto const &
svd_V_wrapper<_Base, _Tensor>::operator()(size_type i, size_type j) const
    noexcept {
  return _V(i, j);
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_V_wrapper<_Base, _Tensor>::dimension() noexcept {
  return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_V_wrapper<_Base, _Tensor>::rank() noexcept {
  return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_V_wrapper<_Base, _Tensor>::raw_data() const noexcept {
  return _V.raw_data();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_V_wrapper<_Base, _Tensor>::evaluate() noexcept {
  const_cast<_Base &>(_base).evaluate();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_V_wrapper<_Base, _Tensor>::derivative() const noexcept {
  return svd_dV_wrapper<_Base, typename _Base::tensor4>(_base, _base._dV);
}

template <typename _Base, typename _Tensor>
svd_dU_wrapper<_Base, _Tensor>::svd_dU_wrapper(_Base const &base,
                                               _Tensor const &dU) noexcept
    : _base(base), _dU(dU) {}

template <typename _Base, typename _Tensor>
svd_dU_wrapper<_Base, _Tensor>::svd_dU_wrapper(
    svd_dU_wrapper const &data) noexcept
    : basetype(data), _base(data._base), _dU(data._dU) {}

template <typename _Base, typename _Tensor>
constexpr inline auto const &
svd_dU_wrapper<_Base, _Tensor>::operator()(size_type i, size_type j,
                                           size_type k,
                                           size_type l) const noexcept {
  return _dU(i, j, k, l);
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dU_wrapper<_Base, _Tensor>::dimension() noexcept {
  return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dU_wrapper<_Base, _Tensor>::rank() noexcept {
  return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dU_wrapper<_Base, _Tensor>::raw_data() const noexcept {
  return _dU.raw_data();
}

template <typename _Base, typename _Tensor>
inline auto svd_dU_wrapper<_Base, _Tensor>::evaluate() {
  const_cast<_Base &>(_base).evaluate_derivatives();
}

template <typename _Base, typename _Tensor>
svd_dS_wrapper<_Base, _Tensor>::svd_dS_wrapper(_Base const &base,
                                               _Tensor const &dS) noexcept
    : _base(base), _dS(dS) {}

template <typename _Base, typename _Tensor>
svd_dS_wrapper<_Base, _Tensor>::svd_dS_wrapper(
    svd_dS_wrapper const &data) noexcept
    : basetype(data), _base(data._base), _dS(data._dS) {}

template <typename _Base, typename _Tensor>
constexpr inline auto const &
svd_dS_wrapper<_Base, _Tensor>::operator()(size_type i, size_type j,
                                           size_type k,
                                           size_type l) const noexcept {
  return _dS(i, j, k, l);
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dS_wrapper<_Base, _Tensor>::dimension() noexcept {
  return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dS_wrapper<_Base, _Tensor>::rank() noexcept {
  return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dS_wrapper<_Base, _Tensor>::raw_data() const noexcept {
  return _dS.raw_data();
}

template <typename _Base, typename _Tensor>
inline auto svd_dS_wrapper<_Base, _Tensor>::evaluate() {
  const_cast<_Base &>(_base).evaluate_derivatives();
}

template <typename _Base, typename _Tensor>
svd_dV_wrapper<_Base, _Tensor>::svd_dV_wrapper(_Base const &base,
                                               _Tensor const &dV) noexcept
    : _base(base), _dV(dV) {}

template <typename _Base, typename _Tensor>
svd_dV_wrapper<_Base, _Tensor>::svd_dV_wrapper(
    svd_dV_wrapper const &data) noexcept
    : basetype(data), _base(data._base), _dV(data._dV) {}

template <typename _Base, typename _Tensor>
constexpr inline auto const &
svd_dV_wrapper<_Base, _Tensor>::operator()(size_type i, size_type j,
                                           size_type k,
                                           size_type l) const noexcept {
  return _dV(i, j, k, l);
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dV_wrapper<_Base, _Tensor>::dimension() noexcept {
  return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dV_wrapper<_Base, _Tensor>::rank() noexcept {
  return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto svd_dV_wrapper<_Base, _Tensor>::raw_data() const noexcept {
  return _dV.raw_data();
}

template <typename _Base, typename _Tensor>
inline auto svd_dV_wrapper<_Base, _Tensor>::evaluate() {
  const_cast<_Base &>(_base).evaluate_derivatives();
}

} // namespace detail

#endif // SVD_MEAT_H
