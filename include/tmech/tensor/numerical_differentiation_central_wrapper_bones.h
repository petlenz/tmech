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
#ifndef NUMERICAL_DIFFERENTIATION_CENTRAL_WRAPPER_BONES_H
#define NUMERICAL_DIFFERENTIATION_CENTRAL_WRAPPER_BONES_H

namespace detail {

template<class TupType, size_t... I>
void print(const TupType& _tup, std::index_sequence<I...>)
{
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}

template<class... T>
void print (const std::tuple<T...>& _tup)
{
    print(_tup, std::make_index_sequence<sizeof...(T)>());
}

template<typename _Position>
class numdiff_central
{
public:
    using size_type = std::size_t;


    numdiff_central(){}


    //diff<<i,k,l,1>>(C,c)
    //diff<<1,3,4,2>>(C,c)

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto evaluate(Function __func, Direction const& __A, Result & __result, T const __eps){
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
    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto scalar_wrt_scalar(Function __func, Direction const& __A, Result & __result, T const __eps){
        const auto inv_eps{static_cast<T>(1.0)/(2*__eps)};

        __result = (__func(__A+__eps) - __func(__A-__eps))*inv_eps;
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto tensor_wrt_scalar(Function __func, Direction const& __A, Result & __result, T const __eps){
        const auto inv_eps{static_cast<T>(1.0)/(2*__eps)};

        __result = (__func(__A+__eps) - __func(__A-__eps))*inv_eps;
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto scalar_wrt_tensor(Function __func, Direction const& __A, Result & __result, T const __eps){
        using direction_loop = typename meta_for_loop_deep<Direction::dimension(), Direction::rank()-1>::type;
        const auto inv_eps{static_cast<T>(1.0)/(2*__eps)};
        //dS/dC_{ij}
        tensor<T, Direction::dimension(), Direction::rank()> Dp(__A);
        tensor<T, Direction::dimension(), Direction::rank()> Dm(Dp);

        auto diff_kernal = [&](auto ...Numbers){
            Dp(Numbers...) += __eps;
            Dm(Numbers...) -= __eps;
            const auto ap{__func(Dp)};
            const auto am{__func(Dm)};
            Dp(Numbers...) -= __eps;
            Dm(Numbers...) += __eps;
            __result(Numbers...) = (ap - am)*inv_eps;
        };

        direction_loop::loop(diff_kernal);
    }



    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto tensor_wrt_tensor(Function __func, Direction const& __A, Result & __result, T const __eps){
        using Position = min_value_squence_t<_Position, 1>;
        constexpr auto FuncRank{decltype (__func(__A))::rank()};
        using direction_loop = typename meta_for_loop_deep<Direction::dimension(), Direction::rank()-1>::type;
        using function_loop  = typename meta_for_loop_deep<Direction::dimension(), FuncRank-1>::type;
        const auto inv_eps{static_cast<T>(1.0)/(2*__eps)};

        static_assert (Position::size() == (Direction::rank() + FuncRank), "numdiff_central:: number of positions does not match!");

        tensor<T, Direction::dimension(), Direction::rank()> Dp(__A);
        tensor<T, Direction::dimension(), Direction::rank()> Dm(Dp);

        auto direction_kernal = [&](auto ...ONumbers){
            Dp(ONumbers...) += __eps;
            Dm(ONumbers...) -= __eps;
            const tensor<T, Direction::dimension(), FuncRank> Ap{__func(Dp)};
            const tensor<T, Direction::dimension(), FuncRank> Am{__func(Dm)};
            Dp(ONumbers...) -= __eps;
            Dm(ONumbers...) += __eps;

            auto diff_kernal = [&](auto ...INumbers){
                const auto tuple = std::make_tuple(INumbers..., ONumbers...);
                tuple_call(__result, tuple, Position()) = (Ap(INumbers...) - Am(INumbers...))*inv_eps;
            };

            function_loop::loop(diff_kernal);
        };
        direction_loop::loop(direction_kernal);
    }
};




/**
* @class inverse_wrapper
* @brief Wrapper for the determination of the
* inverse of a tensor expression.
*
* @tparam Tensor Tensor expression from which the
* inverse should be determined.
*/
class numerical_differentiation_central_wrapper
{
public:
    using size_type   = std::size_t;

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto differentiation(Function & __func, Direction const& __A, Result & __result, T const __eps){
        using output = decltype (__func(__A));

        if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == true){
            diff_0_0(__func, __A, __result, __eps);
        }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == true) {
            //diff_0_Rank
            if constexpr (Direction::rank() == 2){
                diff_0_2(__func, __A, __result, __eps);
            }else{
                static_assert (true, "Not implemented until now");
            }
        }else if constexpr (std::is_fundamental_v<Direction> == true && std::is_fundamental_v<output> == false) {
            static_assert (true, "Not implemented until now");
        }else if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == false) {
            if constexpr (Direction::rank() == 2 && output::rank() == 2){
                diff_2_2(__func, __A, __result, __eps);
            }else if constexpr (Direction::rank() == 2 && output::rank() == 4) {
                diff_4_2(__func, __A, __result, __eps);
            }else if constexpr (Direction::rank() == 4 && output::rank() == 4) {
                diff_4_4(__func, __A, __result, __eps);
            }else{
                static_assert (true, "Not implemented until now");
            }
        }
    }

private:
    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto diff_0_0(Function & __func, Direction const& __A, Result & __result, T const __eps){
        const auto eps_half{__eps*static_cast<T>(0.5)};
        const auto inv_eps{static_cast<T>(1.0)/__eps};

        __result = (__func(__A+eps_half) - __func(__A-eps_half))*inv_eps;
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto diff_0_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
        constexpr auto Dim{Direction::dimension()};
        const auto eps_half{__eps*static_cast<T>(0.5)};
        const auto inv_eps{static_cast<T>(1.0)/__eps};
        tensor<T, Dim, 2> Dp(__A), Dm(__A);
        for(size_type m{0}; m < Dim; ++m){
            for(size_type n{0}; n < Dim; ++n){
                Dp(m,n) += eps_half;
                Dm(m,n) -= eps_half;
                const auto ap{__func(Dp)};
                const auto am{__func(Dm)};
                Dp(m,n) -= eps_half;
                Dm(m,n) += eps_half;
                __result(m,n) = (ap - am)*inv_eps;
            }
        }
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto diff_2_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
        constexpr auto Dim{Direction::dimension()};
        const auto eps_half{__eps*static_cast<T>(0.5)};
        const auto inv_eps{static_cast<T>(1.0)/__eps};
        tensor<T, Dim, 2> Dp(__A), Dm(__A);
        for(size_type m{0}; m < Dim; ++m){
            for(size_type n{0}; n < Dim; ++n){
                Dp(m,n) += eps_half;
                Dm(m,n) -= eps_half;
                const tensor<T, Dim, 2> ap{__func(Dp)};
                const tensor<T, Dim, 2> am{__func(Dm)};
                Dp(m,n) -= eps_half;
                Dm(m,n) += eps_half;
                for(size_type i{0}; i<Dim; ++i){
                    for(size_type j{0}; j<Dim; ++j){
                        __result(i,j,m,n) = (ap(i,j)-am(i,j))*inv_eps;
                    }
                }
            }
        }
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto diff_4_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
        constexpr auto Dim{Direction::dimension()};
        const auto eps_half{__eps*static_cast<T>(0.5)};
        const auto inv_eps{static_cast<T>(1.0)/__eps};
        tensor<T, Dim, 2> Dp(__A), Dm(__A);
        for(size_type m{0}; m<Dim; ++m){
            for(size_type n{0}; n<Dim; ++n){
                Dp(m,n) += eps_half;
                Dm(m,n) -= eps_half;
                const tensor<T, Dim, 4> ap{__func(Dp)};
                const tensor<T, Dim, 4> am{__func(Dm)};
                Dp(m,n) -= eps_half;
                Dm(m,n) += eps_half;
                for(size_type i{0}; i<Dim; ++i){
                    for(size_type j{0}; j<Dim; ++j){
                        for(size_type k{0}; k<Dim; ++k){
                            for(size_type l{0}; l<Dim; ++l){
                                __result(i,j,k,l,m,n) = (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
                            }
                        }
                    }
                }
            }
        }
    }


    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto diff_4_4(Function & __func, Direction const& __A, Result & __result, T const __eps){
        constexpr auto Dim{Direction::dimension()};
        const auto eps_half{__eps*static_cast<T>(0.5)};
        const auto inv_eps{static_cast<T>(1.0)/__eps};
        tensor<T, Dim, 4> Dp(__A), Dm(__A);

        for(size_type m{0}; m<Dim; ++m){
            for(size_type n{0}; n<Dim; ++n){
                for(size_type o{0}; o<Dim; ++o){
                    for(size_type p{0}; p<Dim; ++p){
                        Dp(m,n,o,p) += eps_half;
                        Dm(m,n,o,p) -= eps_half;
                        const tensor<T, Dim, 4> ap{__func(Dp)};
                        const tensor<T, Dim, 4> am{__func(Dm)};
                        Dp(m,n,o,p) -= eps_half;
                        Dm(m,n,o,p) += eps_half;
                        for(size_type i{0}; i<Dim; ++i){
                            for(size_type j{0}; j<Dim; ++j){
                                for(size_type k{0}; k<Dim; ++k){
                                    for(size_type l{0}; l<Dim; ++l){
                                        __result(i,j,k,l,m,n,o,p) = (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};


} // NAMESPACE DETAIL

#endif // NUMERICAL_DIFFERENTIATION_CENTRAL_WRAPPER_BONES_H
