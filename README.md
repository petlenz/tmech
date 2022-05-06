## Introduction

`tmech` is a C++ library for the numerical study of the physics of continuous materials using higher-order tensors

`tmech` provides

- an extensible expression system enabling lazy evaluation
- tmech is a high quality tensor algebra library for the C++ language, aiming towards a good balance between speed and ease of use
- Integer and floating point numbers are fully supported, complex numbers are supported only for basic arithmetic operations
- Provides high-level syntax and functionality for easy use
- Can be used for solid mechanics, fluid mechanics, ...
- isotropic tensor functions
- compile-time differentiation of scalar and tensorial expressions
- many more...


`tmech` requires a modern C++ compiler supporting C++17. The following C++
compilers are supported:

- On Unix platforms, gcc 5 or a recent version of Clang
- On Windows platforms not tested

## Install from sources

`tmech` is a header-only library.

You can directly install it from the sources:

```bash
cmake -DCMAKE_INSTALL_PREFIX=your_install_prefix
make install
```

## Documentation

For more information on using `tmech`, check out the documentation

http://tmech.readthedocs.io/

## Issues and enhancements

We use the [GitHub issue
tracker](https://github.com/petlenz/tmech/issues) for all bug/issue
reports and proposals for enhancements.

## Contributing

We welcome contributions directly to the code to improve performance as
well as new functionality. For the latter, please first explain and
motivate it in an [issue](https://github.com/petlenz/tmech/issues).

Changes to the code base should follow these steps:

-   [Fork](https://docs.github.com/en/get-started/quickstart/fork-a-repo)
    the repository
-   [Make a
    branch](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-and-deleting-branches-within-your-repository)
    with an appropriate name in your fork
-   Implement changes in your fork and include at least one [gtest](https://github.com/petlenz/tmech/blob/master/tests/tmech_test.h) to test your changes.
-   Create a [pull
    request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork)
    into the `main` branch of `tmech`

## Usage

### Basic usage
This example simply adds two second order tensors
```cpp
#include <iostream>
#include <tmech/tmech.h>
...
constexpr std::size_t Rank{2}, Dim{3};
tmech::tensor<double, Dim, Rank> A{1, 2, 3,
                                   4, 5, 6,
                                   7, 8, 9};
tmech::tensor<double, Dim, Rank> B{9, 8, 7,
                                   6, 5, 4,
                                   3, 2, 1};

tmech::tensor<double, Dim, Rank> res = A + B;

std::cout << res << std::endl;
```

Outputs:
```
10 10 10
10 10 10
10 10 10
```

### Basic usage of symdiff

```cpp
#include <iostream>
#include <tmech/tmech.h>

using tensor2 = tmech::tensor<double,3,2>;
using tensor4 = tmech::tensor<double,3,4>;
tensor2 eps = tmech::sym(tmech::randn<double,3,2>());
...
symdiff::variable<tensor2, 0> _eps;
symdiff::constant<double, 0> _lambda(150, "lambda");
symdiff::constant<double, 1> _mu(200, "mu");
symdiff::real<double, 2,0,1> _2;
symdiff::constant<tensor2, 2> _I("I");
_I = tmech::eye<double,3,2>();

//define function
auto _sig = _lambda*tmech::trace(tmech::as_sym(_eps))*_I + _2*_mu*tmech::as_sym(_eps);

//get first derivative
auto _C = symdiff::derivative(_sig, _eps);
//print the result
std::cout<<_C<<std::endl;

//evaluate the function at eps
tensor4 C = _C(std::make_tuple(eps));
//evaluate the function at eps
tensor2 sig = _sig(std::make_tuple(eps));
```

Outputs:
```
(otimes((lambda*I), I)+((2*mu)*(0.5*(otimesu(I, I)+otimesl(I, I)))))
```

