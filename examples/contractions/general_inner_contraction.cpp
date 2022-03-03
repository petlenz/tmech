#include <iostream>
#include <tmech/tmech.h>

int main() {
    using SeqLHS = tmech::sequence<3,4>;
    using SeqRHS = tmech::sequence<1,2>;
    
    tmech::tensor<double, 3, 4> A, B, C;

    A.randn();
    B.randn();

    C = tmech::inner_product<SeqLHS, SeqRHS>(B, A);
    //aliasing
    A = tmech::eval(tmech::inner_product<SeqLHS, SeqRHS>(B, A));
    return 0;
}
