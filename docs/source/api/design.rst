.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Design and architecture
=======================

``tmech`` is a header-only library requiring only a C++17-conformant compiler
(GCC 7+, Clang 5+, MSVC). No external dependencies are needed for core
functionality.

Expression templates with lazy evaluation
-----------------------------------------

``tmech`` uses the curiously recurring template pattern (CRTP) to implement
expression templates that compose tensor operations without creating
intermediate temporaries (see also :ref:`lazy-evaluation`). Over 50 wrapper
classes represent operations such as contractions, tensor products, inverse,
transpose, deviatoric and volumetric projections, basis changes, and cofactor
computation. Expressions are evaluated lazily: the full computation tree is
assembled at compile time and evaluated only when the result is assigned to a
tensor object, enabling the compiler to optimize across the entire expression.

All tensor types and expression wrappers inherit from a common CRTP base class
``tensor_base<Derived>``, which provides a uniform interface and enables static
polymorphism without virtual-function overhead.

Storage model
-------------

The tensor class template ``tensor<T, Dim, Rank>`` supports arbitrary scalar
types (including ``float``, ``double``, and ``std::complex``), dimensions, and
ranks. Tensors with up to 6561 elements (rank :math:`\leq` 8 in three
dimensions) are backed by ``std::array`` for stack allocation and cache
locality; larger tensors automatically use ``std::vector`` with lazy heap
allocation, allowing the library to handle arbitrarily high ranks without
source-level changes.

Header layout (bones/meat)
--------------------------

The codebase is organized using a *bones/meat* separation pattern: each class
has a ``*_bones.h`` header declaring its interface and a companion ``*_meat.h``
header containing the implementation. This separation improves readability and
allows selective inclusion of implementation details.

Contraction kernels and SIMD
----------------------------

GEMM kernels are specialized at compile time based on operand dimensions, with
dedicated paths for vector--matrix, matrix--vector, and matrix--matrix products.
Optional SIMD acceleration is available through the `xsimd
<https://github.com/xtensor-stack/xsimd>`_ library for vectorized evaluation on
x86 and ARM platforms.

Testing
-------

The library is tested with Google Test across GCC, Clang, and MSVC on Linux,
macOS, and Windows, and benchmarked against Eigen and Fastor using Google
Benchmark.
