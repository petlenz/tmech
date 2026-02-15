/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_BONES_H
#define NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_BONES_H


namespace detail {

template<typename Result, std::size_t Dim>
using basis_pair_t = meta_for_loop_symmetric<meta_for_loop_symmetric<Result, Dim, 0>, Dim, 0>;


template <typename Function>
struct basis_pair
{
    template<typename Func, typename ...Indicies>
    static constexpr inline auto loop(Func const& func, const Indicies &... indicies)noexcept{
        Function::loop(func, indicies...);
    }
};

template <typename Result, std::size_t Dim, std::size_t End,
          std::size_t Start = 0>
struct basis_pair_loop_h
    : public basis_pair_loop_h<basis_pair<basis_pair_t<Result, Dim>>, Dim, End,
                               Start + 1> {
  using type = Result;
};

template <std::size_t Dim, std::size_t End, std::size_t Start>
struct basis_pair_loop_h<void, Dim, End, Start>
    : public basis_pair_loop_h<basis_pair<basis_pair_t<void, Dim>>, Dim, End,
                               Start + 1> {};

template <typename Result, std::size_t Dim, std::size_t End>
struct basis_pair_loop_h <Result, Dim, End, End>
{
    using type = Result;

    template<typename Func, typename ...Indicies>
    static constexpr inline auto loop(Func const& func, const Indicies &... indicies)noexcept{
        Result::loop(func, indicies...);
    }
};

template <std::size_t Dim, std::size_t End>
struct basis_pair_loop : public basis_pair_loop_h<void, Dim, End> {};

template <typename First, typename ...Args>
struct set_basis
{
    template<typename Tensor, typename T, typename Tuple>
    static constexpr inline auto set(Tensor & basis, Tuple const& tuple, T const value)noexcept{
        tuple_call(basis, tuple, min_value_squence_t<First,1>()) = value;
        set_basis<Args...>::set(basis, tuple, value);
    }

    template<typename Tensor, typename T, typename Tuple>
    static constexpr inline auto add(Tensor & basis, Tuple const& tuple, T const value)noexcept{
        tuple_call(basis, tuple, min_value_squence_t<First, 1>()) += value;
        set_basis<Args...>::add(basis, tuple, value);
    }
};
template <typename First>
struct set_basis<First>
{
    template<typename Tensor, typename T, typename Tuple>
    static constexpr inline auto set(Tensor & basis, Tuple const& tuple, T const value)noexcept{
        tuple_call(basis, tuple, min_value_squence_t<First, 1>()) = value;
    }

    template<typename Tensor, typename T, typename Tuple>
    static constexpr inline auto add(Tensor & basis, Tuple const& tuple, T const value)noexcept{
        tuple_call(basis, tuple, min_value_squence_t<First, 1>()) += value;
    }
};

template<typename SymDirection, typename SymResult>
class numdiff_central_symmetric;

template<typename ...SymDirection, typename ...SymResult>
class numdiff_central_symmetric<std::tuple<SymDirection...>, std::tuple<SymResult...>>
{
public:
    using size_type = std::size_t;

    numdiff_central_symmetric()noexcept{}

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
    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto scalar_wrt_scalar(Function & __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        const T inv_eps{static_cast<T>(1.0)/(static_cast<T>(2.0)*__eps)};

        __result = (__func(__A+__eps) - __func(__A-__eps))*inv_eps;
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto tensor_wrt_scalar(Function & __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        const T inv_eps{static_cast<T>(1.0)/(static_cast<T>(2.0)*__eps)};

        __result = (__func(__A+__eps) - __func(__A-__eps))*inv_eps;
    }

    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto scalar_wrt_tensor(Function & __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        using direction_loop = basis_pair_loop<Direction::dimension(), Direction::rank()/2ul>;

        const T eps_half{__eps/(sizeof... (SymDirection))};
        const T inv_eps{static_cast<T>(1.0)/((sizeof... (SymDirection))*__eps)};

        //dS/dC_{ij}
        tensor<T, Direction::dimension(), Direction::rank()> Dp(__A);
        tensor<T, Direction::dimension(), Direction::rank()> Dm(Dp);
        auto diff_kernal = [&](auto ...Numbers){
            auto tuple = std::make_tuple(Numbers...);

            set_basis<SymDirection...>::add(Dp, tuple, +eps_half);
            set_basis<SymDirection...>::add(Dm, tuple, -eps_half);
            const auto Ap{__func(Dp)};
            const auto Am{__func(Dm)};
            set_basis<SymDirection...>::add(Dp, tuple, -eps_half);
            set_basis<SymDirection...>::add(Dm, tuple, +eps_half);

            set_basis<SymDirection...>::set(__result, tuple, (Ap - Am)*inv_eps);
        };
        direction_loop::loop(diff_kernal);
    }

    template <typename Function, typename Direction, typename Result,
              typename T>
    static constexpr inline auto
    tensor_wrt_tensor(Function __func, Direction const &__A, Result &__result,
                      T const __eps) noexcept {
      // FIX: Use type aliases instead of constexpr auto FuncRank.
      // MSVC does not treat constexpr auto variables from an enclosing
      // scope as constant expressions inside generic lambdas.
      using output_type = decltype(__func(__A));
      using func_result_tensor =
          tensor<T, Direction::dimension(), output_type::rank()>;

      using direction_loop =
          basis_pair_loop<Direction::dimension(), Direction::rank() / 2ul>;
      using function_loop =
          basis_pair_loop<Direction::dimension(), output_type::rank() / 2ul>;

      const T eps_half{__eps / (sizeof...(SymDirection))};
      const T inv_eps{static_cast<T>(1.0) / (2 * __eps)};

      tensor<T, Direction::dimension(), Direction::rank()> Dp(__A);
      tensor<T, Direction::dimension(), Direction::rank()> Dm(Dp);

      auto diff_kernal =
          [&](auto... ONumbers) {
            auto tuple = std::make_tuple(ONumbers...);

            set_basis<SymDirection...>::add(Dp, tuple, +eps_half);
            set_basis<SymDirection...>::add(Dm, tuple, -eps_half);
            // FIX: Use the type alias instead of constexpr auto FuncRank
            const func_result_tensor Ap{__func(Dp)};
            const func_result_tensor Am{__func(Dm)};
            set_basis<SymDirection...>::add(Dp, tuple, -eps_half);
            set_basis<SymDirection...>::add(Dm, tuple, +eps_half);

            auto diff_kernal = [&](auto ...INumbers){
                const auto tuple = std::make_tuple(ONumbers..., INumbers...);
                set_basis<SymResult...>::set(__result, tuple, (Ap(INumbers...) - Am(INumbers...))*inv_eps);
            };
            function_loop::loop(diff_kernal);
          };

      direction_loop::loop(diff_kernal);
    }
};

} // NAMESPACE DETAIL

#endif // NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_BONES_H
