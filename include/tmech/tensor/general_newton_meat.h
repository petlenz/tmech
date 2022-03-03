#ifndef GENERAL_NEWTON_MEAT_H
#define GENERAL_NEWTON_MEAT_H

namespace detail {

template<std::size_t I, std::size_t Size, typename Vector_x, typename Vector_dx>
constexpr inline auto general_newton_raphson_solver::update(Vector_x & x, Vector_dx const& dx){
    if constexpr (I < Size){
        std::get<I>(x) -= std::get<I>(dx);
        update<I+1, Size>(x, dx);
    }
}

template<std::size_t I, std::size_t Size, typename Vector_dx>
constexpr inline auto general_newton_raphson_solver::norm_tuple(Vector_dx const& dx){
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
constexpr inline auto general_newton_raphson_solver::norm(T const& dx){
    if constexpr (std::is_fundamental_v<T>){
        return dx;
    }else{
        return tmech::norm(dx);
    }
}

} // NAMESPACE DETAIL

#endif // GENERAL_NEWTON_MEAT_H
