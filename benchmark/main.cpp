
// #undef __SEE__
// #undef __AVX__

#include <iostream>
#include <benchmark/benchmark.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>
#include <unsupported/Eigen/KroneckerProduct>
#include <armadillo>
#include <tmech/tmech.h>
#include <boost/align/aligned_allocator.hpp>
#include <Fastor/Fastor.h>

// #define Do2DBenchmark

template <typename Type, std::size_t Dim>
class tensor4;

template <typename Type, std::size_t Dim = 3>
class tensor2
{
    static constexpr auto Size{Dim * Dim};

public:
    tensor2() {}

    const auto &operator=(tensor2 const &_rhs)
    {

        for (std::size_t i{0}; i < Size; ++i)
        {

            _data[i] = _rhs._data[i];
        }

        return *this;
    }

    const auto &operator()(std::size_t i, std::size_t j) const
    {

        return _data[i * Dim + j];
    }

    auto &operator()(std::size_t i, std::size_t j)
    {

        return _data[i * Dim + j];
    }

    auto operator*(Type const _lhs) const
    {
        tensor2 temp = *this;
        for (std::size_t i{0}; i < Dim * Dim; ++i)
        {
            temp._data[i] *= _lhs;
        }
        return temp;
    }

    auto outerProduct(tensor2 const &_rhs) const
    {
        tensor4<Type, Dim> temp;
        for (std::size_t i{0}; i < Dim; ++i)
        {
            for (std::size_t j{0}; j < Dim; ++j)
            {
                for (std::size_t k{0}; k < Dim; ++k)
                {
                    for (std::size_t l{0}; l < Dim; ++l)
                    {
                        temp(i, j, k, l) = this->operator()(i, j) * _rhs(k, l);
                    }
                }
            }
        }

        return temp;
    }

    // otimesu: C_ikjl = A_ij * B_kl
    auto outerProductU(tensor2 const &_rhs) const
    {
        tensor4<Type, Dim> temp;
        for (std::size_t i{0}; i < Dim; ++i)
            for (std::size_t j{0}; j < Dim; ++j)
                for (std::size_t k{0}; k < Dim; ++k)
                    for (std::size_t l{0}; l < Dim; ++l)
                        temp(i, k, j, l) = this->operator()(i, j) * _rhs(k, l);
        return temp;
    }

    // otimesl: C_iljk = A_ij * B_kl
    auto outerProductL(tensor2 const &_rhs) const
    {
        tensor4<Type, Dim> temp;
        for (std::size_t i{0}; i < Dim; ++i)
            for (std::size_t j{0}; j < Dim; ++j)
                for (std::size_t k{0}; k < Dim; ++k)
                    for (std::size_t l{0}; l < Dim; ++l)
                        temp(i, l, j, k) = this->operator()(i, j) * _rhs(k, l);
        return temp;
    }

    void rand_()
    {

        for (std::size_t i{0}; i < Dim * Dim; ++i)
        {

            _data[i] = rand();
        }
    }

private:
    Type _data[Dim * Dim];
};

template <typename Type, std::size_t Dim = 3>
class tensor4
{
    static constexpr auto Size{Dim * Dim * Dim * Dim};

public:
    tensor4() {}

    const auto &operator=(tensor4 const &_rhs)
    {
        for (std::size_t i{0}; i < Size; ++i)
        {
            _data[i] = _rhs._data[i];
        }
        return *this;
    }

    const auto &operator()(std::size_t i, std::size_t j, std::size_t k, std::size_t l) const
    {
        return _data[i * Dim * Dim * Dim + j * Dim * Dim + k * Dim + l];
    }

    auto &operator()(std::size_t i, std::size_t j, std::size_t k, std::size_t l)
    {
        return _data[i * Dim * Dim * Dim + j * Dim * Dim + k * Dim + l];
    }

    auto operator*=(Type const _lhs)
    {
        for (std::size_t i{0}; i < Size; ++i)
        {
            _data[i] *= _lhs;
        }
        return *this;
    }

    auto operator+=(tensor4 const &_lhs)
    {
        for (std::size_t i{0}; i < Size; ++i)
        {
            _data[i] += _lhs._data[i];
        }
        return *this;
    }

    void rand_()
    {
        for (std::size_t i{0}; i < Dim * Dim * Dim * Dim; ++i)
        {
            _data[i] = rand();
        }
    }

    template <typename T, std::size_t _Dim>
    friend std::ostream &operator<<(std::ostream &out, tensor4<T, _Dim> const &);

private:
    Type _data[Dim * Dim * Dim * Dim];
};

template <typename T, std::size_t _Dim>
std::ostream &operator<<(std::ostream &out, tensor4<T, _Dim> const &t)
{
    for (std::size_t i{0}; i < _Dim; ++i)
    {
        for (std::size_t j{0}; j < _Dim; ++j)
        {
            for (std::size_t k{0}; k < _Dim; ++k)
            {
                for (std::size_t l{0}; l < _Dim; ++l)
                {
                    out << t(i, j, k, l) << " ";
                }
                out << "\n";
            }
        }
    }
    return out;
}

template <typename Type, std::size_t Dim>
inline auto operator*(Type _scalar, tensor4<Type, Dim> const &_rhs) noexcept
{
    tensor4<Type> temp = _rhs;
    temp *= _scalar;
    return temp;
}

template <typename Type, std::size_t Dim>
inline auto operator+(tensor4<Type, Dim> const &_lhs, tensor4<Type, Dim> const &_rhs) noexcept
{
    tensor4<Type> temp = _lhs;
    temp += _rhs;
    return temp;
}

enum
{
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T_
};

template <typename T, std::size_t Dim>
static void otimes_iso_old(benchmark::State &state)
{
    tensor2<T, Dim> pk2_part1, pk2_part2, pk2_part3;
    tensor4<T, Dim> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();

    pk2_part1.rand_();
    pk2_part2.rand_();
    pk2_part3.rand_();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a * pk2_part1.outerProduct(pk2_part1) +
                                          b * (pk2_part1.outerProduct(pk2_part2) + pk2_part2.outerProduct(pk2_part1)) +
                                          c * (pk2_part1.outerProduct(pk2_part3) + pk2_part3.outerProduct(pk2_part1)) +
                                          d * pk2_part2.outerProduct(pk2_part2) +
                                          e * (pk2_part2.outerProduct(pk2_part3) + pk2_part3.outerProduct(pk2_part2)) +
                                          f * pk2_part3.outerProduct(pk2_part3));
    }
}

template <typename T, std::size_t Dim>
static void otimes_iso_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> pk2_part1, pk2_part2, pk2_part3;
    tmech::tensor<T, Dim, 4> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();

    pk2_part1.randn();
    pk2_part2.randn();
    pk2_part3.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a * tmech::otimes(pk2_part1, pk2_part1) +
                                          b * (tmech::otimes(pk2_part1, pk2_part2) + tmech::otimes(pk2_part2, pk2_part1)) +
                                          c * (tmech::otimes(pk2_part1, pk2_part3) + tmech::otimes(pk2_part3, pk2_part1)) +
                                          d * tmech::otimes(pk2_part2, pk2_part2) +
                                          e * (tmech::otimes(pk2_part2, pk2_part3) + tmech::otimes(pk2_part3, pk2_part2)) +
                                          f * tmech::otimes(pk2_part3, pk2_part3));
    }
}

template <typename T, std::size_t Dim>
static void otimes_iso_tmech_8(benchmark::State &state)
{
    tmech::tensor<T, Dim, 4> pk2_part1, pk2_part2, pk2_part3;
    tmech::tensor<T, Dim, 8> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();

    pk2_part1.randn();
    pk2_part2.randn();
    pk2_part3.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a * tmech::otimes(pk2_part1, pk2_part1) +
                                          b * (tmech::otimes(pk2_part1, pk2_part2) + tmech::otimes(pk2_part2, pk2_part1)) +
                                          c * (tmech::otimes(pk2_part1, pk2_part3) + tmech::otimes(pk2_part3, pk2_part1)) +
                                          d * tmech::otimes(pk2_part2, pk2_part2) +
                                          e * (tmech::otimes(pk2_part2, pk2_part3) + tmech::otimes(pk2_part3, pk2_part2)) +
                                          f * tmech::otimes(pk2_part3, pk2_part3));
    }
}

template <typename T, std::size_t Dim>
static void otimes_iso_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> pk2_part1, pk2_part2, pk2_part3;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();

    pk2_part1.random();
    pk2_part2.random();
    pk2_part3.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a * Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part1, pk2_part1) +
                                          b * (Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part1, pk2_part2) + Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part2, pk2_part1)) +
                                          c * (Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part1, pk2_part3) + Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part3, pk2_part1)) +
                                          d * Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part2, pk2_part2) +
                                          e * (Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part2, pk2_part3) + Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part3, pk2_part2)) +
                                          f * Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part3, pk2_part3));
    }
}

template <typename T, std::size_t Dim>
static void otimes_iso_tmech_eval_8(benchmark::State &state)
{
    tmech::tensor<T, Dim, 4> pk2_part1, pk2_part2, pk2_part3;
    tmech::tensor<T, Dim, 8> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();

    pk2_part1.randn();
    pk2_part2.randn();
    pk2_part3.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a * tmech::eval(tmech::otimes(pk2_part1, pk2_part1)) +
                                          b * tmech::eval(tmech::otimes(pk2_part1, pk2_part2)) + tmech::eval(tmech::otimes(pk2_part2, pk2_part1)) +
                                          c * tmech::eval(tmech::otimes(pk2_part1, pk2_part3)) + tmech::eval(tmech::otimes(pk2_part3, pk2_part1)) +
                                          d * tmech::eval(tmech::otimes(pk2_part2, pk2_part2)) +
                                          e * tmech::eval(tmech::otimes(pk2_part2, pk2_part3)) + tmech::eval(tmech::otimes(pk2_part3, pk2_part2)) +
                                          f * tmech::eval(tmech::otimes(pk2_part3, pk2_part3)));
    }
}

template <typename T, std::size_t Dim>
static void otimes_iso_fastor_8(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> pk2_part1, pk2_part2, pk2_part3;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim, Dim, Dim, Dim, Dim> result;
    const T a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f = rand();

    pk2_part1.random();
    pk2_part2.random();
    pk2_part3.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a * Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part1, pk2_part1) +
                                          b * (Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part1, pk2_part2) + Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part2, pk2_part1)) +
                                          c * (Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part1, pk2_part3) + Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part3, pk2_part1)) +
                                          d * Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part2, pk2_part2) +
                                          e * (Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part2, pk2_part3) + Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part3, pk2_part2)) +
                                          f * Fastor::einsum<Fastor::Index<I, J>, Fastor::Index<K, L>>(pk2_part3, pk2_part3));
    }
}

// ── otimes simple benchmarks ─────────────────────────────────────────────────

template <typename T, std::size_t Dim>
static void otimes_simple_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn();
    B.randn();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = tmech::otimes(A, B));
    }
}

template <typename T, std::size_t Dim>
static void otimes_simple_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim> A, B;
    Eigen::Matrix<T, Dim * Dim, Dim * Dim> result;
    A = A.Random();
    B = B.Random();
    for (auto _ : state)
    {
        // otimes: C_ijkl stored as (i*Dim+j, k*Dim+l)
        for (std::size_t i = 0; i < Dim; ++i)
            for (std::size_t j = 0; j < Dim; ++j)
                for (std::size_t k = 0; k < Dim; ++k)
                    for (std::size_t l = 0; l < Dim; ++l)
                        result(i * Dim + j, k * Dim + l) = A(i, j) * B(k, l);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T, std::size_t Dim>
static void otimes_simple_eigen_voigt(benchmark::State &state)
{
    // otimes: C_ijkl = A_ij * B_kl  →  Voigt: C = a * b^T
    Eigen::Matrix<T, Dim, Dim> A, B;
    Eigen::Vector<T, Dim * Dim> a_vec, b_vec;
    Eigen::Matrix<T, Dim * Dim, Dim * Dim> result;
    A = A.Random();
    B = B.Random();
    for (std::size_t i = 0; i < Dim; ++i)
        for (std::size_t j = 0; j < Dim; ++j) {
            a_vec(i * Dim + j) = A(i, j);
            b_vec(i * Dim + j) = B(i, j);
        }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = a_vec * b_vec.transpose());
    }
}

template <typename T, std::size_t Dim>
static void otimes_simple_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> result;
    A.random();
    B.random();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = Fastor::outer(A, B));
    }
}

// ── otimesu benchmarks ──────────────────────────────────────────────────────

template <typename T, std::size_t Dim>
static void otimesu_old(benchmark::State &state)
{
    tensor2<T, Dim> A, B;
    tensor4<T, Dim> result;
    A.rand_();
    B.rand_();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = A.outerProductU(B));
    }
}

template <typename T, std::size_t Dim>
static void otimesu_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn();
    B.randn();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = tmech::otimesu(A, B));
    }
}

template <typename T, std::size_t Dim>
static void otimesu_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim> A, B;
    Eigen::Matrix<T, Dim * Dim, Dim * Dim> result;
    A = A.Random();
    B = B.Random();
    for (auto _ : state)
    {
        // otimesu: C_ikjl stored as (i*Dim+k, j*Dim+l)
        for (std::size_t i = 0; i < Dim; ++i)
            for (std::size_t j = 0; j < Dim; ++j)
                for (std::size_t k = 0; k < Dim; ++k)
                    for (std::size_t l = 0; l < Dim; ++l)
                        result(i * Dim + k, j * Dim + l) = A(i, j) * B(k, l);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T, std::size_t Dim>
static void otimesu_eigen_voigt(benchmark::State &state)
{
    // otimesu: C_ikjl = A_ij * B_kl  →  Voigt: C = A ⊗ B (Kronecker product)
    Eigen::Matrix<T, Dim, Dim> A, B;
    Eigen::Matrix<T, Dim * Dim, Dim * Dim> result;
    A = A.Random();
    B = B.Random();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = Eigen::kroneckerProduct(A, B));
    }
}

template <typename T, std::size_t Dim>
static void otimesu_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> result;
    A.random();
    B.random();
    for (auto _ : state)
    {
        // otimesu: C_ikjl = permute(A_ij * B_kl, {i,k,j,l})
        benchmark::DoNotOptimize(result = Fastor::permute<Fastor::Index<I, K, J, L>>(Fastor::outer(A, B)));
    }
}

// ── otimesl benchmarks ──────────────────────────────────────────────────────

template <typename T, std::size_t Dim>
static void otimesl_old(benchmark::State &state)
{
    tensor2<T, Dim> A, B;
    tensor4<T, Dim> result;
    A.rand_();
    B.rand_();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = A.outerProductL(B));
    }
}

template <typename T, std::size_t Dim>
static void otimesl_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B;
    tmech::tensor<T, Dim, 4> result;
    A.randn();
    B.randn();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = tmech::otimesl(A, B));
    }
}

template <typename T, std::size_t Dim>
static void otimesl_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim> A, B;
    Eigen::Matrix<T, Dim * Dim, Dim * Dim> result;
    A = A.Random();
    B = B.Random();
    for (auto _ : state)
    {
        // otimesl: C_iljk stored as (i*Dim+l, j*Dim+k)
        for (std::size_t i = 0; i < Dim; ++i)
            for (std::size_t j = 0; j < Dim; ++j)
                for (std::size_t k = 0; k < Dim; ++k)
                    for (std::size_t l = 0; l < Dim; ++l)
                        result(i * Dim + l, j * Dim + k) = A(i, j) * B(k, l);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T, std::size_t Dim>
static void otimesl_eigen_voigt(benchmark::State &state)
{
    // otimesl: C_iljk = A_ij * B_kl  →  Voigt: C = A ⊗ B^T
    Eigen::Matrix<T, Dim, Dim> A, B;
    Eigen::Matrix<T, Dim * Dim, Dim * Dim> result;
    A = A.Random();
    B = B.Random();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(result = Eigen::kroneckerProduct(A, B.transpose()));
    }
}

template <typename T, std::size_t Dim>
static void otimesl_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> result;
    A.random();
    B.random();
    for (auto _ : state)
    {
        // otimesl: C_iljk = permute(A_ij * B_kl, {i,l,j,k})
        benchmark::DoNotOptimize(result = Fastor::permute<Fastor::Index<I, L, J, K>>(Fastor::outer(A, B)));
    }
}

template <typename T, std::size_t Dim>
static void more_complex_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B, C;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B + A + B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B + A + B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, K>, Fastor::Index<K, J>>(A, B) + A + B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_2_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> B, C;
    tmech::tensor<T, Dim, 4> A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B));
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_2_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim, Dim * Dim, Eigen::ColMajor> A;
    Eigen::Vector<T, Dim * Dim> B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_2_eigen_voigt(benchmark::State &state)
{
    Eigen::Matrix<T, (Dim == 2 ? 4 : 6), (Dim == 2 ? 4 : 6), Eigen::ColMajor> A;
    Eigen::Vector<T, (Dim == 2 ? 4 : 6)> B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_2_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> B, C;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L>>(A, B));
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_2_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> B, C;
    tmech::tensor<T, Dim, 4> A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B) + 2 * B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_2_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim, Dim * Dim, Eigen::ColMajor> A;
    Eigen::Vector<T, Dim * Dim> B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B + 2 * B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_2_eigen_voigt(benchmark::State &state)
{
    Eigen::Matrix<T, (Dim == 2 ? 4 : 6), (Dim == 2 ? 4 : 6), Eigen::ColMajor> A;
    Eigen::Vector<T, (Dim == 2 ? 4 : 6)> B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B + 2 * B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_2_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> B, C;
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L>>(A, B) + 2 * B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_4_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 4> B, C, A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B));
        // C = tmech::dcontract(A,B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_4_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim, Dim * Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_4_eigen_voigt(benchmark::State &state)
{
    Eigen::Matrix<T, (Dim == 2 ? 4 : 6), (Dim == 2 ? 4 : 6), Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_4_4_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L, M, N>>(A, B));
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_times3_4_4_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 4> B, C, A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::dcontract(A, tmech::dcontract(B, A)));
        // C = tmech::dcontract(A,B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_times3_4_4_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim, Dim * Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B * A);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_times3_4_4_eigen_voigt(benchmark::State &state)
{
    Eigen::Matrix<T, (Dim == 2 ? 4 : 6), (Dim == 2 ? 4 : 6), Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B * A);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_times3_4_4_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L, M, N>>(A, Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L, M, N>>(B, A)));
    }
}

template <typename T, std::size_t Dim>
static void complex_double_contraction_4_4_tmech(benchmark::State &state)
{
    using SeqL = tmech::sequence<1, 3>;
    using SeqR = tmech::sequence<2, 4>;
    tmech::tensor<T, Dim, 4> B, C, A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::inner_product<SeqL, SeqR>(A, B));
    }
}

template <typename T, std::size_t Dim>
static void complex_double_contraction_4_4_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<K, I, L, J>, Fastor::Index<M, K, N, L>>(A, B));
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_4_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 4> B, C, A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::dcontract(A, B) + 2 * B + B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_4_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim, Dim * Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B + 2 * B + B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_4_eigen_voigt(benchmark::State &state)
{
    Eigen::Matrix<T, (Dim == 2 ? 4 : 6), (Dim == 2 ? 4 : 6), Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B + 2 * B + B);
    }
}

template <typename T, std::size_t Dim>
static void more_complex_4_4_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L, M, N>>(A, B) + 2 * B + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B, C;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_tmech_4(benchmark::State &state)
{
    tmech::tensor<T, Dim, 4> A, B, C;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_eigen_4(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim, Dim * Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_fastor_4(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_tmech_8(benchmark::State &state)
{
    tmech::tensor<T, Dim, 8> A, B, C;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_eigen_8(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim * Dim * Dim, Dim * Dim * Dim * Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void addition_fastor_8(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A + B);
    }
}

template <typename T, std::size_t Dim>
static void single_contraction_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B, C;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void single_contraction_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void single_contraction_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, K>, Fastor::Index<K, J>>(A, B));
    }
}

template <typename T, std::size_t Dim>
static void single_contraction_times3_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, B, C;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B * A);
    }
}

template <typename T, std::size_t Dim>
static void single_contraction_times3_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B * A);
    }
}

template <typename T, std::size_t Dim>
static void single_contraction_times3_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, K>, Fastor::Index<K, J>>(A, Fastor::einsum<Fastor::Index<I, K>, Fastor::Index<K, J>>(B, A)));
    }
}

template <typename T, std::size_t Dim>
static void inverse_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim> A, A_inv;
    A_inv.Random();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(A = A_inv.inverse());
    }
}

template <typename T, std::size_t Dim>
static void inverse_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, A_inv;
    A_inv.randn();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(A = tmech::inv(A_inv));
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_8_8_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 8> B, C, A;
    B.randn();
    A.randn();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = tmech::ddcontract(A, B));
        // C = tmech::dcontract(A,B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_8_8_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim * Dim * Dim * Dim, Dim * Dim * Dim * Dim, Eigen::ColMajor> A, B, C;
    A = A.Random();
    B = B.Random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = A * B);
    }
}

template <typename T, std::size_t Dim>
static void double_contraction_8_8_fastor(benchmark::State &state)
{
    Fastor::Tensor<T, Dim, Dim, Dim, Dim, Dim, Dim, Dim, Dim> A, B, C;
    A.random();
    B.random();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(C = Fastor::einsum<Fastor::Index<I, J, K, L, M, N, O, P>, Fastor::Index<M, N, O, P, Q, R, S, T_>>(A, B));
    }
}

#ifdef Do2DBenchmark
BENCHMARK_TEMPLATE(addition_tmech, double, 2ul);
BENCHMARK_TEMPLATE(addition_eigen, double, 2ul);
BENCHMARK_TEMPLATE(addition_fastor, double, 2ul);

BENCHMARK_TEMPLATE(single_contraction_tmech, double, 2ul);
BENCHMARK_TEMPLATE(single_contraction_eigen, double, 2ul);
BENCHMARK_TEMPLATE(single_contraction_fastor, double, 2ul);

BENCHMARK_TEMPLATE(more_complex_tmech, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_eigen, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_fastor, double, 2ul);

BENCHMARK_TEMPLATE(double_contraction_4_2_tmech, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_2_eigen, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_2_eigen_voigt, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_2_fastor, double, 2ul);

BENCHMARK_TEMPLATE(more_complex_4_2_tmech, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_4_2_eigen, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_4_2_eigen_voigt, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_4_2_fastor, double, 2ul);

BENCHMARK_TEMPLATE(double_contraction_4_4_tmech, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_eigen, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_eigen_voigt, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_fastor, double, 2ul);

BENCHMARK_TEMPLATE(complex_double_contraction_4_4_tmech, double, 2ul);
BENCHMARK_TEMPLATE(complex_double_contraction_4_4_fastor, double, 2ul);

BENCHMARK_TEMPLATE(more_complex_4_4_tmech, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_4_4_eigen, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_4_4_eigen_voigt, double, 2ul);
BENCHMARK_TEMPLATE(more_complex_4_4_fastor, double, 2ul);
#endif

BENCHMARK_TEMPLATE(otimes_simple_tmech, double, 3ul);
BENCHMARK_TEMPLATE(otimes_simple_eigen, double, 3ul);
BENCHMARK_TEMPLATE(otimes_simple_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(otimes_simple_fastor, double, 3ul);

BENCHMARK_TEMPLATE(otimesu_old, double, 3ul);
BENCHMARK_TEMPLATE(otimesu_tmech, double, 3ul);
BENCHMARK_TEMPLATE(otimesu_eigen, double, 3ul);
BENCHMARK_TEMPLATE(otimesu_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(otimesu_fastor, double, 3ul);

BENCHMARK_TEMPLATE(otimesl_old, double, 3ul);
BENCHMARK_TEMPLATE(otimesl_tmech, double, 3ul);
BENCHMARK_TEMPLATE(otimesl_eigen, double, 3ul);
BENCHMARK_TEMPLATE(otimesl_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(otimesl_fastor, double, 3ul);

BENCHMARK_TEMPLATE(otimes_iso_old, double, 3ul);
BENCHMARK_TEMPLATE(otimes_iso_tmech, double, 3ul);
BENCHMARK_TEMPLATE(otimes_iso_fastor, double, 3ul);

BENCHMARK_TEMPLATE(addition_tmech, double, 3ul);
BENCHMARK_TEMPLATE(addition_eigen, double, 3ul);
BENCHMARK_TEMPLATE(addition_fastor, double, 3ul);

BENCHMARK_TEMPLATE(addition_tmech_4, double, 3ul);
BENCHMARK_TEMPLATE(addition_eigen_4, double, 3ul);
BENCHMARK_TEMPLATE(addition_fastor_4, double, 3ul);

BENCHMARK_TEMPLATE(addition_tmech_8, double, 3ul);
BENCHMARK_TEMPLATE(addition_eigen_8, double, 3ul);
BENCHMARK_TEMPLATE(addition_fastor_8, double, 3ul);

BENCHMARK_TEMPLATE(single_contraction_tmech, double, 3ul);
BENCHMARK_TEMPLATE(single_contraction_eigen, double, 3ul);
BENCHMARK_TEMPLATE(single_contraction_fastor, double, 3ul);

BENCHMARK_TEMPLATE(single_contraction_times3_tmech, double, 3ul);
BENCHMARK_TEMPLATE(single_contraction_times3_eigen, double, 3ul);
BENCHMARK_TEMPLATE(single_contraction_times3_fastor, double, 3ul);

BENCHMARK_TEMPLATE(more_complex_tmech, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_eigen, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_fastor, double, 3ul);

BENCHMARK_TEMPLATE(double_contraction_4_2_tmech, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_4_2_eigen, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_4_2_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_4_2_fastor, double, 3ul);

BENCHMARK_TEMPLATE(more_complex_4_2_tmech, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_4_2_eigen, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_4_2_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_4_2_fastor, double, 3ul);

BENCHMARK_TEMPLATE(double_contraction_4_4_tmech, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_eigen, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_eigen_voigt, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_fastor, double, 2ul);

BENCHMARK_TEMPLATE(double_contraction_4_4_tmech, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_eigen, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_4_4_fastor, double, 3ul);

BENCHMARK_TEMPLATE(double_contraction_times3_4_4_tmech, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_times3_4_4_eigen, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_times3_4_4_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_times3_4_4_fastor, double, 3ul);

BENCHMARK_TEMPLATE(complex_double_contraction_4_4_tmech, double, 3ul);
BENCHMARK_TEMPLATE(complex_double_contraction_4_4_fastor, double, 3ul);

BENCHMARK_TEMPLATE(more_complex_4_4_tmech, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_4_4_eigen, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_4_4_eigen_voigt, double, 3ul);
BENCHMARK_TEMPLATE(more_complex_4_4_fastor, double, 3ul);

BENCHMARK_TEMPLATE(double_contraction_8_8_tmech, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_8_8_eigen, double, 2ul);
BENCHMARK_TEMPLATE(double_contraction_8_8_fastor, double, 2ul);

BENCHMARK_TEMPLATE(double_contraction_8_8_tmech, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_8_8_eigen, double, 3ul);
BENCHMARK_TEMPLATE(double_contraction_8_8_fastor, double, 3ul);

BENCHMARK_TEMPLATE(inverse_tmech, double, 3ul);
BENCHMARK_TEMPLATE(inverse_eigen, double, 3ul);

//-------------------------------------single contraction--------------------------
template <typename T, std::size_t Dim>
static void BM_single_contraction_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, 2> A, A_inv, B, C;
    B.randn();
    A.randn();
    for (auto _ : state)
    {
        C = A * B;
        B = A * C;
    }
}
template <typename T, std::size_t SIZE, std::size_t Dim>
static void BM_single_contraction_tmech_vector(benchmark::State &state)
{
    std::vector<tmech::tensor<T, Dim, 2>> A(SIZE), B(SIZE), C(SIZE);
    for (int i{0}; i < SIZE; ++i)
    {
        A[i].randn();
        B[i].randn();
    }
    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            C[i] = A[i] * B[i];
        }
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim>
static void BM_single_contraction_fastor_vector(benchmark::State &state)
{
    std::vector<Fastor::Tensor<T, Dim, Dim>> A(SIZE), B(SIZE), C(SIZE);
    for (int i{0}; i < SIZE; ++i)
    {
        A[i].random();
        B[i].random();
    }
    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            C[i] = Fastor::einsum<Fastor::Index<I, K>, Fastor::Index<K, J>>(A[i], B[i]);
        }
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim>
static void BM_general_contraction_fastor_vector(benchmark::State &state)
{
    std::vector<Fastor::Tensor<T, Dim, Dim, Dim, Dim>> A(SIZE), B(SIZE), C(SIZE);
    for (int i{0}; i < SIZE; ++i)
    {
        A[i].random();
        B[i].random();
    }
    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            C[i] = Fastor::einsum<Fastor::Index<I, J, K, L>, Fastor::Index<K, L, M, N>>(A[i], B[i]);
        }
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim>
static void BM_single_contraction_eigen_vector(benchmark::State &state)
{
    std::vector<Eigen::Matrix<T, Dim, Dim>> A(SIZE), B(SIZE), C(SIZE);
    for (int i{0}; i < SIZE; ++i)
    {
        A[i].Random();
        B[i].Random();
    }
    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            C[i] = A[i] * B[i];
        }
    }
}

template <typename T, std::size_t Dim>
static void BM_single_contraction_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, Dim, Dim> A, B, C;
    A = A.Identity();
    B = B.Random();

    for (auto _ : state)
    {
        C = A * B;
        B = A * C;
    }
}

template <typename T, std::size_t Dim>
static void BM_single_contraction_arma(benchmark::State &state)
{
    if constexpr (std::is_same_v<T, double>)
    {
        using value_type = double;
        arma::Mat<value_type>::fixed<Dim, Dim> A(arma::fill::eye), B(arma::fill::eye), C(arma::fill::eye);
        B.randn();

        for (auto _ : state)
        {
            C = A * B;
            B = A * C;
        }
    }
    else
    {
        using value_type = float;
        arma::Mat<value_type>::fixed<Dim, Dim> A(arma::fill::eye), B(arma::fill::eye), C(arma::fill::eye);
        B.randn();

        for (auto _ : state)
        {
            C = A * B;
            B = A * C;
        }
    }
}

//-------------------------------------general contraction--------------------------
template <typename T, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_general_contraction(benchmark::State &state)
{

    using seqL = tmech::sequence<3, 4>;
    using seqR = tmech::sequence<1, 2>;

    tmech::tensor<double, Dim, RankRHS> A;
    tmech::tensor<double, Dim, RankLHS> a, res;
    A.randn();
    a.randn();

    for (auto _ : state)
    {
        // tmech::detail::meta_for_loop_contract::evaluate<seqL, seqR>(A, a, res);
        res = tmech::inner_product<seqL, seqR>(A, a);
        a = res;
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_general_contraction_vector_left(benchmark::State &state)
{

    using seqL = tmech::sequence<3, 4>;
    using seqR = tmech::sequence<1, 2>;
    std::vector<tmech::tensor<double, Dim, RankRHS>> A(SIZE);
    std::vector<tmech::tensor<double, Dim, RankLHS>> a(SIZE), res(SIZE);

    tmech::tensor<double, Dim, RankRHS> randA;
    tmech::tensor<double, Dim, RankLHS> randa;
    randA.randn();
    randa.randn();

    for (int i{0}; i < SIZE; ++i)
    {
        A[i] = randA;
        a[i] = randa;
    }
    // std::cout<<tmech::inner_product<seqL,seqR>(A[0],a[0])<<std::endl;
    // std::cout<<tmech::dcontract(A[0],a[0])<<std::endl;

    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            res[i] = tmech::inner_product<seqR, seqL>(a[i], A[i]);
            // res[i] = tmech::inv(a[i]);
        }
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_general_contraction_vector(benchmark::State &state)
{

    using seqL = tmech::sequence<3, 4>;
    using seqR = tmech::sequence<1, 2>;
    std::vector<tmech::tensor<double, Dim, RankRHS>> A(SIZE);
    std::vector<tmech::tensor<double, Dim, RankLHS>> a(SIZE), res(SIZE);

    tmech::tensor<double, Dim, RankRHS> randA;
    tmech::tensor<double, Dim, RankLHS> randa;
    randA.randn();
    randa.randn();

    for (int i{0}; i < SIZE; ++i)
    {
        A[i] = randA;
        a[i] = randa;
    }
    // std::cout<<tmech::inner_product<seqL,seqR>(A[0],a[0])<<std::endl;
    // std::cout<<tmech::dcontract(A[0],a[0])<<std::endl;

    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            res[i] = tmech::inner_product<seqL, seqR>(A[i], a[i]);
            // res[i] = tmech::inv(a[i]);
        }
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_general_fourth_contraction_vector(benchmark::State &state)
{

    using seqL = tmech::sequence<5, 6, 7, 8>;
    using seqR = tmech::sequence<1, 2, 3, 4>;
    std::vector<tmech::tensor<double, Dim, RankRHS>> A(SIZE);
    std::vector<tmech::tensor<double, Dim, RankLHS>> a(SIZE), res(SIZE);

    tmech::tensor<double, Dim, RankRHS> randA;
    tmech::tensor<double, Dim, RankLHS> randa;
    randA.randn();
    randa.randn();

    for (int i{0}; i < SIZE; ++i)
    {
        A[i] = randA;
        a[i] = randa;
    }
    // std::cout<<tmech::inner_product<seqL,seqR>(A[0],a[0])<<std::endl;
    // std::cout<<tmech::dcontract(A[0],a[0])<<std::endl;

    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            res[i] = tmech::inner_product<seqL, seqR>(A[i], a[i]);
            // res[i] = tmech::inv(a[i]);
        }
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_general_single_contraction_vector(benchmark::State &state)
{

    using seqL = tmech::sequence<2>;
    using seqR = tmech::sequence<1>;

    // aligned_vector<tmech::tensor<double, Dim, RankRHS>> A(SIZE);
    // aligned_vector<tmech::tensor<double, Dim, RankLHS>> a(SIZE), res(SIZE);
    std::vector<tmech::tensor<double, Dim, RankRHS>> A(SIZE);
    std::vector<tmech::tensor<double, Dim, RankLHS>> a(SIZE), res(SIZE);

    tmech::tensor<double, Dim, RankRHS> randA;
    tmech::tensor<double, Dim, RankRHS> randa;
    randA.randn();
    randa.randn();

    for (int i{0}; i < SIZE; ++i)
    {
        A[i] = randA;
        a[i] = randa;
    }

    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            res[i] = tmech::inner_product<seqL, seqR>(A[i], a[i]);
            // res[i] = tmech::inv(a[i]);
        }
    }
}

//-----------------trans general--------------------------------------
template <typename T, std::size_t Dim, std::size_t Rank>
static void BM_trans_general_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, Rank> B, C;
    tmech::tensor<T, Dim, Rank> A, A_inv;
    A.randn();
    B.randn();

    for (auto _ : state)
    {
        C = tmech::basis_change<tmech::sequence<1, 2>, tmech::sequence<3, 4>>(B);
        B = C;
    }
}

template <typename T, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_double_contraction_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, RankLHS> B, C;
    tmech::tensor<T, Dim, RankRHS> A, A_inv;
    A.randn();
    B.randn();
    A_inv = tmech::inv(A);

    for (auto _ : state)
    {
        C = tmech::dcontract(A, B);
        B = C;
    }
}

template <typename T, std::size_t SIZE, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_double_contraction_tmech_vector(benchmark::State &state)
{
    std::vector<tmech::tensor<T, Dim, RankLHS>> B(SIZE), C(SIZE);
    std::vector<tmech::tensor<T, Dim, RankRHS>> A(SIZE), A_inv(SIZE);

    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            C[i] = tmech::dcontract(A[i], B[i]);
        }
    }
}

template <typename T, std::size_t Dim, std::size_t RankLHS, std::size_t RankRHS>
static void BM_double_contraction_left_tmech(benchmark::State &state)
{
    tmech::tensor<T, Dim, RankLHS> B, C;
    tmech::tensor<T, Dim, RankRHS> A, A_inv;
    A.randn();
    B.randn();
    A_inv = tmech::inv(A);

    for (auto _ : state)
    {
        C = tmech::dcontract(B, A);
        B = tmech::dcontract(C, A_inv);
    }
}

template <typename T, std::size_t SizeLHS, std::size_t SizeRHS>
static void BM_double_contraction_eigen(benchmark::State &state)
{
    Eigen::Matrix<T, SizeLHS, SizeLHS> A, A_inv;
    Eigen::Matrix<T, SizeLHS, SizeRHS> B, C;
    B = B.Random();
    A = A.Random();
    A_inv = A_inv.Random();

    for (auto _ : state)
    {
        C = A * B;
        B = C;
    }
}

template <typename T, std::size_t SIZE, std::size_t SizeLHS, std::size_t SizeRHS>
static void BM_double_contraction_eigen_vector(benchmark::State &state)
{
    std::vector<Eigen::Matrix<T, SizeLHS, SizeLHS>> A(SIZE);
    std::vector<Eigen::Matrix<T, SizeLHS, SizeRHS>> B(SIZE), C(SIZE);

    A[0] = A[0].Random();
    B[0] = B[0].Random();
    for (int i{0}; i < SIZE; ++i)
    {
        A[i] = A[0];
        B[i] = B[0];
    }

    for (auto _ : state)
    {
        for (int i{0}; i < SIZE; ++i)
        {
            C[i] = A[i] * B[i];
            // C[i] = B[i].inverse();
        }

        // B = C;
    }
}

template <std::size_t SizeLHS, std::size_t SizeRHS>
static void BM_double_contraction_arma_d(benchmark::State &state)
{
    using value_type = double;
    arma::Mat<value_type>::fixed<SizeLHS, SizeLHS> A, A_inv;
    arma::Mat<value_type>::fixed<SizeLHS, SizeRHS> B, C;
    A.randn();
    B.randn();
    A_inv = arma::inv(A);

    for (auto _ : state)
    {
        C = A * B;
        B = C;
    }
}

//------------------------exp-----------------------------
template <std::size_t Dim>
static void BM_exp_tmech(benchmark::State &state)
{
    using value_type = double;
    tmech::tensor<value_type, Dim, 2> B, C;
    B.randn();

    for (auto _ : state)
    {
        C = tmech::exp(B);
    }
}

template <std::size_t Dim>
static void BM_exp_eigen(benchmark::State &state)
{
    using value_type = double;
    Eigen::Matrix<value_type, Dim, Dim> A, B;
    B = B.Random();

    for (auto _ : state)
    {
        A = B.exp();
    }
}

template <std::size_t Dim>
static void BM_exp_arma(benchmark::State &state)
{
    using value_type = double;
    arma::Mat<value_type>::fixed<Dim, Dim> A, B;
    B.randn();

    for (auto _ : state)
    {
        A = arma::expmat(B);
    }
}
////--------------------------------------------------------------------
////static void BM_double_contraction_44_tmech(benchmark::State& state) {
////    using value_type = double;
////    constexpr std::size_t Dim{3};
////    tmech::tensor<value_type, Dim, 4> B, C;
////    tmech::tensor<value_type, Dim, 4> A, A_inv;
////    B.randn();
////    A.randn();
////    A_inv = tmech::inv(A);

////    for (auto _ : state){
////        C = tmech::dcontract(A, B);
////        B = tmech::dcontract(A, C);
////    }
////}
////BENCHMARK(BM_double_contraction_44_tmech);

////static void BM_double_contraction_44_eigen(benchmark::State& state) {
////    using value_type = double;
////    constexpr std::size_t Dim{3*2};

////    Eigen::Matrix<value_type, Dim, Dim> A, A_inv;
////    Eigen::Matrix<value_type, Dim, Dim> B, C;
////    for(std::size_t i{0}; i<Dim; ++i){
////        A(i,i) = 1;
////        A_inv(i,i) = 1;
////    }
////    B = B.Random();

////    for (auto _ : state){
////        C = A*B;
////        B = A_inv*C;
////    }
////}
////BENCHMARK(BM_double_contraction_44_eigen);

////static void BM_double_contraction_44_full_eigen(benchmark::State& state) {
////    using value_type = double;
////    constexpr std::size_t Dim{3*9};

////    Eigen::Matrix<value_type, Dim, Dim> A, A_inv;
////    Eigen::Matrix<value_type, Dim, Dim> B, C;
////    for(std::size_t i{0}; i<Dim; ++i){
////        A(i,i) = 1;
////        A_inv(i,i) = 1;
////    }
////    B = B.Random();

////    for (auto _ : state){
////        C = A*B;
////        B = A_inv*C;
////    }
////}
////BENCHMARK(BM_double_contraction_44_full_eigen);

//////static void BM_double_contraction_44_arma(benchmark::State& state) {
//////    using value_type = double;
//////    constexpr std::size_t Dim{3*2};
//////    std::vector<arma::Mat<value_type>::fixed<Dim, Dim>> A(SIZE);
//////    std::vector<arma::Mat<value_type>::fixed<Dim, Dim>> B(SIZE), C(SIZE);
//////    A[0].randn();
//////    B[0].randn();

//////    for (auto _ : state){
//////        for(std::size_t i{0}; i<SIZE; ++i)
//////            C[i] = A[i]*B[i];
//////    }
//////}
//////BENCHMARK(BM_double_contraction_44_arma);

////static void BM_inverse_44_full_tmech(benchmark::State& state) {
////    using value_type = double;
////    constexpr std::size_t Dim{3};
////    tmech::tensor<value_type, Dim, 4> A, A_inv;
////    A.randn();
////    A_inv.randn();
////    for (auto _ : state){
////        A = tmech::inv(A_inv);
////        A_inv = tmech::inv(A);
////    }
////}
////BENCHMARK(BM_inverse_44_full_tmech);

template <std::size_t Dim, std::size_t Rank>
static void BM_inverse_full_tmech(benchmark::State &state)
{
    using value_type = double;
    tmech::tensor<value_type, Dim, Rank> A, A_inv;
    if constexpr (Rank == 2)
    {
        A.randn();
        A_inv.randn();
    }
    else
    {
        A = tmech::eye<value_type, Dim, Rank>();
        A_inv = tmech::eye<value_type, Dim, Rank>();
    }

    for (auto _ : state)
    {
        A = tmech::invf(A_inv);
        A_inv = tmech::invf(A);
    }
}

template <std::size_t Dim, std::size_t Rank>
static void BM_inverse_tmech(benchmark::State &state)
{
    using value_type = double;
    tmech::tensor<value_type, Dim, Rank> A, A_inv;
    if constexpr (Rank == 2)
    {
        A.randn();
        A_inv.randn();
    }
    else
    {
        A = tmech::eye<value_type, Dim, Rank>();
        A_inv = tmech::eye<value_type, Dim, Rank>();
    }

    for (auto _ : state)
    {
        A = tmech::inv(A_inv);
        A_inv = tmech::inv(A);
    }
}

template <std::size_t Size>
static void BM_inverse_eigen(benchmark::State &state)
{
    using value_type = double;
    Eigen::Matrix<value_type, Size, Size> A, A_inv;
    A.fill(0);
    A_inv.fill(0);
    for (std::size_t i{0}; i < Size; ++i)
    {
        A(i, i) = 1;
        A_inv(i, i) = 1;
    }

    for (auto _ : state)
    {
        A = A_inv.inverse();
        A_inv = A.inverse();
    }
}

// template<std::size_t Size>
// static void BM_inverse_arma(benchmark::State& state) {
//     using value_type = double;
//     Eigen::Matrix<value_type, Size, Size> A, A_inv;
//     A.fill(0);
//     A_inv.fill(0);
//     for(std::size_t i{0}; i<Size; ++i){
//         A(i,i) = 1;
//         A_inv(i,i) = 1;
//     }

//    for (auto _ : state){
//        A     = A_inv.inverse();
//        A_inv = A.inverse();
//    }
//}

// template<std::size_t Dim>
// static void contractio_first(benchmark::State& state) {
//     using value_type = double;
//     tmech::tensor<value_type, Dim, 4> A;
//     tmech::tensor<value_type, Dim, 2> a, b;
//     A.randn();
//     b.randn();

//    for (auto _ : state){
//        for(std::size_t i{0};i<Dim;++i){
//            for(std::size_t j{0};j<Dim;++j){
//                value_type sum{0};
//                for(std::size_t k{0};k<Dim;++k){
//                    for(std::size_t l{0};l<Dim;++l){
//                        sum += A(i,k,l,j) * b(k,l);
//                    }
//                }
//                a(i,j) = sum;
//            }
//        }
//        b = a;
//    }
//}

// template<std::size_t Dim>
// static void contractio_second(benchmark::State& state) {
//     using value_type = double;
//     tmech::tensor<value_type, Dim, 4> A;
//     tmech::tensor<value_type, Dim, 2> a, b, atemp;
//     A.randn();
//     b.randn();
//     double const* const A_ptr{A.raw_data()};
//     for (auto _ : state){
//         for(std::size_t i{0};i<Dim;++i){
//             for(std::size_t j{0};j<Dim;++j){
//                 value_type sum{0};
//                 for(std::size_t k{0};k<Dim;++k){
//                     for(std::size_t l{0};l<Dim;++l){
//                         sum += A_ptr[(i*Dim+k)*(Dim*Dim)+(l*Dim+j)] * b.raw_data()[k*Dim+l];
//                     }
//                 }
//                 atemp.raw_data()[i*Dim+j] = sum;
//             }
//         }
//         a = atemp;
//         b = a;
//     }
// }

// constexpr std::size_t inner_indices_2D[4][4]{{0,  2,  4,  6},
//                                                     {1,  3,  5,  7},
//                                                     {8, 10, 12, 14},
//                                                     {9, 11, 13, 15}};
// template<std::size_t Dim>
// static void contractio_third(benchmark::State& state) {
//     using value_type = double;
//     tmech::tensor<value_type, Dim, 4> A;
//     tmech::tensor<value_type, Dim, 2> a, b;
//     A.randn();
//     b.randn();

//    for (auto _ : state){
//        for(std::size_t i{0};i<Dim*Dim;++i){
//            value_type sum{0};
//            for(std::size_t k{0};k<Dim*Dim;++k){
//                sum += A.raw_data()[inner_indices_2D[i][k]] * b.raw_data()[k];
//            }

//            a.raw_data()[i] = sum;
//        }
//        b = a;
//    }
//}

// template<std::size_t Dim>
// static void contractio_4_4_first(benchmark::State& state) {
//     using value_type = double;
//     tmech::tensor<value_type, Dim, 4> A;
//     tmech::tensor<value_type, Dim, 4> a, b;
//     A.randn();
//     b.randn();

//    for (auto _ : state){
//        for(std::size_t i{0};i<Dim;++i){
//            for(std::size_t l{0};l<Dim;++l){
//                for(std::size_t r{0};r<Dim;++r){
//                    for(std::size_t n{0};n<Dim;++n){
//                        value_type sum{0};
//                        for(std::size_t m{0};m<Dim;++m){
//                            for(std::size_t j{0};j<Dim;++j){
//                                sum += A(i,j,m,n) * b(j,l,r,m);
//                            }
//                        }
//                        a(i,l,r,n) = sum;
//                    }
//                }
//            }
//        }
//        b = a;
//    }
//}

////peformance polar newton vs polar eigen
template <std::size_t Dim>
static void polar_decomposition_eigen(benchmark::State &state)
{
    using value_type = double;
    tmech::tensor<value_type, Dim, 2> F;
    F.randn();
    for (auto _ : state)
    {
        auto Fpolar = tmech::polar_decomposition(F);
        F = Fpolar.R() * Fpolar.U();
    }
}

template <std::size_t Dim>
static void polar_decomposition_newton(benchmark::State &state)
{
    using value_type = double;
    tmech::tensor<value_type, Dim, 2> F;
    F.randn();
    for (auto _ : state)
    {
        auto Fpolar = tmech::polar_decomposition(F, true, 5e-14, 15);
        F = Fpolar.R() * Fpolar.U();
    }
}

////BENCHMARK_TEMPLATE(contractio_first, 2ul);
////BENCHMARK_TEMPLATE(contractio_first, 3ul);
////BENCHMARK_TEMPLATE(contractio_second, 2ul);
////BENCHMARK_TEMPLATE(contractio_second, 3ul);
////BENCHMARK_TEMPLATE(contractio_third, 2ul);
////BENCHMARK_TEMPLATE(contractio_fourth, 2ul);
////BENCHMARK_TEMPLATE(contractio_fourth, 3ul);

////BENCHMARK_TEMPLATE(contractio_4_4_first, 2ul);
////BENCHMARK_TEMPLATE(contractio_4_4_first, 3ul);
////BENCHMARK_TEMPLATE(contractio_4_4_fourth, 2ul);
////BENCHMARK_TEMPLATE(contractio_4_4_fourth, 3ul);

////template<std::size_t Dim>
////static void BM_exp_tmech(benchmark::State& state) {
////    using value_type = double;
////    tmech::tensor<value_type, Dim, 2> A, A_exp;

////    for (auto _ : state){
////        A_exp = tmech::exp(A);
////    }
////}

////BENCHMARK_TEMPLATE(BM_single_contraction_tmech, double, 2ul);
////BENCHMARK_TEMPLATE(BM_single_contraction_eigen, double, 2ul);
////BENCHMARK_TEMPLATE(BM_single_contraction_arma, double, 2ul);

////BENCHMARK_TEMPLATE(BM_single_contraction_tmech, double, 3ul);
////BENCHMARK_TEMPLATE(BM_single_contraction_eigen, double, 3ul);
////BENCHMARK_TEMPLATE(BM_single_contraction_arma, double, 3ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_left_tmech, double, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_left_tmech, double, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_left_tmech, float, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_left_tmech, float, 3ul, 2ul, 4ul);

////BENCHMARK_TEMPLATE(BM_general_contraction, double, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_general_contraction, double, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_general_contraction, double, 2ul, 4ul, 4ul);
////BENCHMARK_TEMPLATE(BM_general_contraction, double, 3ul, 4ul, 4ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, double, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, double, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, double, 2ul, 4ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, double, 3ul, 4ul, 4ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_io_tmech, double, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_io_tmech, double, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_io_tmech, double, 2ul, 4ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_io_tmech, double, 3ul, 4ul, 4ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_oi_tmech, double, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_oi_tmech, double, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_oi_tmech, double, 2ul, 4ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_oi_tmech, double, 3ul, 4ul, 4ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, float, 2ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, float, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, float, 2ul, 4ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech, float, 3ul, 4ul, 4ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 3, 1);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 4, 1);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 6, 1);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 9, 1);

//////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 3, 3);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 6, 6);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, double, 9, 9);
//////BENCHMARK_TEMPLATE(BM_double_contraction_eigen, float, 3, 3);

////BENCHMARK_TEMPLATE(BM_double_contraction_tmech_vector, double, 1, 2ul, 2ul, 2ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech_vector, double, 1, 3ul, 2ul, 2ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech_vector, double, 1, 2ul, 4ul, 4ul);
////BENCHMARK_TEMPLATE(BM_double_contraction_tmech_vector, double, 1, 3ul, 4ul, 4ul);

//static constexpr int size{2};

// BENCHMARK_TEMPLATE(BM_single_contraction_tmech_vector, double, size, 2ul);
// BENCHMARK_TEMPLATE(BM_single_contraction_fastor_vector, double, size, 2ul);
// BENCHMARK_TEMPLATE(BM_single_contraction_eigen_vector, double, size, 2ul);

// BENCHMARK_TEMPLATE(BM_single_contraction_tmech_vector, double, size, 3ul);
// BENCHMARK_TEMPLATE(BM_single_contraction_fastor_vector, double, size, 3ul);
// BENCHMARK_TEMPLATE(BM_single_contraction_eigen_vector, double, size, 3ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 6, 1);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 9, 1);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 1, 9);
////BENCHMARK_TEMPLATE(BM_general_contraction_vector, double, size, 3ul, 2ul, 4ul);
////BENCHMARK_TEMPLATE(BM_general_contraction_vector_left, double, size, 3ul, 2ul, 4ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 3, 1);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 4, 1);
////BENCHMARK_TEMPLATE(BM_general_contraction_vector, double, size, 2ul, 2ul, 4ul);

////BENCHMARK_TEMPLATE(polar_decomposition_eigen, 3);
////BENCHMARK_TEMPLATE(polar_decomposition_newton, 3);
////BENCHMARK_TEMPLATE(polar_decomposition_eigen, 2);
////BENCHMARK_TEMPLATE(polar_decomposition_newton, 2);

////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 2, 2);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 3, 3);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 4, 4);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 6, 6);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 9, 9);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 16, 16);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 81, 81);
////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, size, 8, 8);

////BENCHMARK_TEMPLATE(BM_general_single_contraction_vector, double, size, 2ul, 2ul, 2ul);
////BENCHMARK_TEMPLATE(BM_general_single_contraction_vector, double, size, 3ul, 2ul, 2ul);
//BENCHMARK_TEMPLATE(BM_general_contraction_vector, double, size, 2ul, 4ul, 4ul); // 2*2 x 2*2 = 4x4 matrix multiplication
//BENCHMARK_TEMPLATE(BM_general_contraction_vector, double, size, 3ul, 4ul, 4ul); // 3*3 x 3*3 = 9x9 matrix multiplication
////BENCHMARK_TEMPLATE(BM_general_fourth_contraction_vector, double, size, 2ul, 8ul, 8ul); //2*2*2*2 x 2*2*2*2 = 16x16 matrix multiplication
////BENCHMARK_TEMPLATE(BM_general_fourth_contraction_vector, double, size, 3ul, 8ul, 8ul); //3*3*3*3 x 3*3*3*3 = 81x81 matrix multiplication

////BENCHMARK_TEMPLATE(BM_general_double_contraction_array_vector, double, size, 2ul);
////BENCHMARK_TEMPLATE(BM_general_double_contraction_array_vector, double, size, 3ul);

////BENCHMARK_TEMPLATE(BM_second_general_double_contraction_array_vector, double, size, 2ul);
////BENCHMARK_TEMPLATE(BM_second_general_double_contraction_array_vector, double, size, 3ul);

//BENCHMARK_TEMPLATE(BM_general_contraction_fastor_vector, double, size, 2ul);
//BENCHMARK_TEMPLATE(BM_general_contraction_fastor_vector, double, size, 3ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_eigen_vector, double, 3, 3);
////BENCHMARK_TEMPLATE(BM_general_contraction_vector, double, 3ul, 2ul, 2ul);

////BENCHMARK_TEMPLATE(BM_double_contraction_arma_d, 3 ,1);
////BENCHMARK_TEMPLATE(BM_double_contraction_arma, 6 ,1);
////BENCHMARK_TEMPLATE(BM_double_contraction_arma, 9 ,1);

////BENCHMARK_TEMPLATE(BM_double_contraction_arma_d, 3, 3);
////BENCHMARK_TEMPLATE(BM_double_contraction_arma_d, 6, 6);
////BENCHMARK_TEMPLATE(BM_double_contraction_arma_d, 9, 9);

////BENCHMARK_TEMPLATE(BM_exp_tmech, 2);
////BENCHMARK_TEMPLATE(BM_exp_tmech, 3);
////BENCHMARK_TEMPLATE(BM_exp_eigen, 2);
////BENCHMARK_TEMPLATE(BM_exp_eigen, 3);
////BENCHMARK_TEMPLATE(BM_exp_arma, 2);
////BENCHMARK_TEMPLATE(BM_exp_arma, 3);

////BENCHMARK_TEMPLATE(BM_inverse_tmech, 2, 2);
////BENCHMARK_TEMPLATE(BM_inverse_tmech, 3, 2);
////BENCHMARK_TEMPLATE(BM_inverse_tmech, 2, 4);
////BENCHMARK_TEMPLATE(BM_inverse_tmech, 3, 4);
////BENCHMARK_TEMPLATE(BM_inverse_full_tmech, 2, 4);
////BENCHMARK_TEMPLATE(BM_inverse_full_tmech, 3, 4);

////BENCHMARK_TEMPLATE(BM_inverse_eigen, 2ul);
////BENCHMARK_TEMPLATE(BM_inverse_eigen, 3ul);
////BENCHMARK_TEMPLATE(BM_inverse_eigen, 4ul);
////BENCHMARK_TEMPLATE(BM_inverse_eigen, 6ul);
////BENCHMARK_TEMPLATE(BM_inverse_eigen, 9ul);
//////BENCHMARK_TEMPLATE(BM_inverse_arma, 3ul);
//////BENCHMARK_TEMPLATE(BM_inverse_arma, 6ul);
//////BENCHMARK_TEMPLATE(BM_inverse_arma, 9ul);

BENCHMARK_MAIN();
