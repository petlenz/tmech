# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.1.1] - 2026-07-22

### Fixed
- `inner_product<SeqL, SeqR>` silently returned a zero-initialised result when
  one operand needed a basis change and the other was a lazy expression-template
  (no `raw_data()`), e.g. `inner_product<...>(F, 1.0*T)`. A non-exhaustive
  `if constexpr` dispatch skipped the contraction entirely; the guard is now the
  correct 2×2 over both operands. Added a lazy-operand regression test
  ([#28](https://github.com/petlenz/tmech/issues/28)).

## [1.1.0] - 2026-07-15

### Added
- Portable SIMD-accelerated GEMM kernels with an optional [xsimd](https://github.com/xtensor-stack/xsimd)
  backend (`TMECH_USE_XSIMD`), including small/medium/large kernel tiers and a
  scalar fallback for non-vectorizable types.
- `TMECH_MAX_TREE_NODES_UNROLL` and `TMECH_MAX_LOOP_UNROLL_SIZE` user-tunable
  unroll thresholds.
- Google Benchmark suite and reporting scripts comparing tmech against Eigen
  and Fastor.
- `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, issue/PR templates, and
  `CITATION.cff`.
- Read the Docs build configuration (`.readthedocs.yaml`).

### Changed
- Modernized the CMake package (imported target `tmech::tmech`,
  `find_package(tmech)` support, `TMECH_*` option names).
- Multi-platform CI matrix (Linux/macOS/Windows × GCC/Clang/MSVC ×
  Debug/Release × xsimd on/off).
- Documentation now consistently states the BSD 3-Clause license.

### Fixed
- Uninitialized SIMD accumulator in the GEMM kernel.
- Removed a debug print and other leftovers from library code paths.
- `levi_civita<T, Dim>::operator()(i, j, k)` returned the negated permutation
  symbol (e.g. `e(0,1,2)` gave `-1` instead of `+1`); the rank-3 Levi-Civita
  symbol is now mathematically correct, with a direct symbol-value test
  ([#13](https://github.com/petlenz/tmech/issues/13)).

## [1.0.0] - 2022-09-23

### Added
- Initial public release: header-only higher-order tensor algebra with an
  expression-template / CRTP engine, tensor products (`otimes`, `otimesu`,
  `otimesl`), single/double contraction, inverse, determinant, trace,
  exponential, polar/eigenvalue/spectral decompositions, isotropic tensor
  functions, a compile-time symbolic differentiation module (`symdiff`), and
  Abaqus UMAT/VUMAT adaptors.

[Unreleased]: https://github.com/petlenz/tmech/compare/v1.1.1...HEAD
[1.1.1]: https://github.com/petlenz/tmech/compare/v1.1.0...v1.1.1
[1.1.0]: https://github.com/petlenz/tmech/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/petlenz/tmech/releases/tag/v1.0.0
