/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ISOTROPIC_TENSOR_FUNCTIONS_BONES_H
#define ISOTROPIC_TENSOR_FUNCTIONS_BONES_H


namespace detail {

struct sqrt_{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return std::sqrt(value);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return T(1)/(T(2)*std::sqrt(value));
    }

    template<typename  T>
    static constexpr inline auto second_derivative(T const& value){
        return -T(1)/(T(4)*std::pow(value,3.0/2.0));
    }
};

struct log{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return std::log(value);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return T(1)/value;
    }

    template<typename  T>
    static constexpr inline auto second_derivative(T const& value){
        return -T(1)/(value*value);
    }
};

struct exp{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return std::exp(value);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return std::exp(value);
    }

    template<typename  T>
    static constexpr inline auto second_derivative(T const& value){
        return std::exp(value);
    }
};

struct positive_part{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return value >= T(0) ? value : T(0);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return value >= T(0) ? T(1) : T(0);
    }
};

struct negative_part{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return value <= T(0) ? value : T(0);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return value <= T(0) ? T(1) : T(0);
    }
};


template <typename Tensor, typename Func>
class isotropic_tensor_function : public tensor_base<isotropic_tensor_function<Tensor, Func>>
{
    using basetype = tensor_base<isotropic_tensor_function<Tensor, Func>>;
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
    static constexpr auto Dim{data_type_tensor::dimension()};
    static constexpr auto Rank{data_type_tensor::rank()};
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    static_assert(Dim == 2 || Dim == 3, "isotropic_tensor_function: only for Dim = 2, 3");
    static_assert(Rank == 2,            "isotropic_tensor_function: only only for Rank = 2");

    constexpr isotropic_tensor_function(Tensor data)noexcept;

    constexpr isotropic_tensor_function(isotropic_tensor_function const& data)noexcept;

    constexpr inline auto operator()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto derivative()noexcept;

    constexpr inline auto second_derivative()noexcept{
        _eigen_decomposition.decompose_eigenbasis();
        const auto eig_values{_eigen_decomposition.eigenvalues()};
        const auto eig_basis{_eigen_decomposition.eigenbasis()};
        const auto non_repeated_eigenvalues{_eigen_decomposition.non_repeated_eigenvalues_index()};
        const auto all_repeated_eigenvalues{_eigen_decomposition.all_eigenvalues_repeated()};
        const auto pair_repeated_eigenvalues{_eigen_decomposition.pair_eigenvalues_repeated()};
        tensor<value_type, Dim, 6> _deriv;
        const size_type rows{(all_repeated_eigenvalues ? 1ul : (pair_repeated_eigenvalues ? 2ul : (dimension() == 3ul ? 3ul : 2ul)))};


        for(size_type i{0}; i<rows; ++i){
            const auto eigI{eig_values[non_repeated_eigenvalues[i]]};
            _Fi[non_repeated_eigenvalues[i]]   = Func::apply(eigI);
            _dFi[non_repeated_eigenvalues[i]]  = Func::derivative(eigI);
            _ddFi[non_repeated_eigenvalues[i]] = Func::second_derivative(eigI);
        }

        std::array<std::array<std::array<value_type, Dim>, Dim>, Dim> Gijk;
        for(size_type i{0}; i<rows; ++i){
            const auto I{non_repeated_eigenvalues[i]};
            const auto eigI{eig_values[I]};
            for(size_type j{0}; j<rows; ++j){
                const auto J{non_repeated_eigenvalues[j]};
                const auto eigJ{eig_values[J]};
                for(size_type k{0}; k<rows; ++k){
                    const auto K{non_repeated_eigenvalues[k]};
                    const auto eigK{eig_values[K]};
                    if(i == j && i == k && j == k){
                        Gijk[i][j][k] = _ddFi[I];
                    }else if (i != j && i == k && j != k){
                        Gijk[i][j][k] = ((eigI-eigJ)*_dFi[I]-_Fi[I]+_Fi[J])/((eigI-eigJ)*(eigI-eigJ));
                    }else if (i != j && i != k && j == k){
                        Gijk[i][j][k] = ((eigJ-eigI)*_dFi[J]+_Fi[I]-_Fi[J])/((eigI-eigJ)*(eigI-eigJ));
                    }
                }
            }
        }

        auto sym_f = [](auto const& Ei, auto const Ej){
            using I = eye<value_type, Dim, 2>;
            return (outer_product<sequence<1,3,5,6>,sequence<2,4>>((otimesu(I(),I()) + otimesl(I(),I())), Ej)
                   +outer_product<sequence<1,3>,sequence<2,4,5,6>>(Ei, (otimesu(I(),I()) + otimesl(I(),I())))
                   +outer_product<sequence<1,4,5,6>,sequence<2,3>>((otimesu(I(),I()) + otimesl(I(),I())), Ej)
                   +outer_product<sequence<1,4>,sequence<2,3,5,6>>(Ei, (otimesu(I(),I()) + otimesl(I(),I()))))*0.5;
        };


        std::array<tensor<value_type,Dim,4>,Dim> dEidA;
        for(size_type i{0}; i<rows; ++i){
            const auto I{non_repeated_eigenvalues[i]};
            const auto& _Ei = eig_basis[I];
            const auto lambda_i = eig_values[I];
            for(size_type j{0}; j<rows; ++j){
                const auto J{non_repeated_eigenvalues[j]};
                const auto& _Ej = eig_basis[J];
                const auto lambda_j = eig_values[J];
                if(i!=j){
                    dEidA[I] += (otimesu(_Ei,_Ej) + otimesl(_Ei,_Ej))/((lambda_i-lambda_j));
                }
            }
        }

        evaluate_Gij();

        eye<value_type, Dim, 2> I;
        auto IIs2 = otimesu(I,I) + otimesl(I,I);
        if(rows == 1){
            const auto& _Ei = eig_basis[non_repeated_eigenvalues[0]];
            _deriv = Gijk[0][0][0]*sym_f(_Ei,_Ei)*0.25;
        }else{
            for(size_type i{0}; i<rows; ++i){
                const auto I{non_repeated_eigenvalues[i]};
                const auto& _Ei = eig_basis[I];
                for(size_type j{0}; j<rows; ++j){
                    const auto J{non_repeated_eigenvalues[j]};
                    const auto& _Ej = eig_basis[J];
                    for(size_type k{0}; k<rows; ++k){
                        const auto K{non_repeated_eigenvalues[k]};
                        const auto& _Ek = eig_basis[K];
                        if(Gijk[i][j][k] != 0)
                            _deriv += Gijk[i][j][k]*otimes((otimesu(_Ei,_Ej) + otimesl(_Ei,_Ej)),_Ek)*0.5;
                    }
                    auto Eiji = outer_product<sequence<1,3,5,6>,sequence<2,4>>(IIs2, _Ej)
                               +outer_product<sequence<1,4,5,6>,sequence<2,3>>(IIs2, _Ej);
                    auto Eijj = outer_product<sequence<1,3>,sequence<2,4,5,6>>(_Ei, IIs2)
                               +outer_product<sequence<1,4>,sequence<2,3,5,6>>(_Ei, IIs2);

                    if(_Gij[i][j] != 0)
                        _deriv += 0.5*_Gij[i][j]*0.5*(dcontract(Eiji, dEidA[I]) + dcontract(Eijj, dEidA[J]));
                }
            }
        }

        return _deriv;
    }

    constexpr inline auto evaluate()noexcept;

    constexpr inline auto get_derivative()const noexcept;

    constexpr inline auto raw_data()const noexcept;

private:

    template<typename EigBasis>
    constexpr inline auto otimes_special(EigBasis const& _Ei, EigBasis const& _Ej){
        using I = eye<value_type, Dim, 2>;
        return outer_product<sequence<1,4,5,6,2,3>>(0.5*(otimesu(I(),I()) + otimesl(I(),I())), _Ej) + outer_product<sequence<1,3,5,6,2,4>>(0.5*(otimesu(I(),I()) + otimesl(I(),I())), _Ej)
               + outer_product<sequence<1,4,2,3,5,6>>(_Ei, 0.5*(otimesu(I(),I()) + otimesl(I(),I()))) + outer_product<sequence<1,3,2,4,5,6>>(_Ei, 0.5*(otimesu(I(),I()) + otimesl(I(),I())));
    }

    constexpr inline auto K_iijjkk(size_type const i, size_type const j, size_type const k){
        const eye<value_type, Dim, 2> I;
        return 4*_ddFi[i]*I(i,j)*I(i,k);
    }

    constexpr inline auto K_ijijkk(size_type const i, size_type const j, size_type const k){
        const auto eig_values{_eigen_decomposition.eigenvalues()};
        const eye<value_type, Dim, 2> I;
        if(std::fabs(eig_values[i] - eig_values[j]) <= std::numeric_limits<value_type>::epsilon()*std::max(eig_values[i], eig_values[j])){
            return _ddFi[i];
        }else{
            return gamma_iij(i,j)*I(i,k) + gamma_iij(j,i)*I(j,k);
        }
    }

    constexpr inline auto K_iijkjk(size_type const i, size_type const j, size_type const k){
//        const auto eig_values{_eigen_decomposition.eigenvalues()};
//        const eye<value_type, Dim, 2> I;
//        if(std::fabs(eig_values[i] - eig_values[l]) <= std::numeric_limits<value_type>::epsilon()*std::max(eig_values[i], eig_values[k])){
//            return _ddFi[j];
//        }else{
//            return gamma_iij(i,k)*I(i,j) + gamma_iij(i,j)*I(i,k);
//        }
    }

    constexpr inline auto K_ijjkki(size_type const i, size_type const j, size_type const k){
        const auto eig_values{_eigen_decomposition.eigenvalues()};
        const auto all_repeated_eigenvalues{_eigen_decomposition.all_eigenvalues_repeated()};
        const auto pair_repeated_eigenvalues{_eigen_decomposition.pair_eigenvalues_repeated()};

        if(all_repeated_eigenvalues){
            return 0.5*_ddFi[i];
        }



        if constexpr(Dim == 3){

        }else{
            if(all_repeated_eigenvalues){

            }else{

            }
        }

        const eye<value_type, Dim, 2> I;
//        if(std::fabs(eig_values[i] - eig_values[l]) <= std::numeric_limits<value_type>::epsilon()*std::max(eig_values[i], eig_values[k])){
//            return _ddFi[j];
//        }else{
//            return gamma_iij(i,k)*I(i,j) + gamma_iij(i,j)*I(i,k);
//        }
    }

    constexpr inline auto gamma()const{
//        const auto eig_values{_eigen_decomposition.eigenvalues()};
//        return (eig_values[0]*(_F[1]-_F[2]) + eig_values[1]*(_F[2]-_F[0]) + eig_values[2]*(_F[0]-_F[1]))
//                /((eig_values[0]-eig_values[1])*(eig_values[1]-eig_values[2])*(eig_values[2]-eig_values[0]));
    }

    constexpr inline auto gamma_iij(size_type const i, size_type const j)const{
//        const auto eig_values{_eigen_decomposition.eigenvalues()};
//        return (2*_dF[i]*(eig_values[i] - eig_values[j]) - 2*(_Fi[i] - _Fi[j]))
//                /((eig_values[i] - eig_values[j]) * (eig_values[i] - eig_values[j]));
    }

//    constexpr inline auto setup_equal_eigvals(){
//        const auto non_repeated_eigenvalues{_eigen_decomposition.non_repeated_eigenvalues_index()};
//        const auto all_repeated_eigenvalues{_eigen_decomposition.all_eigenvalues_repeated()};
//        const auto pair_repeated_eigenvalues{_eigen_decomposition.pair_eigenvalues_repeated()};


//        if(all_repeated_eigenvalues){
//            for(size_type i{0}; i<Dim; ++i){
//                for(size_type j{0}; j<Dim; ++j){
//                    _equal_eigvals[i][j] = true;
//                }
//            }
//            return ;
//        }

//        //only for 3D
//        if constexpr (Dim == 3){
//            if(pair_repeated_eigenvalues){

//                return ;
//            }
//        }



//        for(size_type i{0}; i<Dim; ++i){
//            for(size_type j{0}; j<Dim; ++j){
//                _equal_eigvals[i][j] = false;
//            }
//        }
//        return ;
//    }

    template<typename EigBasis>
    constexpr inline auto H_tensor(EigBasis const& basis_i, EigBasis const& basis_j, EigBasis const& basis_k){
        using Seq1 = sequence<1,3,2,5,4,6>;
        using Seq2 = sequence<1,3,2,6,4,5>;
        using Seq3 = sequence<1,4,2,5,3,6>;
        using Seq4 = sequence<1,4,2,6,3,5>;
        using Seq5 = sequence<2,3,1,5,4,6>;
        using Seq6 = sequence<2,3,1,6,4,5>;
        using Seq7 = sequence<2,4,1,5,3,6>;
        using Seq8 = sequence<2,4,1,6,3,5>;


        return basis_change<Seq1>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq2>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq3>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq4>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq5>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq6>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq7>(otimes(basis_i,otimes(basis_j, basis_k)))
                + basis_change<Seq8>(otimes(basis_i,otimes(basis_j, basis_k)));
    }

    constexpr inline auto evaluate_Gij()noexcept;

    tensor<value_type, Dim, 2> _data;
    tensor<value_type, Dim, 4> _deriv;
    std::array<std::array<value_type, Dim>, Dim> _Gij;
    std::array<value_type, Dim> _Fi;
    std::array<value_type, Dim> _dFi;
    std::array<value_type, Dim> _ddFi;
    //std::array<std::array<bool, Dim>, Dim> _equal_eigvals;
    eigen_decomposition_wrapper<Tensor> _eigen_decomposition;
};

} // NAMESPACE DETAIL

#endif // ISOTROPIC_TENSOR_FUNCTIONS_BONES_H
