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

// A small part of the code in the following class was inspired by works of David
// Eberly, Geometric Tools LLC, in code released under the Boost Software
// License. See LICENSE.md
template <typename _Tensor>
class eigen_decomposition_wrapper
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
    static constexpr auto Dim{data_type_tensor::dimension()};
    static constexpr auto Rank{data_type_tensor::rank()};

    friend class eigen_decomposition_basis_wrapper<eigen_decomposition_wrapper<_Tensor>>;
    friend class eigen_decomposition_vectors_wrapper<eigen_decomposition_wrapper<_Tensor>>;
    friend class eigen_decomposition_values_wrapper<eigen_decomposition_wrapper<_Tensor>>;

public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;
    using tensor1    = tensor<value_type, data_type_tensor::dimension(), 1>;
    using tensor2    = tensor<value_type, data_type_tensor::dimension(), 2>;

    static_assert(Dim == 2 || Dim == 3, "eigen_decomposition_wrapper only for Dim == 2, 3");
    static_assert(Rank == 2,            "eigen_decomposition_wrapper for Rank == 2");

    eigen_decomposition_wrapper(_Tensor const& data)noexcept;

    eigen_decomposition_wrapper(eigen_decomposition_wrapper const& data)noexcept;

    constexpr inline auto decompose()noexcept;

    constexpr inline auto decompose_eigenbasis()noexcept;

    constexpr inline auto eigenvalues()const noexcept;

    constexpr inline auto eigenvectors()const noexcept;

    constexpr inline auto eigenbasis()const noexcept;

    constexpr inline auto all_eigenvalues_repeated()const noexcept;

    constexpr inline auto pair_eigenvalues_repeated()const noexcept;

    constexpr inline auto non_repeated_eigenvalues_index()const noexcept;

    constexpr inline auto evaluate_eigenbasis()noexcept;

    constexpr inline size_type number_non_repeated_eigenvalues()const noexcept;

    constexpr inline auto permutation()const noexcept;

private:
    constexpr inline auto get_repeated_eigenvalues()noexcept;

    constexpr inline auto evaluate_detail_2D(bool eigenvectors, value_type const _a00, value_type const _a01, value_type const _a10, value_type const _a11)noexcept;

    constexpr inline auto evaluate_detail_3D(bool eigenvectors, value_type const _a00, value_type const _a01, value_type const _a02, value_type const _a11, value_type const _a12, value_type const _a22)noexcept;

    constexpr inline auto compute_orthogonal_complement(tensor1 const& W, tensor1& U, tensor1& V) const noexcept;

    constexpr inline auto compute_eigenvector0(value_type const a00, value_type const a01, value_type const a02, value_type const a11, value_type const a12, value_type const a22, value_type const eval0, tensor1& evec0) const noexcept;

    constexpr inline auto compute_eigenvector1(value_type const a00, value_type const a01, value_type const a02, value_type const a11, value_type const a12, value_type const a22, tensor1 const& evec0, value_type const eval1, tensor1& evec1) const noexcept;

    constexpr inline auto sort_eigenvalues(int sortType, bool isRotation, bool eigenvectors)noexcept;

    static constexpr inline auto cross(value_type *data, value_type const*const data_lhs, value_type const*const data_rhs)noexcept;

    static constexpr inline auto dot(value_type const*const data_lhs, value_type const*const data_rhs)noexcept;

    static constexpr inline auto mul(value_type *data, value_type const*const data_lhs, value_type const*const data_rhs)noexcept;


    tensor2                     _data;
    std::array<value_type, Dim> _eigval;
    std::array<tensor1, Dim>    _eigvectors;
    std::array<tensor2, Dim>    _eig_basis;
    std::array<size_type, Dim> _non_repeated_eigenvalues;
    std::array<size_type, Dim>  _permut;
    bool                        _all_repeated_eigenvalues;
    bool                        _pair_repeated_eigenvalues;
    bool                        _is_init;
    _Tensor                     _data_base;
};

} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_BONES_H
