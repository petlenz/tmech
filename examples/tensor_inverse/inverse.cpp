#include <iostream>
#include <tmech/tmech.h>

int main() {
    constexpr std::size_t Dim{3};
    tmech::tensor<double, Dim, 2> a,b;
    tmech::tensor<double, Dim, 4> A,B;
    a.randn();
    b.randn();
    
    b = tmech::inv(a);
    //minor symmetry at basis pair 1,2 and 3,4 is assumed
    B = tmech::inv(A);
    //full inverse no symmetry is assumed
    B = tmech::invf<tmech::sequence<1,2,3,4>>(A);
    
    return 0;
}
