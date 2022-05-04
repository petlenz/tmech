#ifndef NEWTON_RAPHSON_SOLVER_BONES_H
#define NEWTON_RAPHSON_SOLVER_BONES_H

namespace detail {

template <typename _Jacobi, typename _Functon>
class newton_raphson_solver
{
public:
    using value_type = typename _Jacobi::value_type;
    using size_type = std::size_t;

    newton_raphson_solver():
        _J(),
        _f()
    {}

    template<typename VectorX>
    constexpr inline auto operator()(VectorX const& __X){
        _J.update(__X);
        _f.update(__X);
        return std::make_tuple(_J.data(), _f.data());
    }

    template<typename _Data>
    inline auto solve(_Data & __data){
        const auto [iter, norm, x_new]{tmech::general_newton_raphson_iterate(*this, __data, 1e-8, 50)};
    }

private:
    _Jacobi _J;
    _Functon _f;
};


}

#endif // NEWTON_RAPHSON_SOLVER_BONES_H
