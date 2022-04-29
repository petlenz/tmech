/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GENERAL_LU_DECOMPOSITION_MEAT_H
#define GENERAL_LU_DECOMPOSITION_MEAT_H

namespace detail {

general_lu_solver::general_lu_solver()noexcept{}

template<typename Jacobian, typename Residuum, typename Vector_x>
constexpr inline auto general_lu_solver::apply(Jacobian & A, Residuum const& R, Vector_x & x)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Residuum>};

    //factorize
    elimination_I<0>(A);
    //forward
    forward_I<0>(A, R, x);
    //backward
    backward_I<size-1>(A, x);
}

template<std::size_t I, typename Jacobian>
constexpr inline auto general_lu_solver::elimination_I(Jacobian & A)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    //for(size_type i{0}; i<Rows; ++i){
    //    const T Akk = 1.0/A[i][i];
    //    for(std::size_t j{i+1};j<Rows;++j){
    //        A[j][i] *=Akk;
    //    }
    //    for(std::size_t j{i+1};j<Rows;++j){
    //        for(std::size_t k{i+1};k<Rows;++k){
    //            A[j][k] -= A[j][i]*A[i][k];
    //        }
    //    }
    //}

    if constexpr (I < size){
        const auto invAii{invert(std::get<I>(std::get<I>(A)))};
        elimination_J<I, I+1>(A, invAii);
        elimination_J<I, I+1>(A);
        elimination_I<I+1>(A);
    }
}


template<std::size_t I, std::size_t J, typename Jacobian, typename Val_inv>
constexpr inline auto general_lu_solver::elimination_J(Jacobian & A, Val_inv const& invAii)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    if constexpr (J < size){
        std::get<I>(std::get<J>(A)) = eval_data(inner_product(invAii, std::get<I>(std::get<J>(A))));
        elimination_J<I, J+1>(A,invAii);
    }
}

template<std::size_t I, std::size_t J, typename Jacobian>
constexpr inline auto general_lu_solver::elimination_J(Jacobian & A)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    if constexpr (J < size){
        elimination_K<I, J, I+1>(A);
        elimination_J<I, J+1>(A);
    }
}

template<std::size_t I, std::size_t J, std::size_t K, typename Jacobian>
constexpr inline auto general_lu_solver::elimination_K(Jacobian & A)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    if constexpr (K < size){
        std::get<K>(std::get<J>(A)) -= inner_product(std::get<I>(std::get<J>(A)), std::get<K>(std::get<I>(A)));
        elimination_K<I, J, K+1>(A);
    }
}



template<std::size_t I, typename Jacobian, typename Residuum, typename Vector_x>
constexpr inline auto general_lu_solver::forward_I(Jacobian & A, Residuum const& R, Vector_x & x)noexcept{
    //        for(size_type i{0};i<Rows;i++) {
    //            x[i] = b[ipiv[i]];
    //            for(size_type k{0};k<i;k++){
    //                x[i] -= A[i][k] * x[k];
    //            }
    //        }

    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    if constexpr (I < size){
        std::get<I>(x) = std::get<I>(R);
        if constexpr (I > 0){
            std::get<I>(x) -= eval_data(forward_K<I, 0>(A,x));
        }
        forward_I<I+1>(A, R, x);
    }
}

template<std::size_t I, std::size_t K, typename Jacobian, typename Vector_x>
constexpr inline auto general_lu_solver::forward_K(Jacobian & A, Vector_x & x)noexcept{
    if constexpr (K == I-1){
        return inner_product(std::get<K>(std::get<I>(A)), std::get<K>(x));
    }else{
        return inner_product(std::get<K>(std::get<I>(A)), std::get<K>(x)) + forward_K<I, K+1>(A, x);
    }
}


template<std::size_t I, typename Jacobian, typename Vector_x>
constexpr inline auto general_lu_solver::backward_I(Jacobian & A, Vector_x & x)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    //for (long long int i = Rows - 1; i >= 0; --i) {
    //    for (long long int k = Rows - 1; k > i; --k){
    //        x[i] -= A[i][k] * x[k];
    //    }
    //    x[i] /= A[i][i];
    //}
    if constexpr (I >= 0){
        backward_K<I, size-1>(A, x);
        //prevent aliasing
        std::get<I>(x) = eval_data(inner_product(invert(std::get<I>(std::get<I>(A))), std::get<I>(x)));
        if constexpr (I>0){
            backward_I<I-1>(A,x);
        }
    }
}

template<std::size_t I, std::size_t K, typename Jacobian, typename Vector_x>
constexpr inline auto general_lu_solver::backward_K(Jacobian & A, Vector_x & x)noexcept{
    constexpr std::size_t size{std::tuple_size_v<Jacobian>};
    if constexpr (K > I){
        std::get<I>(x) = std::get<I>(x) - inner_product(std::get<K>(std::get<I>(A)), std::get<K>(x));
        backward_K<I, K-1>(A, x);
    }
}

template<typename T>
constexpr inline auto general_lu_solver::invert(T const& A)noexcept{
    if constexpr (std::is_fundamental_v<T>){
        return static_cast<T>(1)/A;
    }else{
        return inv(A);
    }
}

template<typename T>
constexpr inline auto general_lu_solver::invert(T && A)noexcept{
    if constexpr (std::is_fundamental_v<typename std::decay<T>::type>){
        return static_cast<typename std::decay<T>::type>(1)/std::forward<T>(A);
    }else{
        return inv(std::forward<T>(A));
    }
}

template<typename LHS, typename RHS>
constexpr inline auto general_lu_solver::inner_product(LHS const& A, RHS const& B)noexcept{
    constexpr bool fundamental_LHS{std::is_fundamental_v<LHS>};
    constexpr bool fundamental_RHS{std::is_fundamental_v<RHS>};
    if constexpr (fundamental_LHS && fundamental_RHS || !fundamental_LHS && fundamental_RHS || fundamental_LHS && !fundamental_RHS){
        //scalar*scalar || tensor*scalar || scalar*tensor
        return A*B;
    }else{
        //tensor:tensor
        return dcontract(A, B);
    }
}

template<typename T>
constexpr inline auto general_lu_solver::eval_data(T const& data)noexcept{
    if constexpr (std::is_fundamental_v<T>){
        return data;
    }else {
        return eval(data);
    }
}

template<typename T>
constexpr inline auto general_lu_solver::eval_data(T && data)noexcept{
    if constexpr (std::is_fundamental_v<typename std::decay<T>::type>){
        return std::forward<T>(data);
    }else {
        return eval(std::forward<T>(data));
        //return inv(std::forward<T>(data));
    }
}

} // NAMESPACE DETAIL
#endif // GENERAL_LU_DECOMPOSITION_MEAT_H
