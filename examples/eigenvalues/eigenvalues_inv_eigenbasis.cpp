#include <iostream>
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> A, A_inv;
    A = tmech::sym(tmech::randn<double,3,2>());
    auto A_eig = tmech::eigen_decomposition(A);
    const auto[eigenvalues, eigenbasis]{A_eig.decompose_eigenbasis()};
    for(auto idx : A_eig.non_repeated_eigenvalues_index()){
        A_inv += (1.0/eigenvalues[idx])*eigenbasis[idx];
    }
    std::cout<<std::boolalpha<<tmech::almost_equal(tmech::inv(A), A_inv, 5e-7)<<std::endl;

    return 0;
}
