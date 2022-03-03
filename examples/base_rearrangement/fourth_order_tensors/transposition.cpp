#include <iostream>
#include <tmech/tmech.h>

int main() {
    //4th order tensors
    tmech::tensor<double, 2, 4> a,b;
    a.randn();
    std::cout<<"Mayjor transpose\n";
    b = tmech::trans(a);
    std::cout<<"a: \n";
    std::cout<<a<<"\n";
    std::cout<<"b = trans(a): \n";
    std::cout<<b<<"\n";

    std::cout<<"a = eval(trans(a)): \n";
    a = tmech::eval(tmech::trans(a));
    std::cout<<a<<"\n";
    return 0;
}
