// Copyright 2021 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------

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

    eigen_decomposition_wrapper(_Tensor const& data);

    eigen_decomposition_wrapper(eigen_decomposition_wrapper const& data);

    constexpr inline auto decompose();

    constexpr inline auto decompose_eigenbasis();

    constexpr inline auto eigenvalues()const;

    constexpr inline auto eigenvectors()const;

    constexpr inline auto eigenbasis()const;

    constexpr inline auto all_eigenvalues_repeated()const;

    constexpr inline auto pair_eigenvalues_repeated()const;

    constexpr inline auto non_repeated_eigenvalues_index()const;

    constexpr inline auto evaluate_eigenbasis();

    constexpr inline size_type number_non_repeated_eigenvalues()const;

    constexpr inline auto permutation()const;

private:
    constexpr inline auto get_repeated_eigenvalues();

    constexpr inline auto evaluate_detail_2D(bool eigenvectors, value_type const _a00, value_type const _a01, value_type const _a10, value_type const _a11);

    constexpr inline auto evaluate_detail_3D(bool eigenvectors, value_type const _a00, value_type const _a01, value_type const _a02, value_type const _a11, value_type const _a12, value_type const _a22);

    constexpr inline auto compute_orthogonal_complement(tensor1 const& W, tensor1& U, tensor1& V) const;

    constexpr inline auto compute_eigenvector0(value_type const a00, value_type const a01, value_type const a02, value_type const a11, value_type const a12, value_type const a22, value_type const eval0, tensor1& evec0) const;

    constexpr inline auto compute_eigenvector1(value_type const a00, value_type const a01, value_type const a02, value_type const a11, value_type const a12, value_type const a22, tensor1 const& evec0, value_type const eval1, tensor1& evec1) const;

    constexpr inline auto sort_eigenvalues(int sortType, bool isRotation, bool eigenvectors);

    static constexpr inline auto cross(value_type *data, value_type const*const data_lhs, value_type const*const data_rhs);

    static constexpr inline auto dot(value_type const*const data_lhs, value_type const*const data_rhs);

    static constexpr inline auto mul(value_type *data, value_type const*const data_lhs, value_type const*const data_rhs);


    tensor2                     _data;
    std::array<value_type, Dim> _eigval;
    std::array<tensor1, Dim>    _eigvectors;
    std::array<tensor2, Dim>    _eig_basis;
    std::array<size_type, Dim> _non_repeated_eigenvalues;
    std::array<size_type, Dim>  _permut;
    bool                        _all_repeated_eigenvalues;
    bool                        _pair_repeated_eigenvalues;
    _Tensor                     _data_base;
};

} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_BONES_H
