#include <iostream>
#include <tmech/tmech.h>

int main() {
    constexpr std::size_t Dim{3};
    tmech::tensor<double, Dim, 2> a,b;
    tmech::tensor<double, Dim, 4> A;
    a.randn();
    b.randn();
    
    A = tmech::otimes(a,b);
    A = tmech::otimesl(b,a);
    A = tmech::otimesu(a,b);
    
    A = tmech::outer_product<tmech::sequence<2,3>, tmech::sequence<1,4>>(a,b);
    return 0;
}
