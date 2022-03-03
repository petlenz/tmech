#include <iostream>
#include <tmech/tmech.h>

int main() {
    //single contraction
    tmech::tensor<double, 3, 2> a, b, c;

    a.randn();
    b.randn();

    c = a*b;
    //aliasing
    a = tmech::eval(a*b);
    return 0;
}
