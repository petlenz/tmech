/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ISOTROPIC_TENSOR_FUNCTIONS_MEAT_H
#define ISOTROPIC_TENSOR_FUNCTIONS_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename Tensor, typename Func>
constexpr isotropic_tensor_function<Tensor, Func>::isotropic_tensor_function(Tensor data)noexcept:
    _data(),
    _deriv(),
    _Gij(),
    _eigen_decomposition(data)
{}

/**
 * @brief Copy constructor
 */
template <typename Tensor, typename Func>
constexpr isotropic_tensor_function<Tensor, Func>::isotropic_tensor_function(isotropic_tensor_function const& data)noexcept:
    _data(),
    _deriv(),
    _Gij(),
    _eigen_decomposition(data._eigen_decomposition)
{}
//@}


template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::operator()(size_type const i, size_type const j)const noexcept{
    return _data(i,j);
}

/**
* @name Dimension and rank
*/
/**
* @brief Returns the dimension.
*/
template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::dimension()noexcept{
    return data_type_tensor::dimension();
}
/**
* @brief Returns the rank.
*/
template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::rank()noexcept{
    return data_type_tensor::rank();
}
//@}

template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::raw_data()const noexcept{
    return _data.raw_data();
}

template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::derivative()noexcept{
    _eigen_decomposition.decompose_eigenbasis();
    const auto eig_values{_eigen_decomposition.eigenvalues()};
    const auto eig_basis{_eigen_decomposition.eigenbasis()};
    const auto non_repeated_eigenvalues{_eigen_decomposition.non_repeated_eigenvalues_index()};
    const auto all_repeated_eigenvalues{_eigen_decomposition.all_eigenvalues_repeated()};
    const auto pair_repeated_eigenvalues{_eigen_decomposition.pair_eigenvalues_repeated()};

    const size_type rows{(all_repeated_eigenvalues ? 1ul : (pair_repeated_eigenvalues ? 2ul : (dimension() == 3ul ? 3ul : 2ul)))};

    evaluate_Gij();

    _deriv.fill(0);
    for(size_type i{0}; i<rows; ++i){
        const auto ii{non_repeated_eigenvalues[i]};
        for(size_type j{0}; j<rows; ++j){
            const auto jj{non_repeated_eigenvalues[j]};
            _deriv += _Gij[i][j]*0.5*(otimesu(eig_basis[ii], eig_basis[jj]) + otimesl(eig_basis[ii], eig_basis[jj]));
        }
    }
    return _deriv;
}

template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::evaluate()noexcept{
    _eigen_decomposition.decompose_eigenbasis();
    const auto eig_values{_eigen_decomposition.eigenvalues()};
    const auto eig_basis{_eigen_decomposition.eigenbasis()};
    const auto non_repeated_eigenvalues{_eigen_decomposition.non_repeated_eigenvalues_index()};

    if constexpr (dimension() == 2){
        if(_eigen_decomposition.all_eigenvalues_repeated()){
            _data = Func::apply(eig_values[0])*eig_basis[0];
        }else{
            _data = Func::apply(eig_values[0])*eig_basis[0] + Func::apply(eig_values[1])*eig_basis[1];
        }
    } else if constexpr (dimension() == 3){
        if(_eigen_decomposition.all_eigenvalues_repeated()){
            _data = Func::apply(eig_values[0])*eig_basis[0];

        }else if (_eigen_decomposition.pair_eigenvalues_repeated()) {
            _data =   Func::apply(eig_values[non_repeated_eigenvalues[0]])*eig_basis[non_repeated_eigenvalues[0]]
                    + Func::apply(eig_values[non_repeated_eigenvalues[1]])*eig_basis[non_repeated_eigenvalues[1]];

        }else{
            _data =   Func::apply(eig_values[0])*eig_basis[0]
                    + Func::apply(eig_values[1])*eig_basis[1]
                    + Func::apply(eig_values[2])*eig_basis[2];
        }
    }
}

template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::get_derivative()const noexcept{
    return _deriv;
}

template <typename Tensor, typename Func>
constexpr inline auto isotropic_tensor_function<Tensor, Func>::evaluate_Gij()noexcept{
    const auto eig_values{_eigen_decomposition.eigenvalues()};
    const auto eig_basis{_eigen_decomposition.eigenbasis()};
    const auto non_repeated_eigenvalues{_eigen_decomposition.non_repeated_eigenvalues_index()};
    const auto all_repeated_eigenvalues{_eigen_decomposition.all_eigenvalues_repeated()};
    const auto pair_repeated_eigenvalues{_eigen_decomposition.pair_eigenvalues_repeated()};

    const size_type rows{(all_repeated_eigenvalues ? 1ul : (pair_repeated_eigenvalues ? 2ul : (dimension() == 3ul ? 3ul : 2ul)))};
    for(size_type i{0}; i<rows; ++i){
        for(size_type j{0}; j<rows; ++j){
            if(i == j){
                _Gij[i][j] = Func::derivative(eig_values[non_repeated_eigenvalues[i]]);
            }else{
                //const auto all_repeated_eigenvalues{_eigen_decomposition.all_eigenvalues_repeated()};
                //const auto pair_repeated_eigenvalues{_eigen_decomposition.pair_eigenvalues_repeated()};
                //const size_type rows{(all_repeated_eigenvalues ? 1ul : (pair_repeated_eigenvalues ? 2ul : (dimension() == 3ul ? 3ul : 2ul)))};
                const auto lambda_i{eig_values[non_repeated_eigenvalues[i]]}, lambda_j{eig_values[non_repeated_eigenvalues[j]]};
                _Gij[i][j] = (Func::apply(lambda_i) - Func::apply(lambda_j))/(lambda_i - lambda_j);
            }
        }
    }
}

} // NAMESPACE DETAIL
#endif // ISOTROPIC_TENSOR_FUNCTIONS_MEAT_H
