cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ── Google Benchmark ──────────────────────────────────────────────────────────
FetchContent_Declare(
    benchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.1.tar.gz
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

# ── Single-binary benchmark with 3 configs ────────────────────────────────────
# Each .cpp file #defines its own macros before including <tmech/tmech.h>.
# Anonymous namespaces keep template instantiations per-TU.
add_executable(bench_tree_nodes
    main.cpp
    bench_scalar.cpp
    bench_xsimd_nounroll.cpp
    bench_xsimd_unroll.cpp
)

# Include paths only — do NOT link tmech target (it propagates TMECH_HAS_XSIMD).
# Each .cpp file controls its own TMECH_HAS_XSIMD via #define/#undef.
target_include_directories(bench_tree_nodes PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# xsimd include path (needed by the xsimd config .cpp files)
if(TARGET xsimd)
    get_target_property(_xsimd_inc xsimd INTERFACE_INCLUDE_DIRECTORIES)
    if(_xsimd_inc)
        target_include_directories(bench_tree_nodes PRIVATE ${_xsimd_inc})
    endif()
elseif(xsimd_SOURCE_DIR)
    target_include_directories(bench_tree_nodes PRIVATE ${xsimd_SOURCE_DIR}/include)
endif()

target_link_libraries(bench_tree_nodes PRIVATE benchmark::benchmark)

target_compile_options(bench_tree_nodes PRIVATE
    -O3 -march=native -DNDEBUG -funroll-loops
)
