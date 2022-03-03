#include <iostream>
#include <tmech/tmech.h>

int main() {
    constexpr std::size_t Dim{3};
    tmech::tensor<double, Dim, 2> a{tmech::sym(tmech::randn<double,Dim,2>())};

    std::cout<<"Eigenvalue decomposition of symmetric tensor a:\n";
    std::cout<<a<<"\n";

    auto eiga{tmech::eigen_decomposition(a)};

    //decompose eigval and eigvec
    const auto [eigval, eigbasis]{eiga.decompose_eigenbasis()};

    std::cout<<"Eigenvalues are sorted eigval[0] <= eigval[1] <= eigval[2]\n";
    for(std::size_t i{0}; i<Dim; ++i){
        std::cout<<"Eigenvalue "<<i<<": "<<eigval[i]<<"\n";
    }

    std::cout<<"\n";

    std::cout<<"The corresponding eigenbasis\n";
    for(std::size_t i{0}; i<Dim; ++i){
        std::cout<<"Eigenbasis "<<i<<": \n"<<eigbasis[i];
    }
    return 0;
}
