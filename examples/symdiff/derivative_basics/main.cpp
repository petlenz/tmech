#include <iostream>
#include <tmech/tmech.h>

// Minimal symbolic-differentiation example (used in the JOSS paper).
// Declare a symbolic tensor variable, build a scalar-valued function of it,
// and differentiate it in closed form: d det(x)/dx = cof(x).
int main() {
    using T2 = tmech::tensor<double, 3, 2>;
    symdiff::variable<T2, 0> x;

    auto f  = tmech::det(x);              // scalar-valued function of a tensor
    auto df = symdiff::derivative(f, x);  // closed-form derivative: cof(x)

    std::cout << "df = " << df << std::endl;

    // Evaluate the symbolic derivative at a concrete tensor and check it.
    T2 val  = tmech::sym(tmech::randn<double, 3, 2>());
    T2 grad = df(std::make_tuple(val));
    std::cout << std::boolalpha
              << "df(x) == cof(x) ? "
              << tmech::almost_equal(grad, T2(tmech::cof(val)), 5e-6) << std::endl;
    return 0;
}
