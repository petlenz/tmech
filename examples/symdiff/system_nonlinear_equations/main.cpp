#include <iostream>
#include <tmech/tmech.h>


class nonlinear_system
{
public:
    using value_type = double;

    nonlinear_system() {}

    template<typename VectorX>
    inline auto operator()(VectorX const& __X){
        symdiff::variable<value_type,0> x1;
        symdiff::variable<value_type,1> x2;
        symdiff::real<value_type,1,0,1> _1;
        symdiff::real<value_type,4,0,1> _4;

        auto R1 = x1*x1 + x2*x2 - _4;
        auto R2 = x1*x1 - x2 + _1;

        auto J11 = symdiff::derivative<1>(R1,x1);
        auto J12 = symdiff::derivative<1>(R1,x2);
        auto J21 = symdiff::derivative<1>(R2,x1);
        auto J22 = symdiff::derivative<1>(R2,x2);

        auto R{std::make_tuple(R1(__X), R2(__X))};
        auto J{std::make_tuple(std::make_tuple(J11(__X), J12(__X)),
                               std::make_tuple(J21(__X), J22(__X)))};
        return std::make_tuple(J, R);
    }

    inline auto solve(value_type const __x1, value_type const __x2){
        auto X{std::make_tuple(__x1, __x2)};
        const auto [iter, norm, x_new]{tmech::general_newton_raphson_iterate(*this, X, 1e-8, 50)};
        std::cout<<"iter "<<iter<<" norm "<<norm<<" results x1 "<<std::get<0>(x_new)<<" x2 "<<std::get<1>(x_new)<<std::endl;
    }
};


int main() {
    
    nonlinear_system system;
    //as input start values
    system.solve(1, 2);
    
    return 0;
}
