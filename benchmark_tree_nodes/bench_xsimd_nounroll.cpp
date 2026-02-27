// Config: xsimd enabled, no loop unrolling
#ifndef TMECH_HAS_XSIMD
#define TMECH_HAS_XSIMD
#endif
#define TMECH_MAX_TREE_NODES_UNROLL 0
#define TMECH_MAX_LOOP_UNROLL_SIZE 0

#include <tmech/tmech.h>
#include <benchmark/benchmark.h>

#define SUFFIX xsimd
#include "bench_kernels.h"
#undef SUFFIX
