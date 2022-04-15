/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TMECH_FORWARD_DEFINITIONS_H
#define TMECH_FORWARD_DEFINITIONS_H

namespace tmech {
namespace detail {
template <typename _T>
class abs_tensor_wrapper;

template <typename _LHS, typename _RHS, typename _OP>
class tensor_binary_expression_wrapper;

template <typename _Tensor, typename _Sequence>
class basis_change_wrapper;

template <typename _Father>
class sign_S_wrapper;

template <typename _Father>
class sign_N_wrapper;

template <typename _Tensor>
class adjoint_wrapper;

template <typename _Tensor, typename ..._Sequences>
class inverse_wrapper;

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
class inner_product_wrapper;

template <typename _Tensor>
class eval_tensor_wrapper;

template <typename _LHS, typename _RHS, typename _SequenceLHS, typename _SequenceRHS>
class outer_product_wrapper;

template <typename Tensor>
class symmetric_part_wrapper;

template <typename _Tensor>
class cofactor_wrapper;

template<typename _LHS, typename _RHS>
class cross_product_wrapper;

template <typename _Tensor>
class skew_symmetric_part_wrapper;

template <typename _Tensor>
class deviatoric_wrapper;

template <typename _Tensor>
class volumetric_wrapper;

template <typename _Tensor, typename _Function>
class isotropic_tensor_function;

template <typename _Base, typename _Tensor>
class polar_decomposition_R_wrapper;

template <typename _Base, typename _Tensor>
class polar_decomposition_U_wrapper;

template <typename _Base, typename _Tensor>
class polar_decomposition_V_wrapper;

template <typename _Base, typename _Tensor>
class polar_decomposition_dR_wrapper;

template <typename _Base, typename _Tensor>
class polar_decomposition_dU_wrapper;

template <typename _Base, typename _Tensor>
class polar_decomposition_dV_wrapper;

template <typename _Tensor>
class exp_tensor_wrapper;

template <typename _Father, typename _Tensor>
class exp_derivative_tensor_wrapper;

template <typename _Base, typename _Tensor>
class positive_negative_decomposition_neg_wrapper;

template <typename _Base, typename _Tensor>
class positive_negative_decomposition_pos_wrapper;

template <typename _Base, typename _Tensor>
class positive_negative_decomposition_dneg_wrapper;

template <typename _Base, typename _Tensor>
class positive_negative_decomposition_dpos_wrapper;

template <typename _Tensor>
class negative_tensor_wrapper;

template <typename _Tensor>
class pow_tensor_wrapper;

template <typename _Base, typename _Tensor>
class pow_tensor_derivative_wrapper;
}

//forward declaration
template <typename _Derived>
class tensor_base;

template <typename _T, std::size_t _Dim, std::size_t _Rank>
class tensor;

template <typename _T, std::size_t _Dim, std::size_t _Rank>
class randn;

template <typename T, std::size_t Dim>
class levi_civita;

template <typename T, std::size_t Dim, std::size_t Rank>
class eye;

template <typename T, std::size_t Dim, std::size_t Rank>
class zeros;

template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
class adaptor;

}


namespace symdiff {
#include "symdiff/forward_definitions.h"
}

#endif // TMECH_FORWARD_DEFINITIONS_H
