#include <iostream>
#include <tmech/tmech.h>




int main() {
    
    using tensor2 = tmech::tensor<double,3,2>;
    using tensor4 = tmech::tensor<double,3,4>;
    tensor2 eps = tmech::sym(tmech::randn<double,3,2>());

    symdiff::variable<tensor2, 0> _eps;
    symdiff::constant<double, 0> _lambda(150, "lambda");
    symdiff::constant<double, 1> _mu(200, "mu");
    symdiff::real<double, 2,0,1> _2;
    symdiff::constant<tensor2, 2> _I("I");
    _I = tmech::eye<double,3,2>();

    //define function
    auto _sig = _lambda*tmech::trace(tmech::as_sym(_eps))*_I + _2*_mu*tmech::as_sym(_eps);

    //get first derivative
    auto _C = symdiff::derivative(_sig, _eps);
    //print the result
    std::cout<<_C<<std::endl;

    //evaluate the function at eps
    tensor4 C = _C(std::make_tuple(eps));
    //evaluate the function at eps
    tensor2 sig = _sig(std::make_tuple(eps));
    
    return 0;
}
