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

# ── Eigen3 (prefer system, fallback to FetchContent) ─────────────────────────
find_package(Eigen3 3.4 QUIET)
if(NOT Eigen3_FOUND)
    FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG        3.4.0
        GIT_SHALLOW    TRUE
    )
    set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(eigen)
endif()

# ── Fastor (header-only via FetchContent) ────────────────────────────────────
FetchContent_Declare(
    fastor
    GIT_REPOSITORY https://github.com/romeric/Fastor.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(fastor)
if(NOT fastor_POPULATED)
    FetchContent_Populate(fastor)
endif()

# ── Boost (prefer system, fallback to FetchContent) ──────────────────────────
find_package(Boost QUIET)
if(NOT Boost_FOUND)
    FetchContent_Declare(
        boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-cmake.tar.gz
        EXCLUDE_FROM_ALL
    )
    set(BOOST_INCLUDE_LIBRARIES align)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_MakeAvailable(boost)
endif()


# ── Benchmark executable ─────────────────────────────────────────────────────
add_executable(tmech_benchmark main.cpp)

target_link_libraries(tmech_benchmark
    PRIVATE
        tmech
        benchmark::benchmark
        Eigen3::Eigen
)

if(Boost_FOUND)
    target_link_libraries(tmech_benchmark PRIVATE Boost::boost)
else()
    target_link_libraries(tmech_benchmark PRIVATE Boost::align)
endif()

target_include_directories(tmech_benchmark
    PRIVATE
        ${fastor_SOURCE_DIR}
)

target_compile_options(tmech_benchmark
    PRIVATE
        -O3 -march=native -DNDEBUG -funroll-loops
)
