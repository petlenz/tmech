// Config: xsimd enabled, loop unrolling enabled (default thresholds)
#ifndef TMECH_HAS_XSIMD
#define TMECH_HAS_XSIMD
#endif

#include <tmech/tmech.h>
#include <benchmark/benchmark.h>

#define SUFFIX xsimd_unroll
#include "bench_kernels.h"
#undef SUFFIX
