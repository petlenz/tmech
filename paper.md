---
title: 'tmech: A C++ header-only library for compile-time tensor algebra with symbolic differentiation'
tags:
  - C++
  - tensor algebra
  - continuum mechanics
  - expression templates
  - symbolic differentiation
  - finite element method
authors:
  - name: Peter Lenz
    orcid: 0000-0001-8041-9538
    affiliation: 1
affiliations:
  - name: Continental Reifen Deutschland GmbH
    index: 1
date: 2 March 2026
bibliography: paper.bib
---

# Summary

`tmech` is a C++17 header-only library for tensor algebra designed for
computational continuum mechanics. It provides a high-level syntax for
tensor operations of arbitrary rank, an expression template engine with
lazy evaluation that composes operations without intermediate temporaries,
and an experimental compile-time symbolic differentiation module —
uncommon among tensor libraries — that differentiates a broad class of
tensor expressions to produce analytic tangent operators. The library also
includes spectral-based isotropic tensor functions (exponential, logarithm,
square root, power) with closed-form derivative tensors, tensor
decompositions (polar and eigenvalue), and positive/negative projections,
all with derivative support. Tensors of
rank eight or below use stack-allocated static storage for maximum
performance, while higher-rank tensors seamlessly fall back to dynamic
allocation. `tmech` targets applications in nonlinear solid and fluid
mechanics where consistent linearization of constitutive models is
essential for Newton-type solvers in finite element codes.

# Statement of need

In computational continuum mechanics, constitutive models relate stress
to deformation through tensor-valued functions. Newton-type solution
procedures for the resulting nonlinear boundary value problems require
consistent tangent operators — fourth-order tensors obtained by
differentiating the stress response with respect to the deformation
measure [@holzapfel2000; @bonetwood2008]. Deriving and implementing these
tangent operators by hand is tedious, error-prone, and a major bottleneck
when developing new material models.

Several C++ tensor libraries exist. Eigen [@eigen] is a widely adopted
linear algebra library but operates primarily on matrices and vectors
rather than higher-order tensors, and provides no symbolic
differentiation. FTensor [@ftensor] supports higher-order tensor index
notation with compile-time index contraction but does not offer
differentiation capabilities. Fastor [@fastor] provides SIMD-accelerated
tensor contractions with expression templates but likewise lacks symbolic
differentiation of tensor expressions.

The differentiation problem itself is addressed by a separate class of
tools. General-purpose automatic differentiation (AD) libraries such as
Sacado [@trilinos], CoDiPack [@codipack], autodiff [@autodiff], and
Enzyme [@enzyme] compute derivatives of C++ code through operator
overloading or compiler-level transformation, typically producing
numerical derivative *values* at run time via a computational tape or dual
numbers. Symbolic code generators such as AceGen [@korelc2002] instead
derive closed-form expressions and emit optimized source, but rely on an
external computer-algebra environment. `tmech` occupies a different point
in this space: its (experimental) `symdiff` module performs symbolic
differentiation of tensor expressions entirely at compile time within
C++, yielding closed-form derivative tensors expressed in the same
tensor-algebra types, without a run-time tape or an external toolchain. It
is specialized to the tensor operations of continuum mechanics rather than
arbitrary C++ code.

`tmech` combines a full-featured tensor algebra engine with this
compile-time symbolic differentiation module. Users can define tensor
expressions using natural mathematical notation and then call a single
differentiation function to obtain exact derivative tensors — avoiding
manual derivation. Because differentiation happens at compile time through
template metaprogramming, the resulting code avoids run-time
interpretation and tape overhead. This capability is
particularly valuable for finite element codes implementing
hyperelastic, viscoelastic, and elastoplastic material models
[@simo1992], where the tangent operator must be updated at every
integration point and every Newton iteration. The library has been
used in multiscale simulations of composite materials, including
non-local damage modelling with mean-field homogenization [@lenz2023]
and thermo-chemo-mechanical curing simulations at large strains
[@lenz2024]. It is also integrated with the MOOSE finite element
framework [@moose] in an industrial setting for high-performance
material point updates. In addition, its tensor-algebra engine is used
as a dependency of the `numsim-cas` [@numsim_cas] and
`numsim-codegen` [@numsim_codegen] tools.

# Key features

## Expression templates with lazy evaluation

`tmech` uses the curiously recurring template pattern (CRTP)
[@coplien1995] to implement expression templates [@veldhuizen1995] that
compose tensor operations without creating intermediate temporaries. Over
50 wrapper
classes represent operations such as contractions, tensor products,
inverse, transpose, deviatoric and volumetric projections, basis changes,
and cofactor computation. Expressions are evaluated lazily: the full
computation tree is assembled at compile time and evaluated only when the
result is assigned to a tensor object, enabling the compiler to optimize
across the entire expression.

## Compile-time symbolic differentiation

The `symdiff` module (around 70 header files) extends the expression
template concept to symbolic algebra. It is an experimental component of
the library: the tensor-algebra engine is production-tested (see below),
whereas `symdiff` is under active development and its API may change.
Users declare symbolic tensor variables, build expressions from them, and
invoke differentiation to obtain exact derivative expressions.
Higher-order derivatives are obtained by repeated application. The module
includes symbolic representations of scalar and tensorial constants,
binary operations, standard mathematical functions, and tensor-specific
operations (contractions, outer products, basis changes), together with
Newton--Raphson solvers built on the symbolic Jacobian.

As a minimal example, differentiating a scalar-valued function of a
tensor variable (the determinant, whose derivative is the cofactor):

```cpp
using T2 = tmech::tensor<double,3,2>;
symdiff::variable<T2, 0> x;

auto f  = tmech::det(x);              // scalar-valued function of a tensor
auto df = symdiff::derivative(f, x);  // closed-form derivative: cof(x)
```

A more representative use case is the automatic derivation of the
tangent stiffness tensor for a linear elastic constitutive model:

```cpp
using T2 = tmech::tensor<double,3,2>;
symdiff::variable<T2, 0> eps;
symdiff::constant<double, 0> lambda(150, "lambda");
symdiff::constant<double, 1> mu(200, "mu");
symdiff::real<double, 2, 0, 1> two;   // symbolic scalar 2
symdiff::constant<T2, 2> I("I");
I = tmech::eye<double,3,2>();

// Stress: sigma = lambda tr(eps) I + 2 mu eps
auto sig = lambda * tmech::trace(tmech::as_sym(eps)) * I
         + two * mu * tmech::as_sym(eps);

// Tangent: C = d(sigma)/d(eps) — derived automatically
auto C = symdiff::derivative(sig, eps);
```

Both examples are compiled as part of the test/example suite (see
`examples/symdiff/`).

## Isotropic tensor functions with analytic derivatives

`tmech` implements isotropic tensor functions — `exp`, `log`, `sqrt`,
`pow`, and positive/negative part operators — via spectral decomposition
[@miehe1993; @miehe2001]. Given a symmetric rank-2 tensor, the library
computes eigenvalues and eigenvectors, applies the scalar function to
each eigenvalue, and reconstructs the tensor result. First and second
derivatives are computed analytically using the eigenbasis, with proper
treatment of repeated eigenvalues to avoid singularities. These functions
integrate seamlessly with the symbolic differentiation module, enabling
automatic tangent computation for models involving tensor logarithms or
exponentials.

## Tensor decompositions with derivative tensors

The library provides polar decomposition ($\mathbf{F} = \mathbf{R}\mathbf{U} = \mathbf{V}\mathbf{R}$)
via Newton iteration or eigenvalue-based methods, and eigenvalue
decomposition with support for 2D and 3D tensors. Both decompositions
expose not only the decomposition factors but also their derivative
tensors ($\partial \mathbf{R}/\partial \mathbf{F}$,
$\partial \mathbf{U}/\partial \mathbf{F}$, etc.), which are required for
consistent linearization when constitutive models are formulated in terms
of stretch or rotation tensors.

# Architecture

`tmech` is a header-only library requiring only a C++17-conformant
compiler (GCC 7+, Clang 5+, MSVC). No external dependencies are needed for
core functionality.

The codebase is organized using a *bones/meat* separation pattern: each
class has a `*_bones.h` header declaring its interface and a companion
`*_meat.h` header containing the implementation. This separation improves
readability and allows selective inclusion of implementation details.

All tensor types and expression wrappers inherit from a common CRTP base
class `tensor_base<Derived>`, providing a uniform interface and enabling
static polymorphism without virtual function overhead. The tensor class
template `tensor<T, Dim, Rank>` supports arbitrary scalar types
(including `float`, `double`, and `std::complex`), dimensions, and ranks.
Tensors with up to 6561 elements (rank $\leq$ 8 in three dimensions) are
backed by `std::array` for stack allocation and cache locality; larger
tensors automatically use `std::vector` with lazy heap allocation,
allowing the library to handle arbitrarily high ranks without
source-level changes.

GEMM kernels are specialized at compile time based on operand dimensions,
with dedicated paths for vector-matrix, matrix-vector, and
matrix-matrix products. Optional SIMD acceleration is available through
the xsimd library [@xsimd] for vectorized evaluation on x86 and ARM
platforms.

The library is tested with Google Test across GCC, Clang, and MSVC on
Linux, macOS, and Windows, and benchmarked against Eigen and Fastor
using Google Benchmark [@googlebenchmark].

# Comparison with related software

| Feature | tmech | Eigen | FTensor | Fastor |
|:--------|:-----:|:-----:|:-------:|:------:|
| Header-only | Yes | Yes | Yes | Yes |
| Arbitrary-rank tensors | Yes | Yes¹ | Yes | Yes |
| Expression templates | Yes | Yes | Yes | Yes |
| Symbolic differentiation | Yes² | No | No | No |
| Isotropic tensor functions | Yes | No | No | No |
| Decomposition derivatives | Yes | No | No | No |
| SIMD support | Optional | Yes | No | Yes |
| Index notation | Implicit | N/A | Yes | Yes |

¹ Eigen provides arbitrary-rank tensors only through its unsupported
`Eigen::Tensor` module. &nbsp; ² Experimental (see above).

The distinguishing feature of `tmech` is the integration of symbolic
differentiation with tensor algebra. All four libraries provide expression
templates for efficient evaluation; among them, only `tmech` also
differentiates tensor expressions symbolically to produce tangent
operators at compile time. General-purpose AD tools achieve differentiation
by other means (run-time tapes or code generation) as discussed in the
statement of need.

# Acknowledgements

The author thanks the open-source communities behind Eigen, xsimd, and
Google Benchmark for their foundational tools.

# References
