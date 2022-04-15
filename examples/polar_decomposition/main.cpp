#include <iostream>
#include <tmech/tmech.h>


int main() {
    tmech::tensor<double, 2, 2> F;
    F.randn();
    auto F_UR = tmech::polar_decomposition(F);
    //F = F_UR.R()*F_UR.U();
    std::cout<<std::boolalpha<<tmech::almost_equal(F, F_UR.R()*F_UR.U(), 5e-7)<<std::endl;

    return 0;
}
