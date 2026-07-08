.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.
   
Changelog
=========

The authoritative, up-to-date changelog is maintained in
`CHANGELOG.md <https://github.com/petlenz/tmech/blob/master/CHANGELOG.md>`_
at the repository root. A summary of releases follows.

1.1.0
-----
- Portable SIMD-accelerated GEMM kernels with an optional xsimd backend.
- User-tunable unroll thresholds (``TMECH_MAX_TREE_NODES_UNROLL``,
  ``TMECH_MAX_LOOP_UNROLL_SIZE``).
- Modernized CMake package (``tmech::tmech`` target, ``find_package`` support).
- Multi-platform CI, benchmark suite, and community/contribution files.
- Documentation corrected to state the BSD 3-Clause license consistently.

1.0.0
-----
- Initial public release: expression-template tensor algebra, tensor products
  and contractions, inverse/determinant/trace/exponential, polar/eigenvalue/
  spectral decompositions, isotropic tensor functions, the ``symdiff``
  compile-time differentiation module, and Abaqus adaptors.

