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

    // Daleckii-Krein: d2f(A)[H, K] = sum_ijk f[l_i, l_k, l_j] (P_i H P_k K P_j + P_i K P_k H P_j) over the
    // rank-one projectors P_i = e_i (x) e_i, with confluent divided differences: valid for any multiplicity.
    constexpr inline auto second_derivative()noexcept{
        _eigen_decomposition.decompose_eigenbasis();
        const auto eig_values{_eigen_decomposition.eigenvalues()};
        const auto& P{_eigen_decomposition.projectors()};
        tensor<value_type, Dim, 6> G;
        for(size_type i{0}; i<Dim; ++i){
            for(size_type j{0}; j<Dim; ++j){
                tensor<value_type, Dim, 2> M;
                for(size_type k{0}; k<Dim; ++k){
                    M += divided_difference(eig_values[i], eig_values[k], eig_values[j])*P[k];
                }
                // G_abcdef = (P_i)_ac (M_ij)_de (P_j)_fb
                G += outer_product<sequence<1,3>, sequence<4,5,6,2>>(P[i], otimes(M, P[j]));
            }
        }
        // both orders of H and K, symmetric in each
        const tensor<value_type, Dim, 6> GK{G + basis_change<sequence<1,2,5,6,3,4>>(G)};
        const tensor<value_type, Dim, 6> GKs{0.5*(GK + basis_change<sequence<1,2,4,3,5,6>>(GK))};
        tensor<value_type, Dim, 6> _deriv{0.5*(GKs + basis_change<sequence<1,2,3,4,6,5>>(GKs))};
        return _deriv;
    }

    constexpr inline auto evaluate()noexcept;

    constexpr inline auto get_derivative()const noexcept;

    constexpr inline auto raw_data()const noexcept;

private:

    // f[a, b]; f' at the midpoint where f(a) - f(b) cancels (e.g. exp near 0, where the eigenvalues are
    // distinct but the values are not)
    static constexpr inline value_type divided_difference(value_type const a, value_type const b)noexcept{
        const auto f_a{Func::apply(a)}, f_b{Func::apply(b)};
        if(std::fabs(f_a - f_b) <= std::sqrt(std::numeric_limits<value_type>::epsilon())*std::max(std::fabs(f_a), std::fabs(f_b))){
            return Func::derivative((a + b)/2);
        }
        return (f_a - f_b)/(a - b);
    }

    // f[a, b, c] with the widest pair in the denominator; where that cancels, the mean of f''/2 at the nodes
    // (exact for a linear f'')
    static constexpr inline value_type divided_difference(value_type a, value_type b, value_type c)noexcept{
        if(std::fabs(a - b) > std::fabs(a - c) && std::fabs(a - b) > std::fabs(b - c)){
            std::swap(b, c);
        }else if(std::fabs(b - c) > std::fabs(a - c)){
            std::swap(a, b);
        }
        const auto f_ab{divided_difference(a, b)}, f_bc{divided_difference(b, c)};
        const auto tol{std::pow(std::numeric_limits<value_type>::epsilon(), value_type(0.25))};
        if(std::fabs(a - c) <= tol*std::max(std::fabs(a), std::fabs(c)) ||
           std::fabs(f_ab - f_bc) <= tol*std::max(std::fabs(f_ab), std::fabs(f_bc))){
            return (Func::second_derivative(a) + Func::second_derivative(b) + Func::second_derivative(c))/6;
        }
        return (f_ab - f_bc)/(a - c);
    }

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

    constexpr inline auto K_iijkjk([[maybe_unused]] size_type const i,
                                   [[maybe_unused]] size_type const j,
                                   [[maybe_unused]] size_type const k) {
      //        const auto eig_values{_eigen_decomposition.eigenvalues()};
      //        const eye<value_type, Dim, 2> I;
      //        if(std::fabs(eig_values[i] - eig_values[l]) <=
      //        std::numeric_limits<value_type>::epsilon()*std::max(eig_values[i],
      //        eig_values[k])){
      //            return _ddFi[j];
      //        }else{
      //            return gamma_iij(i,k)*I(i,j) + gamma_iij(i,j)*I(i,k);
      //        }
    }

    constexpr inline auto K_ijjkki([[maybe_unused]] size_type const i,
                                   [[maybe_unused]] size_type const j,
                                   [[maybe_unused]] size_type const k) {
      const auto eig_values{_eigen_decomposition.eigenvalues()};
      const auto all_repeated_eigenvalues{
          _eigen_decomposition.all_eigenvalues_repeated()};
      const auto pair_repeated_eigenvalues{
          _eigen_decomposition.pair_eigenvalues_repeated()};

      if (all_repeated_eigenvalues) {
        return 0.5 * _ddFi[i];
      }

      if constexpr (Dim == 3) {

      } else {
        if (all_repeated_eigenvalues) {

        } else {
        }
      }

      const eye<value_type, Dim, 2> I;
      //        if(std::fabs(eig_values[i] - eig_values[l]) <=
      //        std::numeric_limits<value_type>::epsilon()*std::max(eig_values[i],
      //        eig_values[k])){
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

    constexpr inline auto gamma_iij([[maybe_unused]] size_type const i,
                                    [[maybe_unused]] size_type const j) const {
      //        const auto eig_values{_eigen_decomposition.eigenvalues()};
      //        return (2*_dF[i]*(eig_values[i] - eig_values[j]) - 2*(_Fi[i] -
      //        _Fi[j]))
      //                /((eig_values[i] - eig_values[j]) * (eig_values[i] -
      //                eig_values[j]));
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
