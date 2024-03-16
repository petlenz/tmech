/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TMECH_H
#define TMECH_H

#include <math.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <array>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <random>
#include <functional>
#include <tuple>
#include <any>
#include <map>
#include <immintrin.h>
#include <complex>
#include <cstring>
#include <string>


#ifdef TMECH_IS_DETECTED
namespace std {
namespace experimental {
namespace detail {
template <class AlwaysVoid, template<class...> class Op, class... Args>
struct detector {
  using value_t = std::false_type;
};

template <template<class...> class Op, class... Args>
struct detector<std::void_t<Op<Args...>>, Op, Args...> {
  using value_t = std::true_type;
};
} // namespace detail

template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
const auto is_detected_v = is_detected<Op, Args...>::value;
}
}
#else
#include <experimental/type_traits>
#endif



#include "tmech_forward_definitions.h"
#include "tensor/tensor_operators.h"

namespace tmech {

#include "tmech_config.h"

#include "tensor/scalar_bones.h"
#include "tensor/scalar_meat.h"

#include "tensor/traits.h"
#include "tensor/utility.h"

#include "tensor/compare_lambda.h"
#include "tensor/compare_almost_equal.h"

#include "tensor/tensor_base_bones.h"
#include "tensor/tensor_base_meat.h"

#include "tensor/gemm_wrapper_bones.h"
#include "tensor/gemm_wrapper_meat.h"
#include "tensor/dot_wrapper_bones.h"
#include "tensor/dot_wrapper_meat.h"

#include "tensor/convert_tensor_to_voigt.h"

#include "tensor/adaptor_wrapper_bones.h"
#include "tensor/adaptor_wrapper_meat.h"

//tensor_functions_forward
#include "tensor/tensor_functions_forward.h"

//special tensors
#include "tensor/identity_tensor_bones.h"
#include "tensor/identity_tensor_meat.h"
#include "tensor/zero_tensor_bones.h"
#include "tensor/zero_tensor_meat.h"
#include "tensor/levi_civita_symbol_bones.h"
#include "tensor/levi_civita_symbol_meat.h"
//

#include "tensor/evaluation_temp_bones.h"
#include "tensor/evaluation_temp_meat.h"
#include "tensor/expression_operators.h"
#include "tensor/math_details.h"
#include "tensor/skew_symmetric_part_wrapper_bones.h"
#include "tensor/skew_symmetric_part_wrapper_meat.h"


#include "tensor/symmetric_part_wrapper_bones.h"
#include "tensor/symmetric_part_wrapper_meat.h"

#include "tensor/tensor_expression_wrapper_bones.h"
#include "tensor/tensor_expression_wrapper_meat.h"

#include "tensor/inverse_wrapper_bones.h"
#include "tensor/inverse_wrapper_meat.h"
#include "tensor/volumetric_wrapper_bones.h"
#include "tensor/volumetric_wrapper_meat.h"
#include "tensor/pow_tensor_wrapper_bones.h"
#include "tensor/pow_tensor_wrapper_meat.h"
#include "tensor/pow_tensor_derivative_wrapper_bones.h"
#include "tensor/pow_tensor_derivative_wrapper_meat.h"

#include "tensor/numerical_differentiation_central_wrapper_bones.h"
#include "tensor/numerical_differentiation_central_wrapper_meat.h"

#include "tensor/numerical_differentiation_central_sym_wrapper_bones.h"
#include "tensor/numerical_differentiation_central_sym_wrapper_meat.h"

//tensor contractions
#include "tensor/meta_inner_product_wrapper_bones.h"
#include "tensor/inner_product_wrapper_bones.h"
#include "tensor/inner_product_wrapper_meat.h"

//tensor products
#include "tensor/outer_product_wrapper_bones.h"
#include "tensor/outer_product_wrapper_meat.h"

//special tensor function wrapper
#include "tensor/negative_tensor_wrapper_bones.h"
#include "tensor/negative_tensor_wrapper_meat.h"
#include "tensor/abs_tensor_wrapper_bones.h"
#include "tensor/abs_tensor_wrapper_meat.h"
//

#include "tensor/cross_product_wrapper_bones.h"
#include "tensor/cross_product_wrapper_meat.h"

#include "tensor/deviatoric_wrapper_bones.h"
#include "tensor/deviatoric_wrapper_meat.h"

//basis rearrangement
#include "tensor/basis_change_wrapper_bones.h"
#include "tensor/basis_change_wrapper_meat.h"

#include "tensor/cofactor_wrapper_bones.h"
#include "tensor/cofactor_wrapper_meat.h"

#include "tensor/adjoint_wrapper_bones.h"
#include "tensor/adjoint_wrapper_meat.h"

#include "tensor/eigen_decomposition_bones.h"
#include "tensor/eigen_decomposition_meat.h"

#include "tensor/isotropic_tensor_functions_bones.h"
#include "tensor/isotropic_tensor_functions_meat.h"


//tensor based functions
#include "tensor/general_lu_decomposition_bones.h"
#include "tensor/general_lu_decomposition_meat.h"

#include "tensor/general_newton_bones.h"
#include "tensor/general_newton_meat.h"

#include "tensor/exp_wrapper_bones.h"
#include "tensor/exp_wrapper_meat.h"

#include "tensor/sign_tensor_wrapper_bones.h"
#include "tensor/sign_tensor_wrapper_meat.h"

#include "tensor/polar_decomposition_dR_wrapper_bones.h"
#include "tensor/polar_decomposition_dR_wrapper_meat.h"
#include "tensor/polar_decomposition_dU_wrapper_bones.h"
#include "tensor/polar_decomposition_dU_wrapper_meat.h"
#include "tensor/polar_decomposition_dV_wrapper_bones.h"
#include "tensor/polar_decomposition_dV_wrapper_meat.h"
#include "tensor/polar_decomposition_R_wrapper_bones.h"
#include "tensor/polar_decomposition_R_wrapper_meat.h"
#include "tensor/polar_decomposition_U_wrapper_bones.h"
#include "tensor/polar_decomposition_U_wrapper_meat.h"
#include "tensor/polar_decomposition_V_wrapper_bones.h"
#include "tensor/polar_decomposition_V_wrapper_meat.h"
#include "tensor/polar_decomposition_bones.h"
#include "tensor/polar_decomposition_meat.h"

#include "tensor/positive_negative_decomposition_dpos_wrapper_bones.h"
#include "tensor/positive_negative_decomposition_dpos_wrapper_meat.h"
#include "tensor/positive_negative_decomposition_dneg_wrapper_bones.h"
#include "tensor/positive_negative_decomposition_dneg_wrapper_meat.h"
#include "tensor/positive_negative_decomposition_pos_wrapper_bones.h"
#include "tensor/positive_negative_decomposition_pos_wrapper_meat.h"
#include "tensor/positive_negative_decomposition_neg_wrapper_bones.h"
#include "tensor/positive_negative_decomposition_neg_wrapper_meat.h"
#include "tensor/positive_negative_decomposition_wrapper_bones.h"
#include "tensor/positive_negative_decomposition_wrapper_meat.h"

#include "tensor/randn_wrapper_bones.h"
#include "tensor/randn_wrapper_meat.h"

#include "tensor/randu_wrapper_bones.h"
#include "tensor/randu_wrapper_meat.h"

#include "tensor/tensor_functions_implementation.h"

#include "tensor/tensor_bones.h"
#include "tensor/tensor_meat.h"

} // NAMESPACE TMECH





namespace tmech {
#include "symdiff/tmech_functions_forward.h"
}

namespace symdiff {
#include "symdiff/forward_definitions.h"
#include "symdiff/symdiff_functions_forward.h"
#include "symdiff/symdiff_type_traits.h"
#include "symdiff/symdiff_utility.h"
#include "symdiff/std_functions_wrapper_bones.h"
#include "symdiff/tmech_function_wrapper.h"
}

#include "symdiff/std_functions_implementation.h"

namespace tmech {
#include "symdiff/tmech_functions_implementation.h"
}

namespace symdiff {

#include "symdiff/variable_base_bones.h"
#include "symdiff/variable_base_meat.h"

#include "symdiff/jacobi_matrix_wrapper.h"
//#include "symdiff/newton_solver_multi_variable_bones.h"
//#include "symdiff/newton_solver_single_variable_bones.h"
#include "symdiff/vector_of_functions.h"
#include "symdiff/newton_raphson_solver_bones.h"


#include "symdiff/implicit_function_bones.h"
#include "symdiff/implicit_nonlinear_function_bones.h"


//fundamental results
#include "symdiff/scalar_one_bones.h"
#include "symdiff/scalar_one_meat.h"
#include "symdiff/scalar_zero_bones.h"
#include "symdiff/scalar_zero_meat.h"
#include "symdiff/real_constant_bones.h"
#include "symdiff/real_constant_meat.h"


#include "symdiff/tensor_one_bones.h"
#include "symdiff/tensor_one_meat.h"
#include "symdiff/tensor_zero_bones.h"
#include "symdiff/tensor_zero_meat.h"

#include "symdiff/variable_bones.h"
#include "symdiff/variable_meat.h"

#include "symdiff/binary_expression_wrapper_bones.h"
#include "symdiff/binary_expression_wrapper_meat.h"

#include "symdiff/symdiff_operators.h"

#include "symdiff/squeezer.h"

#include "symdiff/derivative_wrapper_bones.h"
#include "symdiff/derivative_wrapper_meat.h"

#include "symdiff/symdiff_functions_forward.h"
#include "symdiff/symdiff_functions_implementation.h"

#include "symdiff/constant_bones.h"
#include "symdiff/constant_meat.h"

#include "symdiff/function_wrapper_bones.h"
#include "symdiff/function_wrapper_meat.h"

#include "symdiff/tensor_function_wrapper_bones.h"
#include "symdiff/tensor_function_wrapper_meat.h"


#include "symdiff/negative_bones.h"
#include "symdiff/negative_meat.h"
#include "symdiff/tensor_outer_product_wrapper_bones.h"
#include "symdiff/tensor_outer_product_wrapper_meat.h"
#include "symdiff/tensor_inner_product_wrapper_bones.h"
#include "symdiff/tensor_inner_product_wrapper_meat.h"
#include "symdiff/tensor_basis_change_wrapper_bones.h"
#include "symdiff/tensor_basis_change_wrapper_meat.h"
#include "symdiff/tensor_inv_bones.h"
#include "symdiff/tensor_inv_meat.h"
#include "symdiff/tensor_isotropic_function_wrapper_bones.h"
#include "symdiff/tensor_isotropic_function_wrapper_meat.h"
#include "symdiff/tensor_isotropic_function_wrapper_derivative_bones.h"
#include "symdiff/tensor_isotropic_function_wrapper_derivative_meat.h"
#include "symdiff/tensor_as_sym_wrapper_bones.h"
#include "symdiff/tensor_as_sym_wrapper_meat.h"
#include "symdiff/tensor_pow_wrapper_bones.h"
#include "symdiff/tensor_pow_wrapper_meat.h"
#include "symdiff/tensor_pow_derivative_wrapper_bones.h"
#include "symdiff/tensor_pow_derivative_wrapper_meat.h"
#include "symdiff/tensor_dev_wrapper_bones.h"
#include "symdiff/tensor_dev_wrapper_meat.h"
#include "symdiff/tensor_vol_wrapper_bones.h"
#include "symdiff/tensor_vol_wrapper_meat.h"
} // NAMESPACE SYMDIFF

#include "symdiff/tensor_fourth_order_identity_wrapper_bones.h"
#include "symdiff/tensor_fourth_order_identity_wrapper_meat.h"


#endif // TMECH_H
