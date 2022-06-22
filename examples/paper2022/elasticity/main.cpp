#include <iostream>
#include <tmech/tmech.h>


int main()
{
    using tensor2 = tmech::tensor<double,3,2>;
    symdiff::variable<tensor2,0> E("E");
    symdiff::constant<double,0> k(1,"k");
    symdiff::constant<double,1> mu(200,"mu");
    symdiff::real<double,2,0,1> _2;
    auto psi = k*std::pow(tmech::trace(tmech::as_sym(E)), _2)/_2+mu*std::pow(tmech::trace(tmech::dev(tmech::as_sym(E))), _2);

    std::cout<<symdiff::derivative(psi, E)<<std::endl;

    return 0;
}
