# tmech

[![CI](https://github.com/petlenz/tmech/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/petlenz/tmech/actions/workflows/cmake-multi-platform.yml)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/std/the-standard)

A header-only C++17 library for higher-order tensor algebra in continuum mechanics.

## Features

- Lazy evaluation via an extensible expression template system
- Arbitrary-rank tensors (2nd, 4th, 8th order, ...)
- Full support for integer and floating-point types; basic complex arithmetic
- Tensor operations: contraction, outer products (`otimes`, `otimesu`, `otimesl`), inverse, determinant, trace, exponential, polar decomposition, and more
- Isotropic tensor functions with compile-time symbolic differentiation (`symdiff`)
- Optional SIMD acceleration via [xsimd](https://github.com/xtensor-stack/xsimd)
- FEM solver integration through adaptors (e.g. Abaqus UMAT/VUMAT)
- Applicable to solid mechanics, fluid mechanics, and other fields

## How it works

tmech uses **expression templates** with the Curiously Recurring Template Pattern
(CRTP) to build an abstract syntax tree of tensor operations at compile time.
No intermediate temporaries are allocated until the result is assigned to a
concrete tensor:

```cpp
// No computation happens here -- A + B and the otimes are stored as
// lightweight expression nodes in the type system.
auto expr = tmech::otimes(A + B, C);

// Evaluation happens here: the compiler fuses the entire expression tree
// into a single set of loops with no temporaries.
tmech::tensor<double, 3, 4> result = expr;
```

Every tensor type inherits from `tensor_base<Derived>`, which provides
operator overloads that return expression wrappers instead of evaluated
tensors. Each wrapper stores references to its operands and implements an
`operator()` for element access. When the result is finally assigned, the
compiler inlines the entire expression tree into tight, optimised code.

For performance-critical contractions, tmech provides a dedicated GEMM kernel
that can optionally use [xsimd](https://github.com/xtensor-stack/xsimd) for
portable SIMD vectorisation.

## Requirements

- C++17 compiler (GCC 7+, Clang 5+)
- CMake 3.5+

## Installation

`tmech` is header-only -- no compilation is needed to use it.

### Option 1: System install

```bash
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --install build
```

This installs the headers to `<prefix>/include/tmech/`.

### Option 2: CMake subdirectory

Add tmech directly to your project without installing:

```cmake
add_subdirectory(path/to/tmech)
target_link_libraries(your_target PRIVATE tmech::tmech)
```

### Option 3: find_package

After a system install, use `find_package` in your `CMakeLists.txt`:

```cmake
find_package(tmech REQUIRED)
target_link_libraries(your_target PRIVATE tmech::tmech)
```

### CMake options

| Option | Default | Description |
|---|---|---|
| `TMECH_USE_XSIMD` | `OFF` | Enable xsimd for SIMD-accelerated GEMM kernels (fetched automatically) |
| `TMECH_BUILD_TESTS` | `ON` | Build the test suite (requires GTest) |
| `TMECH_BUILD_EXAMPLES` | `ON` | Build example programs |
| `TMECH_BUILD_BENCHMARK` | `OFF` | Build benchmarks comparing tmech against Eigen, Fastor, etc. |
| `TMECH_INSTALL_LIBRARY` | `ON`* | Install headers and CMake config files (\*`ON` when tmech is the top-level project) |

### Building with xsimd

```bash
cmake -B build -S . -DTMECH_USE_XSIMD=ON
cmake --build build
```

xsimd 13.2.0 is fetched automatically via `FetchContent`. When enabled, the
`TMECH_HAS_XSIMD` compile definition is propagated to all targets that link
against `tmech`.

### Running tests

```bash
cmake -B build -S . -DTMECH_BUILD_TESTS=ON
cmake --build build
cd build && ctest --output-on-failure
```

### Preprocessor macros

tmech exposes several preprocessor macros for tuning behaviour. User-settable
macros can be defined before including `<tmech/tmech.h>` or passed via
`-D` on the compiler command line.

#### User-settable

| Macro | Default | Description |
|---|---|---|
| `TMECH_MAX_TREE_NODES_UNROLL` | `16` | Maximum number of expression-tree nodes that are fully unrolled at compile time. Larger values can improve runtime performance for deeply nested expressions at the cost of longer compile times and larger binaries. |
| `TMECH_MAX_LOOP_UNROLL_SIZE` | `256` | Maximum total element count for which index loops are fully unrolled. Loops over tensors with more elements than this threshold fall back to regular loops. |

Example -- raise the unroll limits for aggressive optimisation:

```cpp
#define TMECH_MAX_TREE_NODES_UNROLL 32
#define TMECH_MAX_LOOP_UNROLL_SIZE  512
#include <tmech/tmech.h>
```

#### Set by the build system

Only public/user-facing macros are listed here; internal implementation macros are intentionally omitted.

| Macro | Description |
|---|---|
| `TMECH_HAS_XSIMD` | Defined when `TMECH_USE_XSIMD=ON`. Enables xsimd-accelerated GEMM kernels and delegates SIMD alignment queries to xsimd. |

## Quick start

### Tensor addition

```cpp
#include <iostream>
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> A{1, 2, 3,
                                   4, 5, 6,
                                   7, 8, 9};
    tmech::tensor<double, 3, 2> B{9, 8, 7,
                                   6, 5, 4,
                                   3, 2, 1};

    tmech::tensor<double, 3, 2> C = A + B;
    std::cout << C << std::endl;
}
```

Output:
```
10 10 10
10 10 10
10 10 10
```

### Outer products

tmech provides three outer product variants that differ in how
indices are arranged in the resulting 4th-order tensor:

```cpp
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> a, b;
    tmech::tensor<double, 3, 4> A;
    a.randn();
    b.randn();

    A = tmech::otimes(a, b);   // C_ijkl = a_ij * b_kl
    A = tmech::otimesu(a, b);  // C_ikjl = a_ij * b_kl
    A = tmech::otimesl(a, b);  // C_iljk = a_ij * b_kl

    // general outer product with custom index mapping
    A = tmech::outer_product<tmech::sequence<2,3>, tmech::sequence<1,4>>(a, b);
}
```

### Contractions

```cpp
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> a, b, c;
    tmech::tensor<double, 3, 4> A, B, C;
    a.randn(); b.randn(); A.randn(); B.randn();

    // single contraction (matrix multiply): c_ij = a_ik * b_kj
    c = a * b;

    // double contraction: scalar = a_ij * b_ij
    double val = tmech::dcontract(a, b);

    // double contraction: 4th-order with 2nd-order -> 2nd-order
    c = tmech::dcontract(A, a);

    // double contraction: 4th-order with 4th-order -> 4th-order
    C = tmech::dcontract(A, B);

    // general inner product with explicit index sequences
    using SeqL = tmech::sequence<3, 4>;
    using SeqR = tmech::sequence<1, 2>;
    C = tmech::inner_product<SeqL, SeqR>(A, B);

    // use tmech::eval() to handle aliasing (when result appears in the expression)
    A = tmech::eval(tmech::dcontract(B, A));
}
```

### Inverse

```cpp
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> a, a_inv;
    tmech::tensor<double, 3, 4> A, A_inv;
    a.randn(); A.randn();

    // 2nd-order inverse (direct formula for 2x2 and 3x3)
    a_inv = tmech::inv(a);

    // 4th-order inverse (minor symmetry assumed at index pairs 1,2 and 3,4)
    A_inv = tmech::inv(A);

    // full 4th-order inverse (no symmetry assumed)
    A_inv = tmech::invf(A, tmech::sequence<1, 2, 3, 4>());
}
```

### Polar decomposition

```cpp
#include <iostream>
#include <tmech/tmech.h>

int main() {
    tmech::tensor<double, 3, 2> F;
    F.randn();

    // F = R * U = V * R
    auto polar = tmech::polar_decomposition(F);

    auto R = polar.R();  // rotation tensor
    auto U = polar.U();  // right stretch tensor
    auto V = polar.V();  // left stretch tensor

    std::cout << std::boolalpha
              << tmech::almost_equal(F, R * U, 5e-7) << std::endl;  // true
}
```

### Symbolic differentiation

```cpp
#include <iostream>
#include <tmech/tmech.h>

int main() {
    using tensor2 = tmech::tensor<double, 3, 2>;
    using tensor4 = tmech::tensor<double, 3, 4>;
    tensor2 eps = tmech::sym(tmech::randn<double, 3, 2>());

    symdiff::variable<tensor2, 0> _eps;
    symdiff::constant<double, 0> _lambda(150, "lambda");
    symdiff::constant<double, 1> _mu(200, "mu");
    symdiff::real<double, 2, 0, 1> _2;
    symdiff::constant<tensor2, 2> _I("I");
    _I = tmech::eye<double, 3, 2>();

    // define stress function: sig = lambda * tr(eps) * I + 2 * mu * eps
    auto _sig = _lambda * tmech::trace(tmech::as_sym(_eps)) * _I
              + _2 * _mu * tmech::as_sym(_eps);

    // compile-time derivative -> tangent modulus
    auto _C = symdiff::derivative(_sig, _eps);
    std::cout << _C << std::endl;

    // evaluate at a specific strain state
    tensor4 C = _C(std::make_tuple(eps));
    tensor2 sig = _sig(std::make_tuple(eps));
}
```

Output:
```
(otimes((lambda*I), I)+((2*mu)*(0.5*(otimesu(I, I)+otimesl(I, I)))))
```

## Abaqus adaptor

tmech can wrap raw C/Fortran arrays from FEM solvers like Abaqus, enabling
tensor operations on pre-allocated memory with automatic Voigt notation handling.
This is useful for writing UMAT (implicit) and VUMAT (explicit) subroutines:

```cpp
#include <tmech/tmech.h>

int main() {
    // arrays as they would come from Abaqus
    double ptrC[36], ptrSig[6];
    double ptrEps[6]{1, 1, 1, 1, 1, 0.5};

    const double K{12000}, G{200};
    tmech::eye<double, 3, 2> I;
    const auto IIsym = (tmech::otimesu(I, I) + tmech::otimesl(I, I)) * 0.5;
    const auto IIvol = tmech::otimes(I, I) / 3.0;
    const auto IIdev = IIsym - IIvol;

    // wrap raw arrays -- adaptor handles Abaqus Voigt layout
    tmech::adaptor<double, 3, 4, tmech::abq_exp<3>> C(ptrC);
    tmech::adaptor<double, 3, 2, tmech::abq_exp<3>> sig(ptrSig);
    tmech::adaptor<double, 3, 2, tmech::abq_exp<3, true>> eps(ptrEps);

    // compute stiffness and stress using standard tensor notation
    C = 3 * K * IIvol + 2 * G * IIdev;
    sig = tmech::dcontract(C, eps);
}
```

See `examples/adaptor/abaqus/` for complete UMAT/VUMAT examples.

## Benchmarks

The `benchmark/` directory contains [Google Benchmark](https://github.com/google/benchmark) tests comparing tmech against Eigen, Fastor, and hand-written implementations across tensor operations (contraction, outer products, addition, inverse, etc.).

- [Library comparison report](benchmark_results/benchmark_report.md) -- scalar vs xsimd, tmech vs Eigen vs Fastor (generated by the scripts below)
- [Tree-node unroll analysis](benchmark_tree_nodes/REPORT.md) -- effect of `TMECH_MAX_TREE_NODES_UNROLL` on performance

### Automated run

Build both scalar and xsimd configurations, run all benchmarks, and generate a report:

```bash
python3 scripts/run_benchmarks.py
python3 scripts/run_bench_report.py --input-dir benchmark_results --output-dir benchmark_results
```

This produces:
1. `benchmark_results/scalar.json` and `benchmark_results/xsimd.json` -- raw results
2. `benchmark_results/benchmark_report.md` -- comparison tables and bar chart plots

### Manual single-configuration run

```bash
cmake -B build -S . -DTMECH_BUILD_BENCHMARK=ON -DTMECH_USE_XSIMD=OFF
cmake --build build --target tmech_benchmark -j$(nproc)
./build/benchmark/tmech_benchmark --benchmark_repetitions=5 --benchmark_report_aggregates_only=true
```

Set `-DTMECH_USE_XSIMD=ON` to enable xsimd-accelerated GEMM kernels.

### Script options

| Flag | Script | Description |
|---|---|---|
| `--repetitions N` | `run_benchmarks.py` | Number of repetitions per benchmark (default: 5) |
| `--benchmark-filter REGEX` | `run_benchmarks.py` | Run only benchmarks matching the regex |
| `--no-plots` | `run_bench_report.py` | Skip plot generation (useful without matplotlib) |
| `--baseline-json-dir DIR` | `run_bench_report.py` | Compare against a baseline for regression detection |

Run `python3 scripts/run_benchmarks.py --help` or `python3 scripts/run_bench_report.py --help` for all options.

## Documentation

Full documentation is available at http://tmech.readthedocs.io/

## Contributing

We welcome contributions to improve performance or add new functionality.
For new features, please first open an [issue](https://github.com/petlenz/tmech/issues)
to discuss the approach.

1. [Fork](https://docs.github.com/en/get-started/quickstart/fork-a-repo) the repository
2. Create a [branch](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-and-deleting-branches-within-your-repository) with a descriptive name
3. Implement your changes and add [tests](https://github.com/petlenz/tmech/blob/master/tests/tmech_test.h)
4. Open a [pull request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork) into `main`

## License

BSD 3-Clause. See [LICENSE](LICENSE) for details.

## Issues

Bug reports and feature requests: [GitHub Issues](https://github.com/petlenz/tmech/issues)
