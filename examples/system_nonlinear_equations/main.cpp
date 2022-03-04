#include <iostream>
#include <tmech/tmech.h>


class nonlinear_system
{
public:
    using value_type = double;

    nonlinear_system() {}

    template<typename VectorX>
    constexpr inline auto operator()(VectorX const& __X){
        const value_type x1{std::get<0>(__X)};
        const value_type x2{std::get<1>(__X)};
	
	//residual
        const value_type R1 = x1*x1 + x2*x2 - 4;
        const value_type R2 = x1*x1 - x2 + 1;
	
	//jacobian entries
        const value_type J11 = 2*x1;
        const value_type J12 = 2*x2;
        const value_type J21 = 2*x1;
        const value_type J22 = -1;
        
        //residual
        auto R{std::make_tuple(R1, R2)};
        
        //jacobian
        auto J{std::make_tuple(std::make_tuple(J11, J12),
                               std::make_tuple(J21, J22))};
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
