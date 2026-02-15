/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GENERAL_NEWTON_MEAT_H
#define GENERAL_NEWTON_MEAT_H

namespace detail {

template <typename System, typename Vector_x, typename Vector_dx, typename T>
constexpr inline auto
general_newton_raphson_solver::apply(System &A, Vector_x &x, Vector_dx &dx,
                                     [[maybe_unused]] T const tol) noexcept {
  constexpr std::size_t size{std::tuple_size_v<Vector_x>};

  // get jacobian and residuum
  auto [J, R]{A(x)};

  //    if(std::sqrt(norm_tuple<0, size>(R)) == 0){
  //        return 0.0;
  //    }

  // solve
  general_lu_solver::apply(J, R, dx);

  // update
  update<0, size>(x, dx);

  // return norm
  return std::sqrt(norm_tuple<0, size>(dx));
}

template<std::size_t I, std::size_t Size, typename Vector_x, typename Vector_dx>
constexpr inline auto general_newton_raphson_solver::update(Vector_x & x, Vector_dx const& dx)noexcept{
    if constexpr (I < Size){
        std::get<I>(x) -= std::get<I>(dx);
        update<I+1, Size>(x, dx);
    }
}

template<std::size_t I, std::size_t Size, typename Vector_dx>
constexpr inline auto general_newton_raphson_solver::norm_tuple(Vector_dx const& dx)noexcept{
    if constexpr (I < Size){
        const auto norm_temp{norm(std::get<I>(dx))};
        if constexpr(I == Size-1){
            return norm_temp*norm_temp;
        }else{
            return  norm_temp*norm_temp + norm_tuple<I+1, Size>(dx);
        }
    }
}

template<typename T>
constexpr inline auto general_newton_raphson_solver::norm(T const& dx)noexcept{
    if constexpr (std::is_fundamental_v<T>){
        return dx;
    }else{
        return tmech::norm(dx);
    }
}

} // NAMESPACE DETAIL

#endif // GENERAL_NEWTON_MEAT_H
