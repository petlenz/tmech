// Common benchmark definitions — included once per configuration .cpp file.
// Expects SUFFIX to be #defined before inclusion (e.g. scalar, xsimd, xsimd_unroll).
// Expects <tmech/tmech.h> and <benchmark/benchmark.h> already included.

#ifndef SUFFIX
#error "Define SUFFIX before including bench_kernels.h"
#endif

#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)
#define BM(name) CAT(name, CAT(_, SUFFIX))

namespace {

template <typename T, std::size_t Dim>
static void BM(otimes_simple)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = tmech::otimes(A, B));
}

template <typename T, std::size_t Dim>
static void BM(otimesu)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = tmech::otimesu(A, B));
}

template <typename T, std::size_t Dim>
static void BM(otimesl)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = tmech::otimesl(A, B));
}

template <typename T, std::size_t Dim>
static void BM(otimes_iso)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> p1, p2, p3;
    tmech::tensor<T, Dim, 4> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();
    p1.randn(); p2.randn(); p3.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = a * tmech::otimes(p1, p1) +
            b * (tmech::otimes(p1, p2) + tmech::otimes(p2, p1)) +
            c * (tmech::otimes(p1, p3) + tmech::otimes(p3, p1)) +
            d * tmech::otimes(p2, p2) +
            e * (tmech::otimes(p2, p3) + tmech::otimes(p3, p2)) +
            f * tmech::otimes(p3, p3));
}

template <typename T, std::size_t Dim>
static void BM(addition)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A + B);
}

template <typename T, std::size_t Dim>
static void BM(addition_4)(benchmark::State &state) {
    tmech::tensor<T, Dim, 4> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A + B);
}

template <typename T, std::size_t Dim>
static void BM(addition_8)(benchmark::State &state) {
    tmech::tensor<T, Dim, 8> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A + B);
}

template <typename T, std::size_t Dim>
static void BM(single_contraction)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A * B);
}

template <typename T, std::size_t Dim>
static void BM(single_contraction_times3)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A * B * A);
}

template <typename T, std::size_t Dim>
static void BM(more_complex)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A * B + A + B);
}

template <typename T, std::size_t Dim>
static void BM(double_contraction_4_2)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> B, C;
    tmech::tensor<T, Dim, 4> A;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B));
}

template <typename T, std::size_t Dim>
static void BM(more_complex_4_2)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> B, C;
    tmech::tensor<T, Dim, 4> A;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B) + 2 * B);
}

template <typename T, std::size_t Dim>
static void BM(double_contraction_4_4)(benchmark::State &state) {
    tmech::tensor<T, Dim, 4> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B));
}

template <typename T, std::size_t Dim>
static void BM(double_contraction_times3_4_4)(benchmark::State &state) {
    tmech::tensor<T, Dim, 4> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::dcontract(A, tmech::dcontract(B, A)));
}

template <typename T, std::size_t Dim>
static void BM(complex_double_contraction_4_4)(benchmark::State &state) {
    using SeqL = tmech::sequence<1, 3>;
    using SeqR = tmech::sequence<2, 4>;
    tmech::tensor<T, Dim, 4> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::inner_product<SeqL, SeqR>(A, B));
}

template <typename T, std::size_t Dim>
static void BM(more_complex_4_4)(benchmark::State &state) {
    tmech::tensor<T, Dim, 4> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B) + 2 * B + B);
}

template <typename T, std::size_t Dim>
static void BM(double_contraction_8_8)(benchmark::State &state) {
    tmech::tensor<T, Dim, 8> A, B, C;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(C = tmech::ddcontract(A, B));
}

template <typename T, std::size_t Dim>
static void BM(inverse)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, A_inv;
    A_inv.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(A = tmech::inv(A_inv));
}

// Tree-node sweep benchmarks
template <typename T, std::size_t Dim>
static void BM(otimes_nodes_3)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = tmech::otimes(A, B));
}

template <typename T, std::size_t Dim>
static void BM(otimes_nodes_7)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = tmech::otimes(A, A) + tmech::otimes(B, B));
}

template <typename T, std::size_t Dim>
static void BM(otimes_nodes_11)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    const T a = rand(), b = rand();
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = a * tmech::otimes(A, A) + b * tmech::otimes(B, B));
}

template <typename T, std::size_t Dim>
static void BM(otimes_nodes_17)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    const T a = rand(), b = rand(), c = rand();
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = a * tmech::otimes(A, A) + b * tmech::otimes(B, B)
                                        + c * tmech::otimes(A, B));
}

template <typename T, std::size_t Dim>
static void BM(otimes_nodes_23)(benchmark::State &state) {
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    const T a = rand(), b = rand(), c = rand(), d = rand();
    A.randn(); B.randn();
    for (auto _ : state)
        benchmark::DoNotOptimize(result = a * tmech::otimes(A, A) + b * tmech::otimes(B, B)
                                        + c * tmech::otimes(A, B) + d * tmech::otimes(B, A));
}

} // anonymous namespace

// ── Registration with explicit names (so suffix appears in JSON output) ──────
#define STR_(x) #x
#define STR(x) STR_(x)
#define REG(name, ...) \
    BENCHMARK_TEMPLATE(BM(name), __VA_ARGS__) \
        ->Name(#name "_" STR(SUFFIX) "<" #__VA_ARGS__ ">")

REG(otimes_simple, double, 3ul);
REG(otimesu, double, 3ul);
REG(otimesl, double, 3ul);
REG(otimes_iso, double, 3ul);

REG(addition, double, 3ul);
REG(addition_4, double, 3ul);
REG(addition_8, double, 3ul);

REG(single_contraction, double, 3ul);
REG(single_contraction_times3, double, 3ul);
REG(more_complex, double, 3ul);

REG(double_contraction_4_2, double, 3ul);
REG(more_complex_4_2, double, 3ul);

REG(double_contraction_4_4, double, 2ul);
REG(double_contraction_4_4, double, 3ul);
REG(double_contraction_times3_4_4, double, 3ul);
REG(complex_double_contraction_4_4, double, 3ul);
REG(more_complex_4_4, double, 3ul);

REG(double_contraction_8_8, double, 2ul);
REG(double_contraction_8_8, double, 3ul);

REG(inverse, double, 3ul);

REG(otimes_nodes_3, double, 3ul);
REG(otimes_nodes_7, double, 3ul);
REG(otimes_nodes_11, double, 3ul);
REG(otimes_nodes_17, double, 3ul);
REG(otimes_nodes_23, double, 3ul);

#undef REG
#undef STR
#undef STR_
#undef BM
#undef CAT
#undef CAT_
