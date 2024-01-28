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
#include <experimental/type_traits>
#include <immintrin.h>
#include <complex>
#include <cstring>
#include <string>


#include "tmech_forward_definitions.h"
#include "tensor/tensor_operators.h"

#include <boost/math/tools/roots.hpp>
namespace boost::math::tools {

// Solves ax^3 + bx^2 + cx + d = 0.
// Only returns the real roots, as types get weird for real coefficients and
// complex roots. Follows Numerical Recipes, Chapter 5, section 6. NB: A better
// algorithm apparently exists: Algorithm 954: An Accurate and Efficient Cubic
// and Quartic Equation Solver for Physical Applications However, I don't have
// access to that paper!
template <typename Real>
std::array<Real, 3> cubic_roots(Real a, Real b, Real c, Real d) {
    using std::abs;
    using std::acos;
    using std::cbrt;
    using std::cos;
    using std::fma;
    using std::sqrt;
    std::array<Real, 3> roots = {std::numeric_limits<Real>::quiet_NaN(),
                                 std::numeric_limits<Real>::quiet_NaN(),
                                 std::numeric_limits<Real>::quiet_NaN()};
    if (a == 0) {
        // bx^2 + cx + d = 0:
        if (b == 0) {
            // cx + d = 0:
            if (c == 0) {
                if (d != 0) {
                    // No solutions:
                    return roots;
                }
                roots[0] = 0;
                roots[1] = 0;
                roots[2] = 0;
                return roots;
            }
            roots[0] = -d / c;
            return roots;
        }
        auto [x0, x1] = quadratic_roots(b, c, d);
        roots[0] = x0;
        roots[1] = x1;
        return roots;
    }
    if (d == 0) {
        auto [x0, x1] = quadratic_roots(a, b, c);
        roots[0] = x0;
        roots[1] = x1;
        roots[2] = 0;
        std::sort(roots.begin(), roots.end());
        return roots;
    }
    Real p = b / a;
    Real q = c / a;
    Real r = d / a;
    Real Q = (p * p - 3 * q) / 9;
    Real R = (2 * p * p * p - 9 * p * q + 27 * r) / 54;
    if (R * R < Q * Q * Q) {
        Real rtQ = sqrt(Q);
        Real theta = acos(R / (Q * rtQ)) / 3;
        Real st = sin(theta);
        Real ct = cos(theta);
        roots[0] = -2 * rtQ * ct - p / 3;
        roots[1] = -rtQ * (-ct + sqrt(Real(3)) * st) - p / 3;
        roots[2] = rtQ * (ct + sqrt(Real(3)) * st) - p / 3;
    } else {
        // In Numerical Recipes, Chapter 5, Section 6, it is claimed that we
        // only have one real root if R^2 >= Q^3. But this isn't true; we can
        // even see this from equation 5.6.18. The condition for having three
        // real roots is that A = B. It *is* the case that if we're in this
        // branch, and we have 3 real roots, two are a double root. Take
        // (x+1)^2(x-2) = x^3 - 3x -2 as an example. This clearly has a double
        // root at x = -1, and it gets sent into this branch.
        Real arg = R * R - Q * Q * Q;
        Real A = (R >= 0 ? -1 : 1) * cbrt(abs(R) + sqrt(arg));
        Real B = 0;
        if (A != 0) {
            B = Q / A;
        }
        roots[0] = A + B - p / 3;
        // Yes, we're comparing floats for equality:
        // Any perturbation pushes the roots into the complex plane; out of the
        // bailiwick of this routine.
        if (A == B || arg == 0) {
            roots[1] = -A - p / 3;
            roots[2] = -A - p / 3;
        }
    }
    // Root polishing:
    for (auto &r : roots) {
        // Horner's method.
        // Here I'll take John Gustaffson's opinion that the fma is a *distinct*
        // operation from a*x +b: Make sure to compile these fmas into a single
        // instruction and not a function call! (I'm looking at you Windows.)
        Real f = fma(a, r, b);
        f = fma(f, r, c);
        f = fma(f, r, d);
        Real df = fma(3 * a, r, 2 * b);
        df = fma(df, r, c);
        if (df != 0) {
            Real d2f = fma(6 * a, r, 2 * b);
            Real denom = 2 * df * df - f * d2f;
            if (denom != 0) {
                r -= 2 * f * df / denom;
            } else {
                r -= f / df;
            }
        }
    }
    std::sort(roots.begin(), roots.end());
    return roots;
}

}


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
