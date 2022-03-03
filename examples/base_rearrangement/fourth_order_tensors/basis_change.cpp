#include <iostream>
#include <tmech/tmech.h>

int main() {
    //4th order tensors
    tmech::tensor<double, 2, 4> a,b;
    a.randn();
    
    //b_{ijkl} = a_{iklj}
    b = tmech::basis_change<tmech::sequence<1,3,4,2>>(a);
    
    return 0;
}
