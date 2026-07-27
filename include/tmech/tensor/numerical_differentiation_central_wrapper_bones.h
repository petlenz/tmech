/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef NUMERICAL_DIFFERENTIATION_CENTRAL_WRAPPER_BONES_H
#define NUMERICAL_DIFFERENTIATION_CENTRAL_WRAPPER_BONES_H

namespace detail {

// Central finite-difference stencil descriptor. _Points selects the accuracy
// order: 2 -> O(h^2) (the classic (f(x+h)-f(x-h))/(2h)), 5 -> O(h^4),
// 7 -> O(h^6). nodes() lists the non-zero (offset, weight) pairs and divisor()
// the shared denominator, i.e. f'(x) ~= (sum_k w_k f(x + k*h)) / (divisor*h).
template<std::size_t _Points>
struct numdiff_stencil
{
    static constexpr inline auto nodes()noexcept{
        if constexpr (_Points == 2){
            return std::array<std::pair<int, double>, 2>{{{-1, -1.0}, {1, 1.0}}};
        }else if constexpr (_Points == 5){
            return std::array<std::pair<int, double>, 4>{
                {{-2, 1.0}, {-1, -8.0}, {1, 8.0}, {2, -1.0}}};
        }else if constexpr (_Points == 7){
            return std::array<std::pair<int, double>, 6>{
                {{-3, -1.0}, {-2, 9.0}, {-1, -45.0}, {1, 45.0}, {2, -9.0}, {3, 1.0}}};
        }else{
            static_assert(_Points == 2 || _Points == 5 || _Points == 7,
                          "num_diff_central: unsupported stencil size (supported: 2, 5, 7 points)");
            return std::array<std::pair<int, double>, 2>{{{-1, -1.0}, {1, 1.0}}};
        }
    }

    static constexpr inline double divisor()noexcept{
        if constexpr (_Points == 5){
            return 12.0;
        }else if constexpr (_Points == 7){
            return 60.0;
        }else{
            return 2.0;
        }
    }

    // Combine the sampled function values into a derivative estimate.
    // __base_scale is the plain 2-point factor 1/(2h) that the kernels already
    // compute; multiplying by (2/divisor) turns the weighted node sum into the
    // higher-order estimate and reduces to the original exactly for _Points==2.
    // __sample(k) must return an already-materialised value at x + k*(unit step)
    // so no expression-template temporaries dangle in the accumulation.
    template<typename _ResultT, typename _ScaleT, typename _SampleFn>
    static constexpr inline _ResultT combine(_SampleFn && __sample, _ScaleT const __base_scale)noexcept{
        constexpr auto nds{nodes()};
        const _ScaleT scale{__base_scale * static_cast<_ScaleT>(2.0 / divisor())};
        _ResultT acc{__sample(nds[0].first) * static_cast<_ScaleT>(nds[0].second)};
        for(std::size_t n{1}; n < nds.size(); ++n){
            const _ResultT s{__sample(nds[n].first)};
            acc = acc + s * static_cast<_ScaleT>(nds[n].second);
        }
        return acc * scale;
    }
};


template<typename _Position, std::size_t _Points = 2>
class numdiff_central
{
public:
    using size_type = std::size_t;


    numdiff_central()noexcept{}


    //diff<<i,k,l,1>>(C,c)
    //diff<<1,3,4,2>>(C,c)

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto evaluate(Function __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        using output = decltype (__func(__A));

        if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == true){
            //scalar wrt scalar
            scalar_wrt_scalar(__func, __A, __result, __eps);
        }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == true) {
            //scalar wrt tensor
            scalar_wrt_tensor(__func, __A, __result, __eps);
        }else if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == false) {
            //tensor wrt to scalar
            tensor_wrt_scalar(__func, __A, __result, __eps);
        }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == false) {
            //tensor wrt to tensor
            tensor_wrt_tensor(__func, __A, __result, __eps);
        }
    }

private:
    using stencil = numdiff_stencil<_Points>;

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto scalar_wrt_scalar(Function __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        const T base{static_cast<T>(1.0)/(static_cast<T>(2.0)*__eps)};
        auto sample = [&](int __k){ return __func(__A + static_cast<T>(__k)*__eps); };
        __result = stencil::template combine<T>(sample, base);
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto tensor_wrt_scalar(Function __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        using output_type = decltype(__func(__A));
        using result_tensor = tensor<T, output_type::dimension(), output_type::rank()>;
        const T base{static_cast<T>(1.0)/(static_cast<T>(2.0)*__eps)};
        auto sample = [&](int __k){ return result_tensor(__func(__A + static_cast<T>(__k)*__eps)); };
        __result = stencil::template combine<result_tensor>(sample, base);
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto scalar_wrt_tensor(Function __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        using direction_loop = typename meta_for_loop_deep<Direction::dimension(), Direction::rank()-1>::type;
        const T base{static_cast<T>(1.0)/(static_cast<T>(2.0)*__eps)};
        //dS/dC_{ij}
        tensor<T, Direction::dimension(), Direction::rank()> D(__A);

        auto diff_kernal = [&](auto ...Numbers){
            auto sample = [&](int __k){
                D(Numbers...) = __A(Numbers...) + static_cast<T>(__k)*__eps;
                const auto v{__func(D)};
                D(Numbers...) = __A(Numbers...);
                return v;
            };
            __result(Numbers...) = stencil::template combine<T>(sample, base);
        };

        direction_loop::loop(diff_kernal);
    }



    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto tensor_wrt_tensor(Function __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        using Position = min_value_squence_t<_Position, 1>;

        // FIX: Use type aliases instead of constexpr auto FuncRank.
        // MSVC does not treat constexpr auto variables from an enclosing
        // scope as constant expressions inside generic lambdas.
        using output_type = decltype(__func(__A));
        using func_result_tensor =
            tensor<T, Direction::dimension(), output_type::rank()>;
        using direction_loop = typename meta_for_loop_deep<Direction::dimension(), Direction::rank()-1>::type;
        using function_loop =
            typename meta_for_loop_deep<Direction::dimension(),
                                        func_result_tensor::rank() - 1>::type;
        const T base{static_cast<T>(1.0)/(static_cast<T>(2.0)*__eps)};

        static_assert(Position::size() ==
                          (Direction::rank() + func_result_tensor::rank()),
                      "numdiff_central:: number of positions does not match!");

        tensor<T, Direction::dimension(), Direction::rank()> D(__A);

        auto direction_kernal = [&](auto ...ONumbers){
            auto sample = [&](int __k){
                D(ONumbers...) = __A(ONumbers...) + static_cast<T>(__k)*__eps;
                func_result_tensor v{__func(D)};
                D(ONumbers...) = __A(ONumbers...);
                return v;
            };
            const func_result_tensor Blk{stencil::template combine<func_result_tensor>(sample, base)};

            auto diff_kernal = [&](auto ...INumbers){
                const auto tuple = std::make_tuple(INumbers..., ONumbers...);
                tuple_call(__result, tuple, Position()) = Blk(INumbers...);
            };

            function_loop::loop(diff_kernal);
        };
        direction_loop::loop(direction_kernal);
    }
};




///**
//* @class inverse_wrapper
//* @brief Wrapper for the determination of the
//* inverse of a tensor expression.
//*
//* @tparam Tensor Tensor expression from which the
//* inverse should be determined.
//*/
//class numerical_differentiation_central_wrapper
//{
//public:
//    using size_type   = std::size_t;

//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto differentiation(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        using output = decltype (__func(__A));

//        if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == true){
//            diff_0_0(__func, __A, __result, __eps);
//        }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == true) {
//            //diff_0_Rank
//            if constexpr (Direction::rank() == 2){
//                diff_0_2(__func, __A, __result, __eps);
//            }else{
//                static_assert (true, "Not implemented until now");
//            }
//        }else if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == false) {
//            static_assert (true, "Not implemented until now");
//        }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == false) {
//            if constexpr (Direction::rank() == 2 && output::rank() == 2){
//                diff_2_2(__func, __A, __result, __eps);
//            }else if constexpr (Direction::rank() == 2 && output::rank() == 4) {
//                diff_4_2(__func, __A, __result, __eps);
//            }else if constexpr (Direction::rank() == 4 && output::rank() == 4) {
//                diff_4_4(__func, __A, __result, __eps);
//            }else{
//                static_assert (true, "Not implemented until now");
//            }
//        }
//    }

//private:
//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_0_0(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        const auto eps_half{__eps*static_cast<T>(0.5)};
//        const auto inv_eps{static_cast<T>(1.0)/__eps};

//        __result = (__func(__A+eps_half) - __func(__A-eps_half))*inv_eps;
//    }

//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_0_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const auto eps_half{__eps*static_cast<T>(0.5)};
//        const auto inv_eps{static_cast<T>(1.0)/__eps};
//        tensor<T, Dim, 2> Dp(__A), Dm(__A);
//        for(size_type m{0}; m < Dim; ++m){
//            for(size_type n{0}; n < Dim; ++n){
//                Dp(m,n) += eps_half;
//                Dm(m,n) -= eps_half;
//                const auto ap{__func(Dp)};
//                const auto am{__func(Dm)};
//                Dp(m,n) -= eps_half;
//                Dm(m,n) += eps_half;
//                __result(m,n) = (ap - am)*inv_eps;
//            }
//        }
//    }

//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_2_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const auto eps_half{__eps*static_cast<T>(0.5)};
//        const auto inv_eps{static_cast<T>(1.0)/__eps};
//        tensor<T, Dim, 2> Dp(__A), Dm(__A);
//        for(size_type m{0}; m < Dim; ++m){
//            for(size_type n{0}; n < Dim; ++n){
//                Dp(m,n) += eps_half;
//                Dm(m,n) -= eps_half;
//                const tensor<T, Dim, 2> ap{__func(Dp)};
//                const tensor<T, Dim, 2> am{__func(Dm)};
//                Dp(m,n) -= eps_half;
//                Dm(m,n) += eps_half;
//                for(size_type i{0}; i<Dim; ++i){
//                    for(size_type j{0}; j<Dim; ++j){
//                        __result(i,j,m,n) = (ap(i,j)-am(i,j))*inv_eps;
//                    }
//                }
//            }
//        }
//    }

//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_4_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const auto eps_half{__eps*static_cast<T>(0.5)};
//        const auto inv_eps{static_cast<T>(1.0)/__eps};
//        tensor<T, Dim, 2> Dp(__A), Dm(__A);
//        for(size_type m{0}; m<Dim; ++m){
//            for(size_type n{0}; n<Dim; ++n){
//                Dp(m,n) += eps_half;
//                Dm(m,n) -= eps_half;
//                const tensor<T, Dim, 4> ap{__func(Dp)};
//                const tensor<T, Dim, 4> am{__func(Dm)};
//                Dp(m,n) -= eps_half;
//                Dm(m,n) += eps_half;
//                for(size_type i{0}; i<Dim; ++i){
//                    for(size_type j{0}; j<Dim; ++j){
//                        for(size_type k{0}; k<Dim; ++k){
//                            for(size_type l{0}; l<Dim; ++l){
//                                __result(i,j,k,l,m,n) = (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }


//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_4_4(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const auto eps_half{__eps*static_cast<T>(0.5)};
//        const auto inv_eps{static_cast<T>(1.0)/__eps};
//        tensor<T, Dim, 4> Dp(__A), Dm(__A);

//        for(size_type m{0}; m<Dim; ++m){
//            for(size_type n{0}; n<Dim; ++n){
//                for(size_type o{0}; o<Dim; ++o){
//                    for(size_type p{0}; p<Dim; ++p){
//                        Dp(m,n,o,p) += eps_half;
//                        Dm(m,n,o,p) -= eps_half;
//                        const tensor<T, Dim, 4> ap{__func(Dp)};
//                        const tensor<T, Dim, 4> am{__func(Dm)};
//                        Dp(m,n,o,p) -= eps_half;
//                        Dm(m,n,o,p) += eps_half;
//                        for(size_type i{0}; i<Dim; ++i){
//                            for(size_type j{0}; j<Dim; ++j){
//                                for(size_type k{0}; k<Dim; ++k){
//                                    for(size_type l{0}; l<Dim; ++l){
//                                        __result(i,j,k,l,m,n,o,p) = (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//};


} // NAMESPACE DETAIL

#endif // NUMERICAL_DIFFERENTIATION_CENTRAL_WRAPPER_BONES_H
