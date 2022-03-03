## Introduction

`tmech` is a C++ library for the numerical analysis of physical effects with symbolic compile time differentiation.

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
