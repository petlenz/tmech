#include <iostream>
#include <tmech/tmech.h>

int main() {
    constexpr std::size_t Dim{3};
    tmech::tensor<double, Dim, 2> a{1,0,0,
                                    0,2,0,
                                    0,0,1};

    std::cout<<"Eigenvalue decomposition of symmetric tensor a:\n";
    std::cout<<a<<"\n";

    auto eiga{tmech::eigen_decomposition(a)};

    //decompose eigval and eigvec
    const auto [eigval, eigbasis]{eiga.decompose_eigenbasis()};
    const auto non_repeated{eiga.non_repeated_eigenvalues_index()};
    std::cout<<"Eigenvalues are sorted eigval[0] <= eigval[1] <= eigval[2]\n";
    for(const auto idx : non_repeated){
        std::cout<<"Eigenvalue "<<idx<<": "<<eigval[idx]<<"\n";
    }

    std::cout<<"\n";

    std::cout<<"The corresponding eigenbasis\n";
    for(const auto idx : non_repeated){
        std::cout<<"Eigenbasis "<<idx<<": \n"<<eigbasis[idx];
    }
    return 0;
}
