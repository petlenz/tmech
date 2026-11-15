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

    // a functor object with parameters, e.g. the Seth-Hill strain (x^(m/2) - 1)/m with a runtime m
    constexpr isotropic_tensor_function(Tensor data, Func func)noexcept;

    constexpr isotropic_tensor_function(isotropic_tensor_function const& data)noexcept;

    constexpr inline auto operator()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto derivative()noexcept;

    // Daleckii-Krein: d2f(A)[H, K] = sum_ijk f[l_i, l_k, l_j] (P_i H P_k K P_j + P_i K P_k H P_j) over the
    // rank-one projectors P_i = e_i (x) e_i, with confluent divided differences: valid for any multiplicity.
    constexpr inline auto second_derivative()noexcept{
        _eigen_decomposition.decompose_eigenbasis();
        const auto l{_eigen_decomposition.eigenvalues()};
        const auto& P{_eigen_decomposition.projectors()};
        std::array<value_type, Dim> f, ddf;
        for(size_type i{0}; i<Dim; ++i){
            f[i] = _func.apply(l[i]);
            ddf[i] = _func.second_derivative(l[i]);
        }
        // f[l_a, l_b, l_c] with the widest pair in the denominator; where that cancels (relative eps^1/4),
        // the mean of f''/2 at the nodes, exact for a linear f'' (accuracy ~1e-8 next to the switch)
        auto divided_difference_3 = [&](size_type a, size_type b, size_type c){
            if(std::fabs(l[a] - l[b]) > std::fabs(l[a] - l[c]) && std::fabs(l[a] - l[b]) > std::fabs(l[b] - l[c])){
                std::swap(b, c);
            }else if(std::fabs(l[b] - l[c]) > std::fabs(l[a] - l[c])){
                std::swap(a, b);
            }
            const auto f_ab{divided_difference(l[a], l[b], f[a], f[b])}, f_bc{divided_difference(l[b], l[c], f[b], f[c])};
            const auto tol{std::pow(std::numeric_limits<value_type>::epsilon(), value_type(0.25))};
            if(std::fabs(l[a] - l[c]) <= tol*std::max(std::fabs(l[a]), std::fabs(l[c])) ||
               std::fabs(f_ab - f_bc) <= tol*std::max(std::fabs(f_ab), std::fabs(f_bc))){
                return (ddf[a] + ddf[b] + ddf[c])/6;
            }
            return (f_ab - f_bc)/(l[a] - l[c]);
        };
        tensor<value_type, Dim, 6> G;
        for(size_type i{0}; i<Dim; ++i){
            for(size_type j{0}; j<Dim; ++j){
                tensor<value_type, Dim, 2> M;
                for(size_type k{0}; k<Dim; ++k){
                    M += divided_difference_3(i, k, j)*P[k];
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

    // f[a, b] from f_a = f(a), f_b = f(b); f' at the midpoint where f_a - f_b cancels (e.g. exp near 0,
    // where the eigenvalues are distinct but the values are not)
    constexpr inline value_type divided_difference(value_type const a, value_type const b, value_type const f_a, value_type const f_b)const noexcept{
        if(std::fabs(f_a - f_b) <= std::sqrt(std::numeric_limits<value_type>::epsilon())*std::max(std::fabs(f_a), std::fabs(f_b))){
            return _func.derivative((a + b)/2);
        }
        return (f_a - f_b)/(a - b);
    }

    constexpr inline auto evaluate_Gij()noexcept;

    tensor<value_type, Dim, 2> _data;
    tensor<value_type, Dim, 4> _deriv;
    std::array<std::array<value_type, Dim>, Dim> _Gij;
    eigen_decomposition_wrapper<Tensor> _eigen_decomposition;
    Func _func;
};

} // NAMESPACE DETAIL

#endif // ISOTROPIC_TENSOR_FUNCTIONS_BONES_H
