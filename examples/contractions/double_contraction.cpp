#include <iostream>
#include <tmech/tmech.h>

int main() {
    //double contraction
    tmech::tensor<double, 3, 2> a, b;
    tmech::tensor<double, 3, 4> A, B, C;

    a.randn();
    b.randn();
    A.randn();
    B.randn();

    //two second order tensors
    double val{tmech::dcontract(a,b)};
    //fourth and second order
    b = tmech::dcontract(B,a);
    //fourth and fourth order
    C = tmech::dcontract(B, A);
    //aliasing
    A = tmech::eval(tmech::dcontract(B, A));
    return 0;
}
