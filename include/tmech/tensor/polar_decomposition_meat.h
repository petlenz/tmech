/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_MEAT_H
#define POLAR_DECOMPOSITION_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename _Tensor>
constexpr polar_decomposition_wrapper<_Tensor>::polar_decomposition_wrapper(data_type_tensor const& __data, bool const __newton_method, value_type const __tol, size_type const __max_steps)noexcept:
    _is_init(false),
    _is_init_deriv(false),
    newton_method(__newton_method),
    newton_tol(__tol),
    max_steps(__max_steps),
    _U(),
    _V(),
    _R(),
    _dR(),
    _dU(),
    _dV(),
    _data(__data)
{}

/**
 * @brief Copy constructor
 */
template <typename _Tensor>
constexpr polar_decomposition_wrapper<_Tensor>::polar_decomposition_wrapper(polar_decomposition_wrapper const& __data)noexcept:
    _is_init(false),
    _is_init_deriv(false),
    newton_method(__data.newton_method),
    newton_tol(__data.newton_tol),
    max_steps(__data.max_steps),
    _U(),
    _V(),
    _R(),
    _dR(),
    _dU(),
    _dV(),
    _data(__data)
{}
//@}

/**
* @name Decomposed tensors accsess
*/
/**
* @brief Returns the orthogonal tensor \f$\SecondT{R}\f$
* from the decomposition \f$\SecondT{F} = \SecondT{R}\SecondT{U}\f$.
*/
template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::R()const noexcept{
    return polar_decomposition_R_wrapper<this_type, tensor2>(*this, _R);
}

/**
* @brief Returns the symmetric tensor \f$\SecondT{U}\f$
* from the decomposition \f$\SecondT{F} = \SecondT{R}\SecondT{U}\f$.
*/
template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::U()const noexcept{
    return polar_decomposition_U_wrapper<this_type, tensor2>(*this, _U);
}

/**
* @brief Returns the symmetric tensor \f$\SecondT{V}\f$
* from the decomposition \f$\SecondT{F} = \SecondT{V}\SecondT{R}\f$.
*/
template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::V()const noexcept{
    return polar_decomposition_V_wrapper<this_type, tensor2>(*this, _V);
}
//@}

template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::evaluate_newton()noexcept{
    // F = R*U = V*R
    set_up_R_newton();
    _U = trans(_R)*_data;
    _V = _data*trans(_R);
}

template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::evaluate_derivatives()noexcept{
    evaluate();
    if(!_is_init_deriv){
        if constexpr (data_type_tensor::dimension() == 2){
            evaluate_derivatives_2D();
        }else{
            evaluate_derivatives_3D();
        }
        _is_init_deriv = true;
    }
}
template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::evaluate_derivatives_2D()noexcept{
    const auto I{eye<value_type, Dim, Rank>()};
    const auto Iu{trace(_U)};
    const auto Iv{trace(_V)};
    //boxtime     := ikjl := otimesu
    //hatboxtimes := iljk := otimesl
    //II = otimesu(1,1)
    //boxtimes(A,B):boxtimes(C,D) = boxtimes(A*C,B*D)
    //Eq. (44)
    _dU = otimesu(trans(_R), I) - dcontract(otimesu(I, _U), otimesu(trans(_R),I) - otimesl(I,trans(_R)))/Iu;
    //Eq. (45)
    _dV = otimesu(I,_R) - dcontract(otimesu(_V,I), otimesu(I,_R) - otimesl(_R,I))/Iv;
    //Eq. (46)
    _dR = (otimesu(I, I) - otimesl(_R, trans(_R)))/Iu;
}

template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::evaluate_derivatives_3D()noexcept{
    const auto I{eye<value_type, Dim, Rank>()};
    const tensor<value_type, 3, 2> U_tilde{trace(_U)*I - _U};
    const tensor<value_type, 3, 2> V_tilde{trace(_V)*I - _V};
    const auto inv_detV{1./det(V_tilde)};
    const auto inv_detU{1./det(U_tilde)};
    const auto B_U{otimesu(U_tilde, U_tilde)};
    const auto B_V{otimesu(V_tilde, V_tilde)};
    //Eq. (40)
    _dU = otimesu(trans(_R),I) - inv_detU*dcontract(otimesu(I,_U), dcontract(B_U, otimesu(trans(_R),I) - otimesl(I,trans(_R))));
    //Eq. (41)
    _dV = otimesu(I,_R) - inv_detV*dcontract(otimesu(_V,I), dcontract(B_V, otimesu(I,_R) - otimesl(_R,I)));
    //Eq. (42)
    _dR = inv_detU*dcontract(otimesu(_R,I), dcontract(B_U, otimesu(trans(_R),I) - otimesl(I,trans(_R))));
}

template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::evaluate()noexcept{
    if(!_is_init){
        if(newton_method){
            evaluate_newton();
        }else{
            evaluate_eigen();
        }
        _is_init = true;
    }
}

/**
* @name Polar decomposition implementation
*/
//@{
/**
* @brief Newton-Raphson iterative polar decomposition procedure
* \f[
*  \SecondT{R}^{k+1} =  \left(\SecondT{R}^{k} + (\SecondT{R}^{k})^{-1}\right)\frac{1}{2}.
* \f]
* A detailed describtion is given <a href="https://en.wikipedia.org/wiki/Polar_decomposition">here</a>.
*/
template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::set_up_R_newton()noexcept{
    size_type iter{0};
    tensor<value_type, Dim, Rank> R_old;
    _R = _data;
    while (true) {
        R_old = _R;
        _R = (_R + trans(inv(_R)))*0.5;

        const auto error{norm(R_old - _R)/norm(_R)};
        if(error <= newton_tol){break;}

        if(iter == max_steps){
            break;
        }
        ++iter;
    }
}


// F = R*U = V*R
// !is_R && !is_U && !is_V
// copy _data into F
// set_up_U() --> R = F*inv(U) --> V = R*U*trans(R)

// use R U or V
// if newton == true
// set_up_R()
// U = R^T*F
// V = F*V^T
// if newton == false
// eigen_decomposition _data^T*_data
// set_up_U_eigen() U = sum_i sqrt(lambda_i)*E_i
// Uinv = sum_i (1/lambda_i)*E_i
// R = F*invU
// set_up_V_eigen() V = R*U*R^T
template <typename _Tensor>
constexpr inline auto polar_decomposition_wrapper<_Tensor>::evaluate_eigen()noexcept{
    auto eigen{eigen_decomposition(trans(_data)*_data)};
    const auto [eigenvalues, eigenbasis]{eigen.decompose_eigenbasis()};
    tensor2 Uinv;
    const auto& idx{eigen.non_repeated_eigenvalues_index()};
    for(size_type i{0}; i<eigen.number_non_repeated_eigenvalues(); ++i){
        const auto val{std::sqrt(eigenvalues[idx[i]])};
        _U += val*eigenbasis[idx[i]];
        Uinv += (1.0/val)*eigenbasis[idx[i]];
    }
    _R = _data*Uinv;
    _V = _R*_U*trans(_R);
    _is_init = true;
}

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_MEAT_H
