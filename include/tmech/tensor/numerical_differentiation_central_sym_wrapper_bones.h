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


template <typename Result, std::size_t Dim, std::size_t End, std::size_t Start = 0>
struct basis_pair_loop_h
        : public basis_pair_loop_h<basis_pair<basis_pair_t<Result, Dim>>, Dim, End, Start+1>
{
    using type = Result;
};

template <std::size_t Dim, std::size_t End, std::size_t Start>
struct basis_pair_loop_h<void, Dim, End, Start>
        : public basis_pair_loop_h<basis_pair<basis_pair_t<void, Dim>>, Dim, End, Start+1>
{};

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
struct basis_pair_loop
        : public basis_pair_loop_h<void, Dim, End>
{};


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

/**
* @class numdiff_central_symmetric
* @brief Class for the numerical differentiation of scalar, second order and
* fourth order tensor valued functions. The differentiation is based on the
* backward scheme, see e.g. <a href="https://en.wikipedia.org/wiki/Numerical_differentiation">numerical differentiation</a>,
* <a href="https://en.wikipedia.org/wiki/Finite_difference">finite difference</a>.
* Note, that the differentiation rule given in
* <a href="https://onlinelibrary.wiley.com/doi/abs/10.1002/zamm.200410242">O. Kintzel</a>. is used.
*
* Scalar valued functions of second-order tensors
* \f[
*   \frac{\partial \phi}{\partial C_{ij}} = \frac{\phi[\SecondT{C}] - \phi[\SecondT{C}_{ij}^\epsilon]}{\epsilon} + \mathcal{O}(\epsilon),
* \f]
* where \f$\SecondT{C}_{ij}^\epsilon= \SecondT{C} - \epsilon ~\Ei\otimes\Ej\f$.
*
* Second-order valued tensor functions wrt. a second order tensor
* \f[
* \frac{\partial \SecondT{S}}{\partial C_{ij}} = \frac{\SecondT{S}[\SecondT{C}] - \SecondT{S}[\SecondT{C}_{ij}^\epsilon]}{\epsilon} + \mathcal{O}(\epsilon),
* \f]
* where \f$\SecondT{C}_{ij}^{+\epsilon}= \SecondT{C} +\frac{\epsilon}{2}  ~\Ei\otimes\Ej\f$
*
* Fourth-order valued tensor functions wrt. a second order tensor
* \f[
* \frac{\partial \FourthT{S}}{\partial C_{ij}} = \frac{\FourthT{S}[\SecondT{C}] - \FourthT{S}[\SecondT{C}_{ij}^\epsilon]}{\epsilon} + \mathcal{O}(\epsilon),
* \f]
* where \f$\SecondT{C}_{ij}^\epsilon= \SecondT{C} - \epsilon ~\Ei\otimes\Ej\f$
*
*
* @tparam Function Data type of the function, which should be differentiate wrt. a tensor.
* @tparam Point Data type of the point.
*/
//Tuple Direction Tuple Results
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



    template <typename Function, typename Direction, typename Result, typename T>
    static constexpr inline auto tensor_wrt_tensor(Function & __func, Direction const& __A, Result & __result, T const __eps)noexcept{
        constexpr auto FuncRank{decltype (__func(__A))::rank()};
        using direction_loop = basis_pair_loop<Direction::dimension(), Direction::rank()/2ul>;
        //using function_loop = basis_pair_loop<Direction::dimension(), FuncRank/2ul>;
        using function_loop  = typename meta_for_loop_deep<Direction::dimension(), FuncRank-1>::type;

        const T eps_half{__eps/(sizeof... (SymDirection))};
        const T inv_eps{static_cast<T>(1.0)/(2*__eps)};

        tensor<T, Direction::dimension(), Direction::rank()> Dp(__A);
        tensor<T, Direction::dimension(), Direction::rank()> Dm(Dp);

        auto diff_kernal = [&](auto ...ONumbers){
            auto tuple = std::make_tuple(ONumbers...);

            set_basis<SymDirection...>::add(Dp, tuple, +eps_half);
            set_basis<SymDirection...>::add(Dm, tuple, -eps_half);
            const tensor<T, Direction::dimension(), FuncRank> Ap{__func(Dp)};
            const tensor<T, Direction::dimension(), FuncRank> Am{__func(Dm)};
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




////template <typename Function, typename Direction>
//class numerical_differentiation_central_sym_wrapper : public tensor_base<numerical_differentiation_central_sym_wrapper>
//{
//public:
//    //    //return type of the function
//    //    //could be fundamental or a tensor expression
//    //    using return_type = decltype (Function()(Direction()));
//    //    using T  = typename Direction::T;
//    using size_type   = std::size_t;
//    //    using data_type   = typename num_diff_get_data_type_result<Function, Direction, std::is_fundamental_v<return_type>>::type;

//    //    numerical_differentiation_central_sym_wrapper(Function & func, Direction const& A, T const eps);

//    //    numerical_differentiation_central_sym_wrapper(numerical_differentiation_central_sym_wrapper const& data);

//    //    template<typename ...Indicies>
//    //    constexpr inline auto operator ()(Indicies const ... indices)const;

//    //    constexpr static inline auto rank();

//    //    constexpr static inline auto dimension();

//    //    template<typename Result>
//    //    constexpr inline auto evaluate(Result & result);

//    //    constexpr inline auto evaluate();

//    //    constexpr inline auto raw_data()const;

//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto differentiation(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        using output = decltype (__func(__A));

//        if constexpr (std::is_fundamental_v<Direction> == false && std::is_fundamental_v<output> == true) {
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
//    /**
//    * @name Evaluation implementation
//    */
//    //@{
//    /**
//    * @brief Numerical differentiation of scalar valued functions of
//    * second-order tensors wrt. a second-order tensor.
//    *
//    * @param result Internal or external tensor to save the result.
//    */
//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_0_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const T eps_half{__eps};
//        const T inv_eps{1.0/(__eps*2)};
//        tensor<T, Dim, 2> Dp(__A), Dm(__A);
//        for(size_type m{0}; m < Dim; ++m){
//            for(size_type n{m}; n < Dim; ++n){
//                if(m == n){
//                    Dp(m,n) += eps_half;
//                    Dm(m,n) -= eps_half;
//                }else{
//                    Dp(m,n) += eps_half*0.5;
//                    Dp(n,m) += eps_half*0.5;
//                    Dm(m,n) -= eps_half*0.5;
//                    Dm(n,m) -= eps_half*0.5;
//                }
//                //            Dp(m,n) += eps_half;
//                //            Dp(n,m) += eps_half;
//                //            Dm(m,n) -= eps_half;
//                //            Dm(n,m) -= eps_half;
//                const auto ap{__func(Dp)};
//                const auto am{__func(Dm)};
//                //            Dp(m,n) -= eps_half;
//                //            Dp(n,m) -= eps_half;
//                //            Dm(m,n) += eps_half;
//                //            Dm(n,m) += eps_half;

//                if(m == n){
//                    Dp(m,n) -= eps_half;
//                    Dm(m,n) += eps_half;
//                }else{
//                    Dp(m,n) -= eps_half*0.5;
//                    Dp(n,m) -= eps_half*0.5;
//                    Dm(m,n) += eps_half*0.5;
//                    Dm(n,m) += eps_half*0.5;
//                }

//                if(n==m){
//                    __result(m,n) = (ap - am)*inv_eps;
//                }else{
//                    __result(n,m) = __result(m,n) = (ap - am)*inv_eps;
//                }
//            }
//        }
//    }

//    /**
//    * @brief Numerical differentiation of second-order tensor
//    * valued functions  wrt. a second-order tensor.
//    *
//    * @param result Internal or external tensor to save the result.
//    */
//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_2_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const T eps_half{__eps};
//        const T inv__eps{1.0/(__eps*2)};
//        tensor<T, Dim, 2> Dp(__A), Dm(__A);
//        //DC_{imnj} =  (C_ij + C_ji)*0.5 / A_mn
//        for(size_type m{0}; m < Dim; ++m){
//            for(size_type n{m}; n < Dim; ++n){
//                if(m == n){
//                    Dp(m,n) += eps_half;
//                    Dm(m,n) -= eps_half;
//                }else{
//                    Dp(m,n) += eps_half*0.5;
//                    Dp(n,m) += eps_half*0.5;
//                    Dm(m,n) -= eps_half*0.5;
//                    Dm(n,m) -= eps_half*0.5;
//                }

//                const tensor<T, Dim, 2> ap{__func(Dp)};
//                const tensor<T, Dim, 2> am{__func(Dm)};

//                if(m == n){
//                    Dp(m,n) -= eps_half;
//                    Dm(m,n) += eps_half;
//                }else{
//                    Dp(m,n) -= eps_half*0.5;
//                    Dp(n,m) -= eps_half*0.5;
//                    Dm(m,n) += eps_half*0.5;
//                    Dm(n,m) += eps_half*0.5;
//                }

//                for(size_type i{0}; i<Dim; ++i){
//                    for(size_type j{i}; j<Dim; ++j){
//                        const auto value{(ap(i,j)-am(i,j))*inv__eps};
//                        if(i == j){
//                            if(m == n){
//                                __result(i,j,m,n) = value;
//                            }else{
//                                __result(i,j,m,n) = value;
//                                __result(i,j,n,m) = value;
//                            }
//                        }else{
//                            if(m == n){
//                                __result(i,j,m,n) = value;
//                                __result(j,i,n,m) = value;
//                            }else{
//                                __result(i,j,m,n) = value;
//                                __result(j,i,m,n) = value;
//                                __result(i,j,n,m) = value;
//                                __result(j,i,n,m) = value;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

//    /**
//    * @brief Numerical differentiation of fourth-order tensor
//    * valued functions  wrt. a second-order tensor.
//    *
//    * @param result Internal or external tensor to save the result.
//    */
//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_4_2(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const T eps_half{__eps*0.25};
//        tensor<T, Dim, 2> Dp(__A), Dm(__A);
//        const T inv__eps{1.0/__eps};
//        for(size_type m{0}; m<Dim; ++m){
//            for(size_type n{n}; n<Dim; ++n){
//                Dp(m,n) += eps_half;
//                Dp(n,m) += eps_half;
//                Dm(m,n) -= eps_half;
//                Dm(n,m) -= eps_half;
//                const tensor<T, Dim, 4> ap{__func(Dp)};
//                const tensor<T, Dim, 4> am{__func(Dm)};
//                Dp(m,n) -= eps_half;
//                Dp(n,m) -= eps_half;
//                Dm(m,n) += eps_half;
//                Dm(n,m) += eps_half;
//                for(size_type i{0}; i<Dim; ++i){
//                    for(size_type j{0}; j<Dim; ++j){
//                        for(size_type k{0}; k<Dim; ++k){
//                            for(size_type l{0}; l<Dim; ++l){
//                                const auto value{(ap(i,j,k,l)-am(i,j,k,l))*inv__eps};
//                                __result(i,j,m,n,k,l) = value;
//                                __result(i,j,n,m,k,l) = value;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

//    //supersymmetric
//    template <typename Function, typename Direction, typename Result, typename T>
//    static constexpr inline auto diff_4_4(Function & __func, Direction const& __A, Result & __result, T const __eps){
//        constexpr auto Dim{Direction::dimension()};
//        const auto eps_half{__eps};
//        const auto inv_eps{static_cast<T>(1.0)/(__eps)};
//        tensor<T, Dim, 4> Dp(__A), Dm(__A);

//        for(size_type m{0}; m<Dim; ++m){
//            for(size_type n{0}; n<Dim; ++n){
//                for(size_type o{0}; o<Dim; ++o){
//                    for(size_type p{0}; p<Dim; ++p){
//                        const auto mn{m == n};
//                        const auto op{o == p};

//                        tensor<T, Dim, 1> em,en,eo,ep;
//                        em(m) = 1;
//                        en(n) = 1;
//                        eo(o) = 1;
//                        ep(p) = 1;
//                        const auto A{otimes(otimes(em,en),otimes(eo,ep))};
//                        const auto AD{otimes(otimes(eo,ep),otimes(em,en))};
//                        const auto Al{otimes(otimes(en,em),otimes(eo,ep))};
//                        const auto Ar{otimes(otimes(em,en),otimes(ep,eo))};
//                        const auto Asym{(A+AD+Al+Ar)*0.25};


//                        const tensor<T, Dim, 4> ap{__func(Asym*__eps*0.5)};
//                        const tensor<T, Dim, 4> am{__func(-Asym*__eps*0.5)};


////                        if(mn && op){
////                            Dp(m,n,o,p) += eps_half;
////                            Dm(m,n,o,p) -= eps_half;
////                        }else if(mn && !op){
////                            Dp(m,n,o,p) += eps_half*0.5;
////                            Dp(m,n,p,o) += eps_half*0.5;

////                            Dm(m,n,o,p) -= eps_half*0.5;
////                            Dm(m,n,p,o) -= eps_half*0.5;
////                        }else if(!mn && op){
////                            Dp(m,n,o,p) += eps_half*0.5;
////                            Dp(n,m,o,p) += eps_half*0.5;

////                            Dm(m,n,o,p) -= eps_half*0.5;
////                            Dm(n,m,o,p) -= eps_half*0.5;
////                        }else if (!mn && !op) {
////                            Dp(m,n,o,p) += eps_half*0.25;
////                            Dp(m,n,p,o) += eps_half*0.25;
////                            Dp(n,m,o,p) += eps_half*0.25;
////                            Dp(o,p,m,n) += eps_half*0.25;

////                            Dm(m,n,o,p) -= eps_half*0.25;
////                            Dm(m,n,p,o) -= eps_half*0.25;
////                            Dm(n,m,o,p) -= eps_half*0.25;
////                            Dm(o,p,m,n) -= eps_half*0.25;
////                        }

////                        //                        Dp(m,n,o,p) += eps_half*0.25;
////                        //                        Dp(m,n,p,o) += eps_half*0.25;
////                        //                        Dp(n,m,o,p) += eps_half*0.25;
////                        //                        Dp(o,p,m,n) += eps_half*0.25;

////                        //                        Dm(m,n,o,p) -= eps_half*0.25;
////                        //                        Dm(m,n,p,o) -= eps_half*0.25;
////                        //                        Dm(n,m,o,p) -= eps_half*0.25;
////                        //                        Dm(o,p,m,n) -= eps_half*0.25;

////                        const tensor<T, Dim, 4> ap{__func(Dp)};
////                        const tensor<T, Dim, 4> am{__func(Dm)};

////                        if(mn && op){
////                            Dp(m,n,o,p) -= eps_half;
////                            Dm(m,n,o,p) += eps_half;
////                        }else if(mn && !op){
////                            Dp(m,n,o,p) -= eps_half*0.5;
////                            Dp(m,n,p,o) -= eps_half*0.5;

////                            Dm(m,n,o,p) += eps_half*0.5;
////                            Dm(m,n,p,o) += eps_half*0.5;
////                        }else if(!mn && op){
////                            Dp(m,n,o,p) -= eps_half*0.5;
////                            Dp(n,m,o,p) -= eps_half*0.5;

////                            Dm(m,n,o,p) += eps_half*0.5;
////                            Dm(n,m,o,p) += eps_half*0.5;
////                        }else if (!mn && !op) {
////                            Dp(m,n,o,p) -= eps_half*0.25;
////                            Dp(m,n,p,o) -= eps_half*0.25;
////                            Dp(n,m,o,p) -= eps_half*0.25;
////                            Dp(o,p,m,n) -= eps_half*0.25;

////                            Dm(m,n,o,p) += eps_half*0.25;
////                            Dm(m,n,p,o) += eps_half*0.25;
////                            Dm(n,m,o,p) += eps_half*0.25;
////                            Dm(o,p,m,n) += eps_half*0.25;
////                        }

//                        //                        Dp(m,n,o,p) -= eps_half*0.25;
//                        //                        Dp(m,n,p,o) -= eps_half*0.25;
//                        //                        Dp(n,m,o,p) -= eps_half*0.25;
//                        //                        Dp(o,p,m,n) -= eps_half*0.25;

//                        //                        Dm(m,n,o,p) += eps_half*0.25;
//                        //                        Dm(m,n,p,o) += eps_half*0.25;
//                        //                        Dm(n,m,o,p) += eps_half*0.25;
//                        //                        Dm(o,p,m,n) += eps_half*0.25;

//                        //std::cout<<(ap - am)*inv_eps<<std::endl;

//                        for(size_type i{0}; i<Dim; ++i){
//                            for(size_type j{0}; j<Dim; ++j){
//                                for(size_type k{0}; k<Dim; ++k){
//                                    for(size_type l{0}; l<Dim; ++l){


//                                        __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;



////                                        const auto ij{i == j};
////                                        const auto kl{k == l};

////                                        if(ij && kl && mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && kl && mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;

////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && kl && !mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && kl && !mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && !kl && mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && !kl && !mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && !kl && mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(ij && !kl && !mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && kl && mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && kl && mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && kl && !mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && kl && !mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && !kl && mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && !kl && !mn && op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && !kl && mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else if(!ij && !kl && !mn && !op){
////                                            __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                            __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                            __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        }else {
////                                            assert(0);
////                                        }
//                                        //ij kl mn op
//                                        //ij kl mn !op
//                                        //ij kl !mn op
//                                        //ij kl !mn !op
//                                        //ij !kl mn op
//                                        //ij !kl !mn op
//                                        //ij !kl mn !op
//                                        //ij !kl !mn !op

//                                        //ij! kl mn op
//                                        //ij! kl mn !op
//                                        //ij! kl !mn op
//                                        //ij! kl !mn !op
//                                        //ij! !kl mn op
//                                        //ij! !kl !mn op
//                                        //ij! !kl mn !op
//                                        //ij! !kl !mn !op

////                                        __result(i,j,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                        __result(j,i,m,n,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(j,i,m,n,p,o,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(j,i,n,m,o,p,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(j,i,o,p,m,n,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                        __result(i,j,m,n,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,m,n,p,o,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,n,m,o,p,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,o,p,m,n,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                        __result(k,l,m,n,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(k,l,m,n,p,o,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(k,l,n,m,o,p,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(k,l,o,p,m,n,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;


////                                        __result(i,j,,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(j,i,,k,l) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(i,j,,l,k) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(k,l,,i,j) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;

////                                        __result(,m,n,o,p,) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(,m,n,p,o,) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(,n,m,o,p,) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;
////                                        __result(,o,p,m,n,) += (ap(i,j,k,l)-am(i,j,k,l))*inv_eps;



//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    //@}num_diff_central
//};


} // NAMESPACE DETAIL

#endif // NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_BONES_H
