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

template<typename Jacobian, typename Residuum, typename Vector_x>
constexpr inline auto general_lu_solver::apply(Jacobian & A, Residuum const& R, Vector_x & x)noexcept{
    using elem_type = std::decay_t<decltype(std::get<0>(R))>;
    if constexpr (std::is_fundamental_v<elem_type>){
        // scalar system: use partial pivoting (robust against zero pivots)
        apply_pivoted(A, R, x);
    } else {
        // block/tensor system: original pivot-free tuple-recursive path
        constexpr std::size_t size{std::tuple_size_v<Residuum>};
        elimination_I<0>(A);
        forward_I<0>(A, R, x);
        backward_I<size-1>(A, x);
    }
}

template<typename Jacobian, typename Residuum, typename Vector_x>
inline auto general_lu_solver::apply_pivoted(Jacobian & A, Residuum const& R, Vector_x & x)noexcept{
    constexpr std::size_t N{std::tuple_size_v<Residuum>};
    using T = std::decay_t<decltype(std::get<0>(R))>;
    T m[N][N]{};
    T b[N]{};
    // gather tuple/array system into runtime matrix and rhs
    { std::size_t i{0};
      std::apply([&](auto const&... row){
          ([&](auto const& rw){ std::size_t j{0};
               std::apply([&](auto const&... e){ ((m[i][j++] = static_cast<T>(e)), ...); }, rw);
               ++i; }(row), ...); }, A); }
    { std::size_t i{0};
      std::apply([&](auto const&... e){ ((b[i++] = static_cast<T>(e)), ...); }, R); }
    // forward elimination with partial (row) pivoting
    for (std::size_t k{0}; k < N; ++k) {
        std::size_t p{k};
        T mx{std::abs(m[k][k])};
        for (std::size_t i{k + 1}; i < N; ++i) {
            const T v{std::abs(m[i][k])};
            if (v > mx) { mx = v; p = i; }
        }
        if (p != k) {
            for (std::size_t j{0}; j < N; ++j) { std::swap(m[k][j], m[p][j]); }
            std::swap(b[k], b[p]);
        }
        for (std::size_t i{k + 1}; i < N; ++i) {
            const T f{m[i][k] / m[k][k]};
            for (std::size_t j{k}; j < N; ++j) { m[i][j] -= f * m[k][j]; }
            b[i] -= f * b[k];
        }
    }
    // back substitution
    T sol[N]{};
    for (std::size_t i{N}; i-- > 0;) {
        T s{b[i]};
        for (std::size_t j{i + 1}; j < N; ++j) { s -= m[i][j] * sol[j]; }
        sol[i] = s / m[i][i];
    }
    // scatter solution back into x
    { std::size_t i{0};
      std::apply([&](auto&... e){ ((e = sol[i++]), ...); }, x); }
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
  [[maybe_unused]] constexpr std::size_t size{std::tuple_size_v<Jacobian>};
  if constexpr (K > I) {
    std::get<I>(x) = std::get<I>(x) -
                     inner_product(std::get<K>(std::get<I>(A)), std::get<K>(x));
    backward_K<I, K - 1>(A, x);
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
    if constexpr ((fundamental_LHS && fundamental_RHS) ||
                  (!fundamental_LHS && fundamental_RHS) ||
                  (fundamental_LHS && !fundamental_RHS)) {
      // scalar*scalar || tensor*scalar || scalar*tensor
      return A * B;
    } else {
      // tensor:tensor
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
