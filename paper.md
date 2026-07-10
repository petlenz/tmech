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

`tmech` is a C++17 header-only library for higher-order tensor algebra aimed at
computational continuum mechanics. It provides a high-level, coordinate-free
syntax for tensors of arbitrary rank and an expression-template engine that
composes operations without intermediate temporaries. Beyond standard tensor
algebra, it offers spectral-based isotropic tensor functions (`exp`, `log`,
`sqrt`, `pow`) and tensor decompositions (polar, eigenvalue) with closed-form
derivatives, and an experimental compile-time *symbolic differentiation* module
that produces analytic tangent operators for tensor-valued expressions. Tensors
up to rank eight (in three dimensions) use stack-allocated storage; larger ranks
fall back to dynamic allocation, and optional SIMD acceleration is available
through xsimd [@xsimd]. The library targets nonlinear solid and fluid mechanics,
where consistent linearization of constitutive models is central to Newton-type
finite-element solvers.

# Statement of need

Newton-type solvers for nonlinear boundary-value problems in continuum mechanics
require *consistent tangent operators* — fourth-order tensors obtained by
differentiating the stress response with respect to a deformation measure
[@holzapfel2000; @bonetwood2008]. Deriving and implementing these by hand is
tedious, error-prone, and a recurring bottleneck when developing new material
models [@simo1992].

Existing C++ tensor libraries do not address this. Eigen [@eigen] targets
matrices and vectors rather than higher-order tensors, while FTensor [@ftensor]
and Fastor [@fastor] provide higher-order tensor contractions through expression
templates but offer no differentiation. General-purpose automatic
differentiation tools — Sacado [@trilinos], CoDiPack [@codipack], autodiff
[@autodiff], and Enzyme [@enzyme] — differentiate arbitrary C++ code, but
typically produce derivative *values* at run time through a tape or dual numbers
and are not specialized to tensor algebra; symbolic code generators such as
AceGen [@korelc2002] emit closed-form code but depend on an external
computer-algebra environment. `tmech` occupies a different point in this space:
its (experimental) `symdiff` module differentiates tensor expressions
symbolically, at compile time and entirely within C++, yielding closed-form
derivative tensors in the library's own types without a run-time tape or external
toolchain. Its tensor-algebra core is production-tested: it underlies the
author's multiscale composite simulations [@lenz2023; @lenz2024] and is a
dependency of the `numsim-cas` [@numsim_cas] and `numsim-codegen`
[@numsim_codegen] tools.

# Functionality

`tmech` uses expression templates with the curiously recurring template pattern
[@coplien1995; @veldhuizen1995]: tensor operations — contractions, products,
inverse, transpose, deviatoric/volumetric projections, and decompositions —
assemble a compile-time expression tree that is evaluated only on assignment,
avoiding temporaries. The `symdiff` module extends this to symbolic algebra, so
that a consistent tangent operator can be obtained directly from a stress
definition. For a linear-elastic model:

```cpp
using T2 = tmech::tensor<double,3,2>;
symdiff::variable<T2, 0> eps;
symdiff::constant<double, 0> lambda(150, "lambda");
symdiff::constant<double, 1> mu(200, "mu");
symdiff::real<double, 2, 0, 1> two;
symdiff::constant<T2, 2> I("I");  I = tmech::eye<double,3,2>();

auto sig = lambda * tmech::trace(tmech::as_sym(eps)) * I
         + two * mu * tmech::as_sym(eps);   // stress
auto C   = symdiff::derivative(sig, eps);   // tangent d(sigma)/d(eps)
```

Isotropic tensor functions and polar/eigenvalue decompositions additionally
expose analytic derivative tensors [@miehe1993; @miehe2001], required when
constitutive models are formulated in terms of stretch or rotation tensors. The
library is header-only, requires only a C++17 compiler (GCC 7+, Clang, MSVC) with
no mandatory dependencies, and is tested with Google Test across GCC, Clang, and
MSVC on Linux, macOS, and Windows. Further examples and API documentation are
provided in the repository.

# Acknowledgements

The author thanks the open-source communities behind Eigen, xsimd, and Google
Benchmark.

# References
