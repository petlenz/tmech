// Config: no xsimd, no loop unrolling (scalar baseline)
#ifdef TMECH_HAS_XSIMD
#undef TMECH_HAS_XSIMD
#endif
#define TMECH_MAX_TREE_NODES_UNROLL 0
#define TMECH_MAX_LOOP_UNROLL_SIZE 0

#include <tmech/tmech.h>
#include <benchmark/benchmark.h>

#define SUFFIX scalar
#include "bench_kernels.h"
#undef SUFFIX
