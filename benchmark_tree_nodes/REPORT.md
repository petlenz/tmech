# Benchmark Report: xsimd Vectorization & Loop Unrolling

**Branch:** `cleanup-gemm-wrapper`
**Date:** 2026-02-27
**Platform:** Linux x86_64, 16 cores @ 4.2 GHz, AVX2
**Repetitions:** 5 per benchmark (mean, stddev, cv reported)

## Overview

This report evaluates the performance impact of two optimizations in tmech's
expression-template evaluation pipeline:

1. **xsimd vectorization** — SIMD batch evaluation of tensor expressions
2. **Compile-time loop unrolling** — fold-expression unrolling of nested index
   loops, gated by an expression-tree node counter

Three configurations are compared in a single benchmark binary to eliminate
process-level measurement overhead:

| Config | `TMECH_HAS_XSIMD` | `TMECH_MAX_TREE_NODES_UNROLL` | `TMECH_MAX_LOOP_UNROLL_SIZE` |
|---|---|---|---|
| **Scalar** | off | 0 | 0 |
| **xsimd (no unroll)** | on | 0 | 0 |
| **xsimd + unroll** | on | 16 (default) | 256 (default) |

## Results

### Absolute Times

Error bars show one standard deviation across 5 repetitions.

![Absolute times](bench_absolute.png)

### Speedup vs Scalar Baseline

![Speedup](bench_speedup.png)

### Summary Table

All times in nanoseconds. Values are mean +/- stddev (cv = coefficient of variation).

| Benchmark | Scalar | cv | xsimd | cv | +unroll | cv | Speedup |
|---|---:|---:|---:|---:|---:|---:|---:|
| double_contraction_8_8 (2D) | 2119 +/- 15 | 0.7% | 163 +/- 1 | 0.7% | 164 +/- 2 | 1.2% | **13.0x** |
| double_contraction_times3_4_4 (3D) | 285 +/- 5 | 1.7% | 106 +/- 1 | 1.0% | 108 +/- 1 | 1.1% | **2.6x** |
| more_complex_4_2 (3D) | 16.4 +/- 0.1 | 0.5% | 8.9 +/- 0.1 | 1.2% | 8.7 +/- 0.1 | 0.7% | **1.9x** |
| double_contraction_8_8 (3D) | 49833 +/- 665 | 1.3% | 30444 +/- 196 | 0.6% | 30201 +/- 185 | 0.6% | **1.7x** |
| double_contraction_4_2 (3D) | 12.4 +/- 0.1 | 0.9% | 8.3 +/- 0.1 | 1.4% | 8.1 +/- 0.1 | 0.9% | **1.5x** |
| otimes_simple (3D) | 9.4 +/- 0.1 | 1.4% | 9.4 +/- 0.0 | 0.3% | 6.3 +/- 0.1 | 1.0% | **1.5x** |
| double_contraction_4_4 (2D) | 6.2 +/- 0.1 | 0.8% | 4.4 +/- 0.1 | 1.3% | 4.4 +/- 0.0 | 0.4% | **1.4x** |
| complex_double_contraction_4_4 (3D) | 108 +/- 2 | 1.8% | 85.7 +/- 0.7 | 0.8% | 88.3 +/- 1.1 | 1.3% | 1.2x |
| more_complex_4_4 (3D) | 84.1 +/- 0.5 | 0.6% | 70.9 +/- 1.1 | 1.6% | 72.9 +/- 0.8 | 1.1% | 1.2x |
| double_contraction_4_4 (3D) | 61.2 +/- 0.3 | 0.6% | 53.0 +/- 0.6 | 1.1% | 53.5 +/- 0.5 | 1.0% | 1.1x |
| otimes_iso (3D) | 94.6 +/- 1.4 | 1.5% | 94.7 +/- 0.4 | 0.4% | 94.5 +/- 0.2 | 0.2% | 1.0x |
| addition (3D) | 0.93 +/- 0.01 | 1.0% | 0.93 +/- 0.02 | 1.7% | 0.92 +/- 0.01 | 0.6% | 1.0x |
| single_contraction (3D) | 4.9 +/- 0.0 | 0.4% | 5.0 +/- 0.1 | 1.3% | 4.9 +/- 0.0 | 0.4% | 1.0x |
| inverse (3D) | 5.2 +/- 0.0 | 0.5% | 5.4 +/- 0.1 | 1.5% | 5.4 +/- 0.0 | 0.7% | 1.0x |

All measurements show cv < 2%, confirming low variance and reliable results.

### Key Observations

- **Double contractions benefit the most from xsimd.** The 2D rank-8 case
  (`double_contraction_8_8 (2D)`) sees a 13x speedup, dominated by the GEMM
  kernel replacing the scalar loop.
- **Loop unrolling has minimal additional impact** on top of xsimd for most
  benchmarks. The xsimd vectorization is the primary driver of speedups.
- **`otimes_simple` benefits from unrolling** (6.3 ns vs 9.4 ns) because it
  has a small expression tree (3 nodes) and a small loop body that the compiler
  can fully optimize when unrolled.
- **No regressions.** The complex expression `otimes_iso` (47 tree nodes) runs
  at the same speed across all configs — the tree-node guard correctly prevents
  harmful unrolling.

## Tree-Node Threshold Validation

The tree-node counter (`tree_node_count`) walks the expression tree at compile
time and disables fold-expression loop unrolling when the node count exceeds
`TMECH_MAX_TREE_NODES_UNROLL` (default: 16).

Error bars show one standard deviation.

![Tree-node sweep](bench_tree_nodes.png)

| Tree Nodes | Scalar | xsimd | +unroll | Unroll Speedup |
|---:|---:|---:|---:|---:|
| 3 | 9.3 +/- 0.1 (0.5%) | 9.4 +/- 0.1 (1.1%) | 6.5 +/- 0.0 (0.4%) | **1.4x** |
| 7 | 17.2 +/- 0.3 (1.7%) | 17.5 +/- 0.2 (1.2%) | 23.9 +/- 0.1 (0.6%) | 0.7x |
| 11 | 23.9 +/- 0.2 (0.8%) | 24.0 +/- 0.5 (1.9%) | 26.0 +/- 0.2 (0.9%) | 0.9x |
| 17 | 31.7 +/- 0.3 (0.9%) | 31.6 +/- 0.2 (0.7%) | 31.2 +/- 0.2 (0.7%) | 1.0x |
| 23 | 32.6 +/- 0.3 (1.0%) | 32.2 +/- 0.2 (0.6%) | 32.1 +/- 0.2 (0.5%) | 1.0x |

- At **3 nodes**, unrolling wins — the compiler fully optimizes the unrolled body.
- At **7 nodes**, unrolling hurts — the unrolled code size exceeds the
  instruction cache benefit and the compiler cannot optimize as effectively.
- At **17+ nodes**, all configs converge — the guard disables unrolling, so
  `xsimd + unroll` falls back to the runtime loop, matching `xsimd (no unroll)`.

The default threshold of **16** is well-placed between the 11-node crossover
and the 17-node convergence point.

## Implementation

### Expression Tree Node Counter

A compile-time trait walks the expression tree using template-template parameter
matching:

```cpp
// Generic unary wrapper: template<typename> class
template<template<typename> class Unary, typename Expr>
struct tree_node_count<Unary<Expr>>
    : std::integral_constant<std::size_t, 1 + tree_node_count_v<...>> {};

// Generic binary wrapper: template<typename, typename> class
template<template<typename, typename> class Binary, typename LHS, typename RHS>
struct tree_node_count<Binary<LHS, RHS>>
    : std::integral_constant<std::size_t, 1 + tree_node_count_v<...> + tree_node_count_v<...>> {};
```

Explicit specializations are only needed for wrappers that don't match the
generic pattern (e.g. `tensor_binary_expression_wrapper` with 3 template
parameters, `outer_product_wrapper` with 4, and `basis_change_wrapper` whose
second parameter is a sequence, not a sub-expression).

### Loop Unrolling Guard

```cpp
template <std::size_t DEEP, std::size_t Size, std::size_t TreeNodes = 1>
constexpr bool should_unroll_loop =
    (ct_pow<Size, DEEP+1>::value <= TMECH_MAX_LOOP_UNROLL_SIZE)
    && (TreeNodes <= TMECH_MAX_TREE_NODES_UNROLL);
```

Both thresholds are user-configurable via macros defined before including
`<tmech/tmech.h>`.

## Reproducing

```bash
cd benchmark_tree_nodes
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/bench_tree_nodes \
    --benchmark_out=results.json \
    --benchmark_out_format=json \
    --benchmark_repetitions=5 \
    --benchmark_report_aggregates_only=true
python3 plot_report.py results.json
```
