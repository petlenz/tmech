/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EIGEN_DECOMPOSITION_BONES_H
#define EIGEN_DECOMPOSITION_BONES_H

#include "eigen_decomposition_eigenbasis_bones.h"
#include "eigen_decomposition_eigenbasis_meat.h"
#include "eigen_decomposition_eigenvectors_bones.h"
#include "eigen_decomposition_eigenvectors_meat.h"
#include "eigen_decomposition_eigenvalues_bones.h"
#include "eigen_decomposition_eigenvalues_meat.h"

namespace detail {

// Forward declarations
template <typename _Tensor> class eigen_decomposition_inline;
template <typename _Tensor> class eigen_decomposition_lapack;

/**
 * @class eigen_decomposition_base
 * @brief CRTP base class providing shared eigenvalue decomposition
 *        infrastructure: data storage, eigenbasis computation,
 *        repeated-eigenvalue detection, sorting, and math helpers.
 *
 * Derived classes must implement:
 *   constexpr inline auto compute_decomposition(bool eigenvectors) noexcept;
 *
 * @tparam _Tensor  Tensor expression type.
 * @tparam _Derived CRTP derived class.
 */
template <typename _Tensor, typename _Derived> class eigen_decomposition_base {
  using data_type_tensor = typename std::remove_const<
      typename std::remove_reference<_Tensor>::type>::type;
  static constexpr auto Dim{data_type_tensor::dimension()};
  static constexpr auto Rank{data_type_tensor::rank()};

  // Grant access to the result wrapper classes
  template <typename> friend class eigen_decomposition_basis_wrapper;
  template <typename> friend class eigen_decomposition_vectors_wrapper;
  template <typename> friend class eigen_decomposition_values_wrapper;

public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;
    using tensor1 = tensor<value_type, Dim, 1>;
    using tensor2 = tensor<value_type, Dim, 2>;

    static_assert(Dim == 2 || Dim == 3,
                  "eigen_decomposition: only Dim == 2, 3");
    static_assert(Rank == 2, "eigen_decomposition: only Rank == 2");

    eigen_decomposition_base(_Tensor const &data) noexcept;
    eigen_decomposition_base(eigen_decomposition_base const &data) noexcept;

    constexpr inline auto decompose() noexcept;
    constexpr inline auto decompose_eigenbasis()noexcept;

    constexpr inline auto eigenvalues() const noexcept;
    constexpr inline auto eigenvectors() const noexcept;
    constexpr inline auto eigenbasis()const noexcept;

    constexpr inline auto all_eigenvalues_repeated() const noexcept;
    constexpr inline auto pair_eigenvalues_repeated() const noexcept;
    constexpr inline auto non_repeated_eigenvalues_index()const noexcept;
    constexpr inline size_type number_non_repeated_eigenvalues() const noexcept;
    constexpr inline auto permutation() const noexcept;

    constexpr inline auto get_repeated_eigenvalues() noexcept;
    constexpr inline auto evaluate_eigenbasis()noexcept;
    constexpr inline auto sort_eigenvalues(int sortType, bool isRotation,
                                           bool eigenvectors) noexcept;

  protected:
    static constexpr inline auto cross(value_type *data,
                                       value_type const *const lhs,
                                       value_type const *const rhs) noexcept;
    static constexpr inline auto dot(value_type const *const lhs,
                                     value_type const *const rhs) noexcept;
    static constexpr inline auto mul(value_type *data,
                                     value_type const *const lhs,
                                     value_type const *const rhs) noexcept;

    tensor2 _data;
    std::array<value_type, Dim> _eigval;
    std::array<tensor1, Dim> _eigvectors;
    std::array<tensor2, Dim> _eig_basis;
    std::array<size_type, Dim> _non_repeated_eigenvalues;
    std::array<size_type, Dim> _permut;
    bool _all_repeated_eigenvalues;
    bool _pair_repeated_eigenvalues;
    bool _is_init;
    _Tensor _data_base;
};

/**
 * @class eigen_decomposition_inline
 * @brief Analytical (closed-form) eigenvalue decomposition.
 *
 * Uses direct formulas for 2D (quadratic) and 3D (cubic via
 * trigonometric solution). Based in part on the work of
 * David Eberly, Geometric Tools LLC.
 *
 * @tparam _Tensor Tensor expression type.
 */
template <typename _Tensor>
class eigen_decomposition_inline
    : public eigen_decomposition_base<_Tensor,
                                      eigen_decomposition_inline<_Tensor>> {
  using base =
      eigen_decomposition_base<_Tensor, eigen_decomposition_inline<_Tensor>>;
  using data_type_tensor = typename std::remove_const<
      typename std::remove_reference<_Tensor>::type>::type;
  static constexpr auto Dim{data_type_tensor::dimension()};

  friend base;

  template <typename> friend class eigen_decomposition_basis_wrapper;
  template <typename> friend class eigen_decomposition_vectors_wrapper;
  template <typename> friend class eigen_decomposition_values_wrapper;

public:
  using typename base::size_type;
  using typename base::tensor1;
  using typename base::tensor2;
  using typename base::value_type;

  using base::base; // inherit constructors

  constexpr inline auto compute_decomposition(bool eigenvectors) noexcept;

private:
  constexpr inline auto evaluate_detail_2D(bool eigenvectors, value_type _a00,
                                           value_type _a01, value_type _a10,
                                           value_type _a11) noexcept;

  constexpr inline auto evaluate_detail_3D(bool eigenvectors, value_type _a00,
                                           value_type _a01, value_type _a02,
                                           value_type _a11, value_type _a12,
                                           value_type _a22) noexcept;

  constexpr inline auto
  compute_orthogonal_complement(tensor1 const &W, tensor1 &U,
                                tensor1 &V) const noexcept;

  constexpr inline auto compute_eigenvector0(value_type a00, value_type a01,
                                             value_type a02, value_type a11,
                                             value_type a12, value_type a22,
                                             value_type eval0,
                                             tensor1 &evec0) const noexcept;

  constexpr inline auto compute_eigenvector1(value_type a00, value_type a01,
                                             value_type a02, value_type a11,
                                             value_type a12, value_type a22,
                                             tensor1 const &evec0,
                                             value_type eval1,
                                             tensor1 &evec1) const noexcept;
};

/**
 * @class eigen_decomposition_lapack
 * @brief LAPACK-based eigenvalue decomposition using dgeev/sgeev.
 *
 * Uses LAPACK's general eigenvalue solver (dgeev for double,
 * sgeev for float). Suitable for any non-singular symmetric
 * 2D or 3D tensor.
 *
 * @tparam _Tensor Tensor expression type.
 */
template <typename _Tensor>
class eigen_decomposition_lapack
    : public eigen_decomposition_base<_Tensor,
                                      eigen_decomposition_lapack<_Tensor>> {
  using base =
      eigen_decomposition_base<_Tensor, eigen_decomposition_lapack<_Tensor>>;
  using data_type_tensor = typename std::remove_const<
      typename std::remove_reference<_Tensor>::type>::type;
  static constexpr auto Dim{data_type_tensor::dimension()};

  friend base;

  template <typename> friend class eigen_decomposition_basis_wrapper;
  template <typename> friend class eigen_decomposition_vectors_wrapper;
  template <typename> friend class eigen_decomposition_values_wrapper;

public:
  using typename base::size_type;
  using typename base::tensor1;
  using typename base::tensor2;
  using typename base::value_type;

  using base::base; // inherit constructors

  constexpr inline auto compute_decomposition(bool eigenvectors) noexcept;
};

/**
 * Backward-compatible alias.
 */
template <typename _Tensor>
using eigen_decomposition_wrapper = eigen_decomposition_inline<_Tensor>;

} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_BONES_H
