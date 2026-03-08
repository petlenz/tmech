/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SVD_BONES_H
#define SVD_BONES_H

namespace detail {

template <typename _Base, typename _Tensor>
class svd_U_wrapper;

template <typename _Base, typename _Tensor>
class svd_S_wrapper;

template <typename _Base, typename _Tensor>
class svd_V_wrapper;

template <typename _Base, typename _Tensor>
class svd_dU_wrapper;

template <typename _Base, typename _Tensor>
class svd_dS_wrapper;

template <typename _Base, typename _Tensor>
class svd_dV_wrapper;

template <typename _Tensor>
class singular_value_decomposition_wrapper {
  using data_type_tensor = typename std::remove_const<
      typename std::remove_reference<_Tensor>::type>::type;

public:
  using value_type = typename data_type_tensor::value_type;
  using size_type = std::size_t;
  using this_type = singular_value_decomposition_wrapper<_Tensor>;
  using tensor1 = tensor<value_type, data_type_tensor::dimension(), 1>;
  using tensor2 = tensor<value_type, data_type_tensor::dimension(), 2>;
  using tensor4 = tensor<value_type, data_type_tensor::dimension(), 4>;

  static_assert(data_type_tensor::dimension() == 2 ||
                    data_type_tensor::dimension() == 3,
                "svd: only Dim == 2, 3");
  static_assert(data_type_tensor::rank() == 2, "svd: only Rank == 2");

  friend class svd_U_wrapper<this_type, tensor2>;
  friend class svd_S_wrapper<this_type, tensor2>;
  friend class svd_V_wrapper<this_type, tensor2>;
  friend class svd_dU_wrapper<this_type, tensor4>;
  friend class svd_dS_wrapper<this_type, tensor4>;
  friend class svd_dV_wrapper<this_type, tensor4>;

  constexpr singular_value_decomposition_wrapper(data_type_tensor const &data,
                                                 value_type tol) noexcept;
  constexpr singular_value_decomposition_wrapper(
      singular_value_decomposition_wrapper const &data) noexcept;

  constexpr inline auto U() const noexcept;
  constexpr inline auto S() const noexcept;
  constexpr inline auto V() const noexcept;
  constexpr inline auto singular_values() const noexcept;

private:
  static constexpr auto Dim = data_type_tensor::dimension();

  constexpr inline auto evaluate() noexcept;
  inline auto evaluate_derivatives();
  inline auto ensure_well_defined_derivatives() const;
  constexpr inline auto set_matrix_column(tensor2 &matrix, size_type col,
                                          tensor1 const &vec) noexcept;
  constexpr inline auto get_matrix_column(tensor2 const &matrix,
                                          size_type col) const noexcept;
  constexpr inline auto normalise_vector(tensor1 &vec) const noexcept;
  constexpr inline auto orthonormalise_against_previous(tensor1 &vec,
                                                        size_type count) const
      noexcept;
  constexpr inline auto make_orthogonal_vector_2d(tensor1 const &vec) const
      noexcept;
  constexpr inline auto make_orthogonal_vector_3d(tensor1 const &vec) const
      noexcept;
  constexpr inline auto
  complete_left_basis(std::array<bool, Dim> const &occupied,
                      size_type count) noexcept;
  constexpr inline auto effective_tol() const noexcept;

  bool _is_init;
  bool _is_init_deriv;
  value_type _tol;
  tensor2 _U;
  tensor2 _S;
  tensor2 _V;
  tensor4 _dU;
  tensor4 _dS;
  tensor4 _dV;
  std::array<value_type, Dim> _singular_values;
  _Tensor _data;
};

template <typename _Base, typename _Tensor>
class svd_U_wrapper : public tensor_base<svd_U_wrapper<_Base, _Tensor>> {
  using basetype = tensor_base<svd_U_wrapper<_Base, _Tensor>>;

public:
  using size_type = std::size_t;
  using value_type = typename _Tensor::value_type;

  svd_U_wrapper(_Base const &base, _Tensor const &U) noexcept;
  svd_U_wrapper(svd_U_wrapper const &data) noexcept;

  constexpr inline auto const &operator()(size_type i, size_type j) const noexcept;
  static constexpr inline auto dimension() noexcept;
  static constexpr inline auto rank() noexcept;
  constexpr inline auto raw_data() const noexcept;
  constexpr inline auto evaluate() noexcept;
  constexpr inline auto derivative() const noexcept;

private:
  _Base const &_base;
  _Tensor const &_U;
};

template <typename _Base, typename _Tensor>
class svd_S_wrapper : public tensor_base<svd_S_wrapper<_Base, _Tensor>> {
  using basetype = tensor_base<svd_S_wrapper<_Base, _Tensor>>;

public:
  using size_type = std::size_t;
  using value_type = typename _Tensor::value_type;

  svd_S_wrapper(_Base const &base, _Tensor const &S) noexcept;
  svd_S_wrapper(svd_S_wrapper const &data) noexcept;

  constexpr inline auto const &operator()(size_type i, size_type j) const noexcept;
  static constexpr inline auto dimension() noexcept;
  static constexpr inline auto rank() noexcept;
  constexpr inline auto raw_data() const noexcept;
  constexpr inline auto evaluate() noexcept;
  constexpr inline auto derivative() const noexcept;

private:
  _Base const &_base;
  _Tensor const &_S;
};

template <typename _Base, typename _Tensor>
class svd_V_wrapper : public tensor_base<svd_V_wrapper<_Base, _Tensor>> {
  using basetype = tensor_base<svd_V_wrapper<_Base, _Tensor>>;

public:
  using size_type = std::size_t;
  using value_type = typename _Tensor::value_type;

  svd_V_wrapper(_Base const &base, _Tensor const &V) noexcept;
  svd_V_wrapper(svd_V_wrapper const &data) noexcept;

  constexpr inline auto const &operator()(size_type i, size_type j) const noexcept;
  static constexpr inline auto dimension() noexcept;
  static constexpr inline auto rank() noexcept;
  constexpr inline auto raw_data() const noexcept;
  constexpr inline auto evaluate() noexcept;
  constexpr inline auto derivative() const noexcept;

private:
  _Base const &_base;
  _Tensor const &_V;
};

template <typename _Base, typename _Tensor>
class svd_dU_wrapper : public tensor_base<svd_dU_wrapper<_Base, _Tensor>> {
  using basetype = tensor_base<svd_dU_wrapper<_Base, _Tensor>>;

public:
  using size_type = std::size_t;
  using value_type = typename _Tensor::value_type;

  svd_dU_wrapper(_Base const &base, _Tensor const &dU) noexcept;
  svd_dU_wrapper(svd_dU_wrapper const &data) noexcept;

  constexpr inline auto const &operator()(size_type i, size_type j, size_type k,
                                          size_type l) const noexcept;
  static constexpr inline auto dimension() noexcept;
  static constexpr inline auto rank() noexcept;
  constexpr inline auto raw_data() const noexcept;
  inline auto evaluate();

private:
  _Base const &_base;
  _Tensor const &_dU;
};

template <typename _Base, typename _Tensor>
class svd_dS_wrapper : public tensor_base<svd_dS_wrapper<_Base, _Tensor>> {
  using basetype = tensor_base<svd_dS_wrapper<_Base, _Tensor>>;

public:
  using size_type = std::size_t;
  using value_type = typename _Tensor::value_type;

  svd_dS_wrapper(_Base const &base, _Tensor const &dS) noexcept;
  svd_dS_wrapper(svd_dS_wrapper const &data) noexcept;

  constexpr inline auto const &operator()(size_type i, size_type j, size_type k,
                                          size_type l) const noexcept;
  static constexpr inline auto dimension() noexcept;
  static constexpr inline auto rank() noexcept;
  constexpr inline auto raw_data() const noexcept;
  inline auto evaluate();

private:
  _Base const &_base;
  _Tensor const &_dS;
};

template <typename _Base, typename _Tensor>
class svd_dV_wrapper : public tensor_base<svd_dV_wrapper<_Base, _Tensor>> {
  using basetype = tensor_base<svd_dV_wrapper<_Base, _Tensor>>;

public:
  using size_type = std::size_t;
  using value_type = typename _Tensor::value_type;

  svd_dV_wrapper(_Base const &base, _Tensor const &dV) noexcept;
  svd_dV_wrapper(svd_dV_wrapper const &data) noexcept;

  constexpr inline auto const &operator()(size_type i, size_type j, size_type k,
                                          size_type l) const noexcept;
  static constexpr inline auto dimension() noexcept;
  static constexpr inline auto rank() noexcept;
  constexpr inline auto raw_data() const noexcept;
  inline auto evaluate();

private:
  _Base const &_base;
  _Tensor const &_dV;
};

} // namespace detail

#endif // SVD_BONES_H
