#include <iostream>
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> A{3.0, -0.5, 0.3,
                                  0.7,  2.5, -0.4,
                                  -0.2, 0.6, 4.0};

    const auto A_svd = tmech::svd(A);
    const auto A_rec = A_svd.U() * A_svd.S() * tmech::trans(A_svd.V());

    std::cout << std::boolalpha
              << tmech::almost_equal(A, A_rec, 5e-7) << '\n';

    return 0;
}
