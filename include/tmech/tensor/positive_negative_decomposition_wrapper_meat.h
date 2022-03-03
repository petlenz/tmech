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
#ifndef POSITVE_NEGATIVE_DECOMPOSITION_WRAPPER_MEAT_H
#define POSITVE_NEGATIVE_DECOMPOSITION_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename Tensor>
positive_negative_decomposition_wrapper<Tensor>::positive_negative_decomposition_wrapper(data_type_tensor const& data):
    is_pos(false),
    is_neg(false),
    is_dpos(false),
    is_dneg(false),
    data_pos(),
    data_neg(),
    deriv_pos(),
    deriv_neg(),
    Gij(),
    eigen_decomposition(data)
{}

/**
 * Copy constructor
 */
template <typename Tensor>
positive_negative_decomposition_wrapper<Tensor>::positive_negative_decomposition_wrapper(positive_negative_decomposition_wrapper const& data):
    is_pos(data.is_pos),
    is_neg(data.is_neg),
    is_dpos(data.is_dpos),
    is_dneg(data.is_dneg),
    data_pos(data.data_pos),
    data_neg(data.data_neg),
    deriv_pos(data.deriv_pos),
    deriv_neg(data.deriv_neg),
    Gij(data.Gij),
    eigen_decomposition(data.eigen_decomposition)
{}
//@}


//template <typename Tensor>
//constexpr inline auto positive_negative_decomposition_wrapper<Tensor>::decompose(){
//    eigen_decomposition.decompose_eigenbasis();
//    decompose_details<positive_part>(data_pos);
//    decompose_details<negative_part>(data_neg);
//    return std::make_tuple(data_pos,data_neg);
//}

//template <typename Tensor>
//constexpr inline auto positive_negative_decomposition_wrapper<Tensor>::derivative(){
//    eigen_decomposition.decompose_eigenbasis();
//    evaluate_Gij<positive_part>();
//    derivative_details(deriv_pos);
//    evaluate_Gij<negative_part>();evaluate_Gij<positive_part>();
//    derivative_details(deriv_pos);
//    derivative_details(deriv_neg);
//    return std::make_tuple(deriv_pos, deriv_neg);
//}

template <typename Tensor>
template <typename Func>
constexpr inline auto positive_negative_decomposition_wrapper<Tensor>::evaluate_Gij(){
    const auto& eig_values{eigen_decomposition.eigenvalues()};
    const auto& eig_basis{eigen_decomposition.eigenbasis()};
    const auto& non_repeated_eigenvalues{eigen_decomposition.non_repeated_eigenvalues_index()};
    const auto all_repeated_eigenvalues{eigen_decomposition.all_eigenvalues_repeated()};
    const auto pair_repeated_eigenvalues{eigen_decomposition.pair_eigenvalues_repeated()};

    const size_type rows{(all_repeated_eigenvalues ? 1ul : (pair_repeated_eigenvalues ? 2ul : (Tensor::dimension() == 3ul ? 3ul : 2ul)))};
    for(size_type i{0}; i<rows; ++i){
        for(size_type j{0}; j<rows; ++j){
            if(i == j){
                Gij[i][j] = Func::derivative(eig_values[non_repeated_eigenvalues[i]]);
            }else{const auto all_repeated_eigenvalues{eigen_decomposition.all_eigenvalues_repeated()};
                const auto lambda_i{eig_values[non_repeated_eigenvalues[i]]}, lambda_j{eig_values[non_repeated_eigenvalues[j]]};
                Gij[i][j] = (Func::apply(lambda_i) - Func::apply(lambda_j))/(lambda_i - lambda_j);
            }
        }
    }
}

template <typename Tensor>
template <typename Data>
constexpr inline auto positive_negative_decomposition_wrapper<Tensor>::derivative_details(Data & data){
    const auto& eig_values{eigen_decomposition.eigenvalues()};
    const auto& eig_basis{eigen_decomposition.eigenbasis()};
    const auto& non_repeated_eigenvalues{eigen_decomposition.non_repeated_eigenvalues_index()};
    const auto all_repeated_eigenvalues{eigen_decomposition.all_eigenvalues_repeated()};
    const auto pair_repeated_eigenvalues{eigen_decomposition.pair_eigenvalues_repeated()};

    const size_type rows{(all_repeated_eigenvalues ? 1ul : (pair_repeated_eigenvalues ? 2ul : (data_type_tensor::dimension() == 3ul ? 3ul : 2ul)))};

    data.fill(0);
    for(size_type i{0}; i<rows; ++i){
        const auto ii{non_repeated_eigenvalues[i]};
        for(size_type j{0}; j<rows; ++j){
            const auto jj{non_repeated_eigenvalues[j]};
            data += Gij[i][j]*0.5*(otimes(eig_basis[ii], eig_basis[jj]) + boxtimes(eig_basis[ii], eig_basis[jj]));
        }
    }
}

template <typename Tensor>
template <typename Func, typename Data>
constexpr inline auto positive_negative_decomposition_wrapper<Tensor>::decompose_details(Data & data){
    const auto& eig_values{eigen_decomposition.eigenvalues()};
    const auto& eig_basis{eigen_decomposition.eigenbasis()};
    const auto& non_repeated_eigenvalues{eigen_decomposition.non_repeated_eigenvalues_index()};

    if constexpr (data_type_tensor::dimension() == 2){
        if(eigen_decomposition.all_eigenvalues_repeated()){
            data = Func::apply(eig_values[0])*eig_basis[0];
        }else{
            data = Func::apply(eig_values[0])*eig_basis[0] + Func::apply(eig_values[1])*eig_basis[1];
        }
    } else if constexpr (data_type_tensor::dimension() == 3){
        if(eigen_decomposition.all_eigenvalues_repeated()){
            data = Func::apply(eig_values[0])*eig_basis[0];

        }else if (eigen_decomposition.pair_eigenvalues_repeated()) {
            data =   Func::apply(eig_values[non_repeated_eigenvalues[0]])*eig_basis[non_repeated_eigenvalues[0]]
                    + Func::apply(eig_values[non_repeated_eigenvalues[1]])*eig_basis[non_repeated_eigenvalues[1]];

        }else{
            data =   Func::apply(eig_values[0])*eig_basis[0]
                    + Func::apply(eig_values[1])*eig_basis[1]
                    + Func::apply(eig_values[2])*eig_basis[2];
        }
    }
}

} // NAMESPACE DETAIL
#endif // POSITVE_NEGATIVE_DECOMPOSITION_WRAPPER_MEAT_H
