/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TMECH_CONFIG_H
#define TMECH_CONFIG_H

#define TMECH_VERSION_MAJOR 1
#define TMECH_VERSION_MINOR 0
#define TMECH_VERSION_PATCH 0

//Rank 8 Dim 3
#define TMECH_MAX_STATIC_SIZE 6561ul

// SIMD register width (bytes) for the current target architecture.
// When xsimd is available, its architecture detection is authoritative
// (xsimd.hpp is included in tmech.h before this header).
// Otherwise, fall back to predefined compiler macros.
#ifndef TMECH_HAS_XSIMD
#if defined(__AVX512F__)
#define TMECH_SIMD_REGISTER_WIDTH 64
#elif defined(__AVX__) || defined(__AVX2__)
#define TMECH_SIMD_REGISTER_WIDTH 32
#elif defined(__SSE__) || defined(__SSE2__) || defined(__ARM_NEON) || defined(__ARM_NEON__)
#define TMECH_SIMD_REGISTER_WIDTH 16
#else
#define TMECH_SIMD_REGISTER_WIDTH 0
#endif
#endif

namespace detail {

// Optimal data alignment for type T on the current architecture.
// Ensures SIMD-width loads/stores are naturally aligned regardless
// of element type (float, double, int, std::complex<>, ...).
template <typename T>
struct simd_alignment {
#ifdef TMECH_HAS_XSIMD
    static constexpr std::size_t value =
        (xsimd::default_arch::alignment() > alignof(T))
            ? xsimd::default_arch::alignment()
            : alignof(T);
#else
    static constexpr std::size_t value =
        (TMECH_SIMD_REGISTER_WIDTH > alignof(T))
            ? TMECH_SIMD_REGISTER_WIDTH
            : alignof(T);
#endif
};

template <typename T>
inline constexpr std::size_t simd_alignment_v = simd_alignment<T>::value;

#ifdef TMECH_HAS_XSIMD
template <typename T, typename = void>
struct has_xsimd_batch : std::false_type {};

template <typename T>
struct has_xsimd_batch<T, std::void_t<decltype(xsimd::batch<T>::size)>> : std::true_type {};

template <typename T>
inline constexpr bool has_xsimd_batch_v = has_xsimd_batch<T>::value;
#endif

} // namespace detail

#endif // TMECH_CONFIG_H
