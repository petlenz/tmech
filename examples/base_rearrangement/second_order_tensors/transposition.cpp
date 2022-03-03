#include <iostream>
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> a,b;
    a.randn();
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
