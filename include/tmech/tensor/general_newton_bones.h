/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GENERAL_NEWTON_BONES_H
#define GENERAL_NEWTON_BONES_H

namespace detail {

class general_newton_raphson_solver{

    template <typename T>
    struct get_return_type{
        using type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
    };

    template <typename ...Data>
    struct get_return_base;

    template <typename ...Data>
    struct get_return_base<std::tuple<Data...>>{
        using type = std::tuple<typename get_return_type<Data>::type...>;
    };

public:
    template<typename Argument>
    using result_type = typename get_return_base<Argument>::type;


    template<typename System, typename Vector_x, typename Vector_dx, typename T>
    static constexpr inline auto apply(System & A, Vector_x & x, Vector_dx & dx, T const tol){
        constexpr std::size_t size{std::tuple_size_v<Vector_x>};

        //get jacobian and residuum
        auto [J, R]{A(x)};

        //solve
        general_lu_solver::apply(J, R, dx);

        //update
        update<0, size>(x, dx);

        //return norm
        return std::sqrt(norm_tuple<0, size>(dx));
    }

private:
    template<std::size_t I, std::size_t Size, typename Vector_x, typename Vector_dx>
    static constexpr inline auto update(Vector_x & x, Vector_dx const& dx);

    template<std::size_t I, std::size_t Size, typename Vector_dx>
    static constexpr inline auto norm_tuple(Vector_dx const& dx);

    template<typename T>
    static constexpr inline auto norm(T const& dx);
};

} // NAMESPACE DETAIL
#endif // GENERAL_NEWTON_BONES_H
