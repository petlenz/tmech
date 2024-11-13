#ifndef TST_GTEST_H
#define TST_GTEST_H

#include <tmech/tmech.h>
#include "gtest/gtest.h"
#include <complex>

using namespace testing;

using complexd = std::complex<double>;
using complexf = std::complex<float>;

using tensor2 = tmech::tensor<double, 3, 2>;
using tensor4 = tmech::tensor<double, 3, 4>;


template<typename T>
static constexpr inline auto positive_value(T const value){
    return (value < T(0) ? T(0) : value);
}

template<typename T>
static constexpr inline auto negative_value(T const value){
    return (value > T(0) ? T(0) : value);
}


//dot product
#define dotProduct(ValueType, Dim)  \
TEST(gtest, dotProduct_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 1> a, b; \
    a.randn(); \
    b.randn(); \
    ValueType sum{0};\
    for(std::size_t i{0}; i<Dim; ++i){sum += a(i)*b(i);}\
    EXPECT_EQ(true, (sum == tmech::dot(a,b))); \
}

//compare equal operator
#define compareEqualOperator(ValueType, Dim, Rank)  \
TEST(gtest, compareEqualOperator_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, (a==b)); \
}

#define compareEqualOperatorEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, compareEqualOperatorEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, (a+a==b*2)); \
    EXPECT_EQ(true, (2*a==b+b)); \
    EXPECT_EQ(true, (a+a==b+b)); \
}

#define compareEqualOperatorFunction(ValueType, Dim, Rank, Function)  \
TEST(gtest, compareEqualOperatorFunction_##ValueType##_##Dim##_##Rank##_##Function){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.randn(); \
    b = a; \
    EXPECT_EQ(true, (tmech::Function(a)==tmech::Function(b))); \
}

//compare not equal operator
#define compareNoEqualOperator(ValueType, Dim, Rank)  \
TEST(gtest, compareNoEqualOperator_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, !(a!=b)); \
}

#define compareNoEqualOperatorEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, compareNoEqualOperatorEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, !(a+a!=b*2)); \
    EXPECT_EQ(true, !(2*a!=b+b)); \
    EXPECT_EQ(true, !(a+a!=b+b)); \
}

//compare greater operator
#define compareGreaterOperator(ValueType, Dim, Rank)  \
TEST(gtest, compareGreaterOperator_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(2)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, (a>b)); \
}

#define compareGreaterOperatorEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, compareGraterOperatorEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(2)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, (a+a>b*2)); \
    EXPECT_EQ(true, (2*a>b+b)); \
    EXPECT_EQ(true, (a+a>b+b)); \
}

//compare less operator
#define compareLessOperator(ValueType, Dim, Rank)  \
TEST(gtest, compareLessOperator_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(2)); \
    EXPECT_EQ(true, (a<b)); \
}

#define compareLessOperatorEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, compareLessOperatorEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(2)); \
    EXPECT_EQ(true, (a+a<b*2)); \
    EXPECT_EQ(true, (2*a<b+b)); \
    EXPECT_EQ(true, (a+a<b+b)); \
}

//compare greater equal operator
#define compareGreaterEqualOperator(ValueType, Dim, Rank)  \
TEST(gtest, compareGreaterEqualOperator_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(2)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, (a>=b)); \
}

#define compareGreaterEqualOperatorEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, compareGreaterEqualOperatorEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(2)); \
    b.fill(static_cast<ValueType>(1)); \
    EXPECT_EQ(true, (a+a>=b*2)); \
    EXPECT_EQ(true, (2*a>=b+b)); \
    EXPECT_EQ(true, (a+a>=b+b)); \
}


//compare less equal operator
#define compareLessEqualOperator(ValueType, Dim, Rank)  \
TEST(gtest, compareLessEqualOperator_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(2)); \
    EXPECT_EQ(true, (a<=b)); \
}

#define compareLessEqualOperatorEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, compareLessEqualOperatorEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(1)); \
    b.fill(static_cast<ValueType>(2)); \
    EXPECT_EQ(true, (a+a<=b*2)); \
    EXPECT_EQ(true, (2*a<=b+b)); \
    EXPECT_EQ(true, (a+a<=b+b)); \
}

//inv function
#define inv_function_2(ValueType, Dim)  \
TEST(gtest, inv_function_2_##ValueType##_##Dim##_){ \
    tmech::tensor<ValueType, Dim, 2> a; \
    a.randn();\
    EXPECT_EQ(true, tmech::almost_equal(tmech::inv(a)*a, tmech::eye<ValueType, Dim, 2>(), 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inv(tmech::abs(a))*tmech::abs(a), tmech::eye<ValueType, Dim, 2>(), 5e-5)); \
}

#define inv_function_4(ValueType, Dim)  \
TEST(gtest, inv_function_4_##ValueType##_##Dim##_){ \
    using SeqL = tmech::sequence<1,4>;\
    using SeqR = tmech::sequence<2,3>;\
    tmech::tensor<ValueType, Dim, 4> A; \
    ValueType mu{200}, lambda{600}; \
    const auto I{tmech::eye<ValueType,Dim,2>()};\
    const auto IIsym{(tmech::otimesu(I,I) + tmech::otimesl(I,I))*0.5};\
    A = 2*mu*IIsym + lambda*tmech::otimes(I,I);\
    const auto Anew{tmech::basis_change<tmech::sequence<1,3,4,2>>(A)}; \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::inv(A),A), IIsym, 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::inv(tmech::abs(A)),tmech::abs(A)), IIsym, 5e-5)); \
EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::basis_change<tmech::sequence<1,4,2,3>>(tmech::inv(Anew, SeqL(), SeqR())),A), IIsym, 5e-5)); \
EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::basis_change<tmech::sequence<1,4,2,3>>(tmech::inv(tmech::abs(Anew), SeqL(), SeqR())),tmech::abs(A)), IIsym, 5e-5)); \
}

#define inv_full_function_4(ValueType, Dim)  \
TEST(gtest, inv_full_function_4_##ValueType##_##Dim##_){ \
    tmech::tensor<ValueType, Dim, 4> A; \
    A.randn();\
    const auto I{tmech::eye<ValueType, Dim, 2>()};\
    const auto II{tmech::otimesu(I,I)};\
    const auto Anew{tmech::basis_change<tmech::sequence<1,3,4,2>>(A)}; \
    constexpr ValueType eps{(std::is_same_v<ValueType,float> ? 5e-3 : 8e-5)}; \
    std::cout<<tmech::dcontract(tmech::invf(A),A) - II<<std::endl; \
    std::cout<<tmech::dcontract(tmech::invf(tmech::abs(A)),tmech::abs(A)) - II<<std::endl; \
    std::cout<<tmech::dcontract(tmech::basis_change<tmech::sequence<1,4,2,3>>(tmech::invf(Anew, tmech::sequence<1,4,2,3>())),A) - II<<std::endl; \
    std::cout<<tmech::dcontract(tmech::basis_change<tmech::sequence<1,4,2,3>>(tmech::invf(tmech::abs(Anew), tmech::sequence<1,4,2,3>())),tmech::abs(A)) - II<<std::endl; \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::invf(A),A), II, eps)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::invf(tmech::abs(A)),tmech::abs(A)), II, eps)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::basis_change<tmech::sequence<1,4,2,3>>(tmech::invf(Anew, tmech::sequence<1,4,2,3>())),A), II, eps)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dcontract(tmech::basis_change<tmech::sequence<1,4,2,3>>(tmech::invf(tmech::abs(Anew), tmech::sequence<1,4,2,3>())),tmech::abs(A)), II, eps)); \
}

//abs function
#define absFunction(ValueType, Dim, Rank)  \
TEST(gtest, absFunc_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(-10)); \
    b.fill(static_cast<ValueType>(10)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::abs(a), b, 5e-6)); \
}
#define absFunctionEvaluate(ValueType, Dim, Rank)  \
TEST(gtest, absFuncEvaluate_##ValueType##_##Dim##_##Rank){ \
    tmech::tensor<ValueType, Dim, Rank> a, b; \
    a.fill(static_cast<ValueType>(-10)); \
    b.fill(static_cast<ValueType>(10)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::abs(a + a), (b + b), 5e-6)); \
}

//adjoint function
#define adjFunction(ValueType, Dim)  \
TEST(gtest, adjFunc_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> a; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::adj(a), tmech::det(a)*tmech::inv(a), 5e-7)); \
}

#define adjFunctionEvaluate(ValueType, Dim)  \
TEST(gtest, adjFuncEvaluate_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> a; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::adj(a+a), tmech::det(a+a)*tmech::inv(a+a), 5e-7)); \
}

//cofactor function
#define cofFunction(ValueType, Dim)  \
TEST(gtest, cofFunc_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> a; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cof(a), tmech::trans(tmech::adj(a)), 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cof(tmech::abs(a)), tmech::trans(tmech::adj(tmech::abs(a))), 5e-6)); \
}

#define cofFunctionEvaluate(ValueType, Dim)  \
TEST(gtest, cofFuncEvaluate_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> a; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cof(a+a), tmech::trans(tmech::adj(a+a)), 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cof(tmech::abs(a+a)), tmech::trans(tmech::adj(tmech::abs(a+a))), 5e-6)); \
}

//basis change second order
#define basisChange2(ValueType, Dim)  \
TEST(gtest, basisChange2_##ValueType##_##Dim){ \
    using Seq = tmech::sequence<2,1>;\
    tmech::tensor<ValueType, Dim, 2> a, b; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    b(j,i) = a(i,j);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::basis_change<Seq>(a), b, 5e-6)); \
}
#define basisChangeEvaluate2(ValueType, Dim)  \
TEST(gtest, basisChangeEvaluate2_##ValueType##_##Dim){ \
    using Seq = tmech::sequence<2,1>;\
    tmech::tensor<ValueType, Dim, 2> a, b; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    b(j,i) = static_cast<ValueType>(2.0)*a(i,j);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::basis_change<Seq>(a+a), b, 5e-6)); \
}

//basis change fourth order
#define basisChange4(ValueType, Dim)  \
TEST(gtest, basisChange4_##ValueType##_##Dim){ \
    using Seq = tmech::sequence<2,1,4,3>;\
    tmech::tensor<ValueType, Dim, 4> a, b; \
    a = tmech::randn<ValueType, Dim, 4>(); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    b(i,j,k,l) = a(j,i,l,k);\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::basis_change<Seq>(a), b, 5e-6)); \
}
#define basisChangeEvaluate4(ValueType, Dim)  \
TEST(gtest, basisChangeEvaluate4_##ValueType##_##Dim){ \
    using Seq = tmech::sequence<2,1,4,3>;\
    tmech::tensor<ValueType, Dim, 4> a, b; \
    a = tmech::randn<ValueType, Dim, 4>(); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    b(i,j,k,l) = static_cast<ValueType>(2.0)*a(j,i,l,k);\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::basis_change<Seq>(a+a), b, 5e-6)); \
}

//inner product test
#define inner_product2(ValueType, Dim)  \
TEST(gtest, inner_product2_##ValueType##_##Dim){ \
    using SeqL = tmech::sequence<2>;\
    using SeqR = tmech::sequence<1>;\
    tmech::tensor<ValueType, Dim, 2> a, b, c; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 2>()); \
    b = tmech::abs(tmech::randn<ValueType, Dim, 2>()); \
    c.fill(0);\
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    c(i,j) += a(i,k)*b(k,j);\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqL, SeqR>(a, b), c, 5e-7)); \
    c.fill(0);\
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    c(i,j) += a(k,i)*b(j,k);\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(a, b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(tmech::abs(a), b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(a, tmech::abs(b)), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(tmech::abs(a), tmech::abs(b)), c, 5e-7)); \
}

#define inner_product4(ValueType, Dim)  \
TEST(gtest, inner_product4_##ValueType##_##Dim){ \
    using SeqL = tmech::sequence<3,4>;\
    using SeqR = tmech::sequence<1,2>;\
    tmech::tensor<ValueType, Dim, 4> a, b, c; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 4>()); \
    b = tmech::abs(tmech::randn<ValueType, Dim, 4>()); \
    c.fill(0);\
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    for(std::size_t m{0}; m<Dim; ++m){\
    for(std::size_t n{0}; n<Dim; ++n){\
    c(i,j,k,l) += a(i,j,m,n)*b(m,n,k,l); \
    }\
    }\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqL, SeqR>(a, b), c, 5e-7)); \
    c.fill(0);\
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    for(std::size_t m{0}; m<Dim; ++m){\
    for(std::size_t n{0}; n<Dim; ++n){\
    c(i,j,k,l) += a(m,n,i,j)*b(k,l,m,n); \
    }\
    }\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(a, b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(tmech::abs(a), b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(a, tmech::abs(b)), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::inner_product<SeqR, SeqL>(tmech::abs(a), tmech::abs(b)), c, 5e-7)); \
}

//inner product test
#define outer_product2(ValueType, Dim)  \
TEST(gtest, outer_product2_##ValueType##_##Dim){ \
    using SeqL = tmech::sequence<2,4>;\
    using SeqR = tmech::sequence<1,3>;\
    tmech::tensor<ValueType, Dim, 2> a, b;\
    tmech::tensor<ValueType, Dim, 4> c; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 2>()); \
    b = tmech::abs(tmech::randn<ValueType, Dim, 2>()); \
    c.fill(0);\
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    c(i,j,k,l) = a(j,l)*b(i,k);\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(a, b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(a, b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(tmech::abs(a), b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(a, tmech::abs(b)), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(tmech::abs(a), tmech::abs(b)), c, 5e-7)); \
}


#define outer_product4(ValueType, Dim)  \
TEST(gtest, outer_product4_##ValueType##_##Dim){ \
    using SeqL = tmech::sequence<7,8,3,6>;\
    using SeqR = tmech::sequence<5,4,1,2>;\
    tmech::tensor<ValueType, Dim, 4> a, b; \
    tmech::tensor<ValueType, Dim, 8> c; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 4>()); \
    b = tmech::abs(tmech::randn<ValueType, Dim, 4>()); \
    c.fill(0);\
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    for(std::size_t m{0}; m<Dim; ++m){\
    for(std::size_t n{0}; n<Dim; ++n){\
    for(std::size_t r{0}; r<Dim; ++r){\
    for(std::size_t q{0}; q<Dim; ++q){\
    c(i,j,k,l,m,n,r,q) = a(r,q,k,n)*b(m,l,i,j);\
    }\
    }\
    }\
    }\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(a, b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(a, b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(tmech::abs(a), b), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(a, tmech::abs(b)), c, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::outer_product<SeqL, SeqR>(tmech::abs(a), tmech::abs(b)), c, 5e-7)); \
}

//trace of a second oder tensor
#define Trace(ValueType, Dim)  \
TEST(gtest, trace_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> a; \
    a = tmech::randn<ValueType, Dim, 2>(); \
    ValueType sum{0};\
    for(std::size_t i{0}; i<Dim; ++i){\
    sum += a(i,i);\
    }\
    EXPECT_EQ(tmech::trace(a), sum); \
}

//cross product
#define crossProduct(ValueType, Dim)  \
TEST(gtest, crossProduct_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 1> a, b; \
    a = tmech::randn<ValueType, Dim, 1>(); \
    b = tmech::randn<ValueType, Dim, 1>();\
    tmech::levi_civita<ValueType, Dim> levi;\
    EXPECT_EQ(true, tmech::almost_equal(tmech::cross(a,b), a*levi*b, 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cross(tmech::abs(a),b), tmech::abs(a)*levi*b, 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cross(a,tmech::abs(b)), a*levi*tmech::abs(b), 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::cross(tmech::abs(a),tmech::abs(b)), tmech::abs(a)*levi*tmech::abs(b), 5e-6)); \
}

//transposition_second_order_tensors
#define transposition_second_order_tensors(ValueType, Dim)  \
TEST(gtest, transposition_second_order_tensors_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> a, b; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 2>()); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    b(i,j) = a(j,i);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::trans(a), b, 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::trans(tmech::abs(a)), b, 5e-6)); \
}

//transposition_fourth_order_tensors
#define transposition_fourth_order_tensors(ValueType, Dim)  \
TEST(gtest, transposition_fourth_order_tensors_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 4> a, b; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 4>()); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    b(i,j,k,l) = a(k,l,i,j);\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::trans(a), b, 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::trans(tmech::abs(a)), b, 5e-6)); \
}

//left transposition_fourth_order_tensors
#define left_transposition_fourth_order_tensors(ValueType, Dim)  \
TEST(gtest, left_transposition_fourth_order_tensors_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 4> a, b; \
    a = tmech::abs(tmech::randn<ValueType, Dim, 4>()); \
    for(std::size_t i{0}; i<Dim; ++i){\
    for(std::size_t j{0}; j<Dim; ++j){\
    for(std::size_t k{0}; k<Dim; ++k){\
    for(std::size_t l{0}; l<Dim; ++l){\
    b(i,j,k,l) = a(j,i,k,l);\
    }\
    }\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::transl(a), b, 5e-6)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::transl(tmech::abs(a)), b, 5e-6)); \
}

//DoubleContractionLeft
#define DoubleContractionLeft(ValueType, Dim, RankLHS, RankRHS) \
    TEST(gtest, DoubleContractionLeft_##ValueType##_##Dim##_##RankLHS##_##RankRHS) \
{ \
    tmech::tensor<ValueType, Dim, RankLHS> B, C1, C2; \
    tmech::tensor<ValueType, Dim, RankRHS> A; \
    A.randn(); \
    B.randn(); \
    C1 = tmech::dcontract(B, A); \
    tmech::detail::double_contraction::apply(C2, B, A); \
    EXPECT_EQ(true, tmech::almost_equal(C1, C2, 5e-6)); \
    }

//DoubleContractionRight
#define DoubleContractionRight(ValueType, Dim, RankLHS, RankRHS) \
    TEST(gtest, DoubleContractionRight_##ValueType##_##Dim##_##RankLHS##_##RankRHS) \
{ \
    tmech::tensor<ValueType, Dim, RankLHS> B, C1, C2; \
    tmech::tensor<ValueType, Dim, RankRHS> A; \
    A.randn(); \
    B.randn(); \
    C1 = tmech::dcontract(A, B); \
    tmech::detail::double_contraction::apply(C2, A, B); \
    EXPECT_EQ(true, tmech::almost_equal(C1, C2, 5e-6)); \
    }

//SingleContraction
#define SingleContraction(ValueType, Dim, RankLHS, RankRHS) \
    TEST(gtest, SingleContraction_##ValueType##_##Dim##_##RankLHS##_##RankRHS) \
{ \
    tmech::tensor<ValueType, Dim, RankLHS> B, C1, C2; \
    tmech::tensor<ValueType, Dim, RankRHS> A; \
    A.randn(); \
    B.randn(); \
    C1 = A*B; \
    tmech::detail::single_contraction::apply(C2, A, B); \
    EXPECT_EQ(true, tmech::almost_equal(C1, C2, 5e-6)); \
    }

// sign tensor decomposition
#define signDecomposition(ValueType, Dim) \
    TEST(gtest, signDecomposition_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A = tmech::sym(tmech::randn<ValueType, Dim, 2>());\
    auto signA{tmech::sign(A, 5e-12, 15)};\
    EXPECT_EQ(true, tmech::almost_equal(signA.N(), tmech::sqrt(A*A), 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(signA.S()*signA.N(), A, 5e-5)); \
}

#define signDecompositionEvaluate(ValueType, Dim) \
    TEST(gtest, signDecompositionEvaluate_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A = tmech::sym(tmech::randn<ValueType, Dim, 2>());\
    auto signA{tmech::sign(A*2, 5e-12, 15)};\
    EXPECT_EQ(true, tmech::almost_equal(signA.N(), 2*tmech::sqrt(A*A), 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(signA.S()*signA.N(), 2*A, 5e-6)); \
}

//positive and negavtive decomposition
#define positive_and_negavtive_decomposition(ValueType, Dim) \
    TEST(gtest, positive_and_negavtive_decomposition_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A = tmech::sym(tmech::randn<ValueType, Dim, 2>()); \
    auto A_pos_neg = tmech::positive_negative_decomposition(A); \
    EXPECT_EQ(true, tmech::almost_equal(A, A_pos_neg.negative()+A_pos_neg.positive(), 5e-7)); \
}


//Symmetric eigenvalue problem
#define symmetric_eigenvalue_problem(ValueType, TensorFunction, LambdaFunction) \
    TEST(gtest, SymmetricEigenvalueProblem_##TensorFunction##_##ValueType##_3) \
{ \
    tmech::tensor<ValueType, 3, 2> A{2.0306, 0.162, 2.0208, 0.162, 1.245, 0, 2.0208, 0, 2.561}; \
    tmech::tensor<ValueType, 3, 1> Vec1{7.482647933407099e-01,-1.209568822617077e-01,-6.522800255107561e-01};\
    tmech::tensor<ValueType, 3, 1> Vec2{-6.824020025926589e-02,-9.920557437763156e-01,1.056819582946706e-01,};\
    tmech::tensor<ValueType, 3, 1> Vec3{6.598811060451425e-01,3.456636911723016e-02,7.505745079675242e-01};\
    ValueType lambda1{2.428338209898600e-01}, lambda2{1.256143438774842e+00}, lambda3{4.337622740235298e+00};\
    tmech::tensor<ValueType, 3, 2> B = LambdaFunction(lambda1)*tmech::otimes(Vec1, Vec1) + LambdaFunction(lambda2)*tmech::otimes(Vec2, Vec2) + LambdaFunction(lambda3)*tmech::otimes(Vec3, Vec3); \
    EXPECT_EQ(true, tmech::almost_equal(B, tmech::TensorFunction(A), 5e-5)); \
}

//Symmetric part second order tensor
#define symmetricPart(ValueType, Dim) \
    TEST(gtest, symmetricPart_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A, B; \
    A.randn(); \
    for(int i{0}; i<Dim; ++i){\
    for(int j{0}; j<Dim; ++j){\
    B(i,j) = (A(i,j)+A(j,i))*static_cast<ValueType>(0.5);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::sym(A), B, 5e-7)); \
}

#define symmetricPartEvaluate(ValueType, Dim) \
    TEST(gtest, symmetricPartEvaluate_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A, B; \
    A.randn(); \
    const ValueType a{2}; \
    for(int i{0}; i<Dim; ++i){\
    for(int j{0}; j<Dim; ++j){\
    B(i,j) = (a*A(i,j)+a*A(j,i))*static_cast<ValueType>(0.5);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::sym(a*A), B, 5e-7)); \
}

//pow second order tensor function
#define powFunction(ValueType, Dim) \
    TEST(gtest, powFunction_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A.randn(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A, 1), A, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A, 2), A*A, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A, 3), A*A*A, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A, 4), A*A*A*A, 5e-7)); \
}

#define powFunctionEvaluate(ValueType, Dim) \
    TEST(gtest, powFunctionEvaluate_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A.randn(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A+A, 1), A+A, 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A+A, 2), (A+A)*(A+A), 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A+A, 3), (A+A)*(A+A)*(A+A), 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::pow(A+A, 4), (A+A)*(A+A)*(A+A)*(A+A), 5e-7)); \
}

//skew Symmetric part second order tensor
#define skewSymmetricPart(ValueType, Dim) \
    TEST(gtest, skewSymmetricPart_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A, B; \
    A.randn(); \
    for(int i{0}; i<Dim; ++i){\
    for(int j{0}; j<Dim; ++j){\
    B(i,j) = (A(i,j)-A(j,i))*static_cast<ValueType>(0.5);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::skew(A), B, 5e-7)); \
}

#define skewSymmetricPartEvaluate(ValueType, Dim) \
    TEST(gtest, skewSymmetricPartEvaluate_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A, B; \
    A.randn(); \
    const ValueType a{2}; \
    for(int i{0}; i<Dim; ++i){\
    for(int j{0}; j<Dim; ++j){\
    B(i,j) = (a*A(i,j)+a*A(j,i))*static_cast<ValueType>(0.5);\
    }\
    }\
    EXPECT_EQ(true, tmech::almost_equal(tmech::sym(a*A), B, 5e-7)); \
}

//deviatoric part second order tensor
#define deviatoricPart(ValueType, Dim) \
    TEST(gtest, deviatoricPart_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A.randn(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dev(A), A-tmech::vol(A), 5e-7)); \
}

#define deviatoricPartEvaluate(ValueType, Dim) \
    TEST(gtest, deviatoricPartEvaluate_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A; \
    A.randn(); \
    const ValueType a{2}; \
    EXPECT_EQ(true, tmech::almost_equal(tmech::dev(a*A), a*A-a*tmech::vol(A), 5e-7)); \
}

//volumetric part second order tensor
#define volumetricPart(ValueType, Dim) \
    TEST(gtest, volumetricPart_##ValueType##_##Dim) \
{ \
    tmech::tensor<ValueType, Dim, 2> A, B; \
    A.randn(); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::vol(A), A-tmech::dev(A), 5e-7)); \
    EXPECT_EQ(true, tmech::almost_equal(tmech::vol(tmech::abs(A)), tmech::abs(A)-tmech::dev(tmech::abs(A)), 5e-7)); \
}

//derivatives polar decomposition eigen
#define derivatives_polar_decomposition_eigen(ValueType, Dim) \
TEST(gtest, derivatives_polar_decomposition_eigen_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> F; \
    F.randn(); \
    auto Fpolar = tmech::polar_decomposition(F); \
    auto lambda_R = [](auto const& t){return tmech::eval(tmech::polar_decomposition(t).R());}; \
    auto lambda_U = [](auto const& t){return tmech::eval(tmech::polar_decomposition(t).U());}; \
    auto lambda_V = [](auto const& t){return tmech::eval(tmech::polar_decomposition(t).V());}; \
    auto F_R_numdif = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lambda_R, F); \
    auto F_U_numdif = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lambda_U, F); \
    auto F_V_numdif = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lambda_V, F); \
    EXPECT_EQ(true, tmech::almost_equal(Fpolar.R().derivative(), F_R_numdif, 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(Fpolar.V().derivative(), F_V_numdif, 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(Fpolar.U().derivative(), F_U_numdif, 5e-5)); \
}

//derivatives polar decomposition newton
#define derivatives_polar_decomposition_newton(ValueType, Dim) \
TEST(gtest, derivatives_polar_decomposition_newton_##ValueType##_##Dim){ \
    tmech::tensor<ValueType, Dim, 2> F; \
    F.randn(); \
    auto Fpolar = tmech::polar_decomposition(F, true, 5e-12, 50); \
    auto lambda_R = [](auto const& t){return tmech::eval(tmech::polar_decomposition(t, true, 5e-12, 50).R());}; \
    auto lambda_U = [](auto const& t){return tmech::eval(tmech::polar_decomposition(t, true, 5e-12, 50).U());}; \
    auto lambda_V = [](auto const& t){return tmech::eval(tmech::polar_decomposition(t, true, 5e-12, 50).V());}; \
    auto F_R_numdif = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lambda_R, F); \
    auto F_U_numdif = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lambda_U, F); \
    auto F_V_numdif = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lambda_V, F); \
    EXPECT_EQ(true, tmech::almost_equal(Fpolar.R().derivative(), F_R_numdif, 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(Fpolar.V().derivative(), F_V_numdif, 5e-5)); \
    EXPECT_EQ(true, tmech::almost_equal(Fpolar.U().derivative(), F_U_numdif, 5e-5)); \
}

#define productRuleScalar(ValueType)  \
TEST(gtest, productRuleScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{x*x}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(df(f_var), ValueType(10)); \
}

#define sumRuleScalar(ValueType)  \
TEST(gtest, sumRuleScalar_##ValueType)  \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{x+x}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(df(f_var), ValueType(2)); \
}

#define minRuleScalar(ValueType)  \
TEST(gtest, minRuleScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{x-x}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(df(f_var), ValueType(0)); \
}

#define expVarScalar(ValueType)  \
TEST(gtest, expVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::exp(x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true , std::abs(df(f_var) - std::exp(std::get<0>(f_var))) < 5e-5); \
}

#define expFuncScalar(ValueType)  \
TEST(gtest, expFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::exp(x*x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - ValueType(2)*std::get<0>(f_var)*std::exp(std::get<0>(f_var)*std::get<0>(f_var))) < 5e-5); \
}

#define logVarScalar(ValueType)  \
TEST(gtest, logVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::log(x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - ValueType(1)/std::get<0>(f_var)) < 5e-5); \
}

#define logFuncScalar(ValueType)  \
TEST(gtest, logFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::log(x*x+x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (((ValueType(2)*std::get<0>(f_var))+ValueType(1))/((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))) < 5e-5); \
}

#define sqrtVarScalar(ValueType)  \
TEST(gtest, sqrtVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::sqrt(x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (ValueType(1)/(ValueType(2)*sqrt(std::get<0>(f_var))))) < 5e-5); \
}

#define sqrtFuncScalar(ValueType)  \
TEST(gtest, sqrtFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::sqrt(x*x+x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (((ValueType(2)*std::get<0>(f_var))+ValueType(1))/(ValueType(2)*sqrt(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))))) < 5e-5); \
}

#define absVarScalar(ValueType)  \
TEST(gtest, absVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::abs(x)}; \
    auto f_var{std::make_tuple(-ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - std::get<0>(f_var)/std::abs(std::get<0>(f_var))) < 5e-5); \
}

#define absFuncScalar(ValueType)  \
TEST(gtest, absFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::abs(x*x+x)}; \
    auto f_var{std::make_tuple(-ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - ((((ValueType(2)*std::get<0>(f_var))+ValueType(1))*((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))/abs(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))) < 5e-5); \
}

#define cosVarScalar(ValueType)  \
TEST(gtest, cosVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::cos(x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (-std::sin(std::get<0>(f_var)))) < 5e-5); \
}

#define cosFuncScalar(ValueType)  \
TEST(gtest, cosFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::cos(x*x+x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (-((((ValueType(2)*std::get<0>(f_var))+ValueType(1))*sin(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))))) < 5e-5); \
}

#define sinVarScalar(ValueType)  \
TEST(gtest, sinVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::sin(x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - std::cos(std::get<0>(f_var))) < 5e-5); \
}

#define sinFuncScalar(ValueType)  \
TEST(gtest, sinFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::sin(x*x+x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - ((((ValueType(2)*std::get<0>(f_var))+ValueType(1))*cos(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))))) < 5e-5); \
}

#define tanVarScalar(ValueType)  \
TEST(gtest, tanVarScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::tan(x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - std::pow(ValueType(1)/std::cos(std::get<0>(f_var)), ValueType(2))) < 5e-5); \
}

#define tanFuncScalar(ValueType)  \
TEST(gtest, tanFuncScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::tan(x*x+x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (((ValueType(2)*std::get<0>(f_var))+1.)*std::pow(ValueType(1)/std::cos(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))), ValueType(2)))) < 5e-5); \
}

#define powVaraScalar(ValueType)  \
TEST(gtest, powVaraScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    symdiff::constant<ValueType, 1> c(2,"c"); \
    auto f{std::pow(x, c)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - ValueType(2)*std::get<0>(f_var)) < 5e-5); \
}

#define powFuncaScalar(ValueType)  \
TEST(gtest, powFuncaScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    symdiff::constant<ValueType, 1> c; \
    auto f{std::pow(x*x+x, c)}; \
    auto f_var{std::make_tuple(-ValueType(0.5), ValueType(2))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (ValueType(2)*((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))*((ValueType(2)*std::get<0>(f_var))+ValueType(1))) < 5e-5); \
}

#define powVarbScalar(ValueType)  \
TEST(gtest, powVarbScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    symdiff::constant<ValueType, 1> c(2,"c"); \
    auto f{std::pow(c, x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - std::pow(ValueType(2),std::get<0>(f_var))*std::log(ValueType(2))) < 5e-5); \
}


#define powFuncbScalar(ValueType)  \
TEST(gtest, powFuncbScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    symdiff::constant<ValueType, 1> c(2,"c"); \
    auto f{std::pow(c, x*x+x)}; \
    auto f_var{std::make_tuple(-ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (std::pow(ValueType(2), std::get<0>(f_var)*std::get<0>(f_var)+std::get<0>(f_var))*((((ValueType(2)*std::get<0>(f_var))+ValueType(1))*log(ValueType(2)))))) < 5e-5); \
}

#define powVarcScalar(ValueType)  \
TEST(gtest, powVarcScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::pow(x, x)}; \
    auto f_var{std::make_tuple(ValueType(5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - std::pow(std::get<0>(f_var), (std::get<0>(f_var)-ValueType(1)))*((std::log(std::get<0>(f_var))*std::get<0>(f_var))+std::get<0>(f_var))) < 5e-5); \
}

#define powFunccScalar(ValueType)  \
TEST(gtest, powFunccScalar_##ValueType) \
{ \
    symdiff::variable<ValueType, 0> x; \
    auto f{std::pow(x*x+x, x*x+x)}; \
    auto f_var{std::make_tuple(ValueType(0.5))}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(true, std::abs(df(f_var) - (std::pow(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)), (((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))-ValueType(1)))*(((((ValueType(2)*std::get<0>(f_var))+ValueType(1))*std::log(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))*((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))+(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))*((ValueType(2)*std::get<0>(f_var))+ValueType(1)))))) < 5e-5); \
}


#define productRuleTensor(ValueType, Dim, Rank)  \
TEST(gtest, productRuleTensor_##ValueType##_##Dim##_##Rank) \
{ \
    tmech::tensor<ValueType, Dim, Rank> a; \
    symdiff::variable<tmech::tensor<ValueType, Dim, Rank>, 0> x; \
    a.randn(); \
    tmech::eye<ValueType, Dim, 2> I;\
    auto f{x*x}; \
    auto f_var{std::make_tuple(a)}; \
    auto df = symdiff::derivative<1>(f, x); \
    EXPECT_EQ(df(f_var), (tmech::basis_change<tmech::sequence<1,3,4,2>>(tmech::inner_product<tmech::sequence<2>,tmech::sequence<1>>(tmech::otimesu(I, I), x(f_var))) \
                          +tmech::inner_product<tmech::sequence<2>,tmech::sequence<1>>(x(f_var), tmech::otimesu(I, I)))); \
}


#define addRuleTensor(ValueType, Dim, Rank)  \
TEST(gtest, addRuleTensor_##ValueType##_##Dim##_##Rank) \
{ \
    tmech::tensor<ValueType, Dim, Rank> a; \
    symdiff::variable<tmech::tensor<ValueType, Dim, Rank>, 0> x; \
    a.randn(); \
    auto f{x+x}; \
    auto f_var{std::make_tuple(a)}; \
    auto df = symdiff::derivative<1>(f, x); \
    auto lamb = [](auto const& T){return T+T;};\
    auto df_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lamb, a);\
    EXPECT_EQ(true, tmech::almost_equal(df(f_var), df_num, 5e-7)); \
}

#define minRuleTensor(ValueType, Dim, Rank)  \
TEST(gtest, minRuleTensor_##ValueType##_##Dim##_##Rank) \
{ \
    tmech::tensor<ValueType, Dim, Rank> a; \
    symdiff::variable<tmech::tensor<ValueType, Dim, Rank>, 0> x; \
    a.randn(); \
    auto f{x-x}; \
    auto f_var{std::make_tuple(a)}; \
    auto df = symdiff::derivative<1>(f, x); \
    auto lamb = [](auto const& T){return T-T;};\
    auto df_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lamb, a);\
    EXPECT_EQ(true, tmech::almost_equal(df(f_var), df_num, 5e-7)); \
}

#define lambdaTensor(ValueType, Dim, Rank, Lambda)  \
TEST(gtest, lambdaTensor_##ValueType##_##Dim##_##Rank##_##Lambda) \
{ \
    tmech::tensor<ValueType, Dim, Rank> a; \
    symdiff::variable<tmech::tensor<ValueType, Dim, Rank>, 0> x; \
    a.randn(); \
    auto f{tmech::Lambda(x)}; \
    auto f_var{std::make_tuple(a)}; \
    auto df = symdiff::derivative<1>(f, x); \
    auto lamb = [](auto const& x){return tmech::Lambda(x);};\
    auto df_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(lamb, a);\
    EXPECT_EQ(true, tmech::almost_equal(df(f_var), df_num, 5e-7)); \
}

#define lambdaTensorSym_3(ValueType, Rank, Lambda)  \
TEST(gtest, lambdaTensorSym_3_##ValueType##_##Rank##_##Lambda) \
{ \
    using Sym2x2 = std::tuple<tmech::sequence<1,2>,tmech::sequence<2,1>>; \
    using Sym4x4 = std::tuple< \
    tmech::sequence<1,2,3,4>, \
    tmech::sequence<2,1,3,4>, \
    tmech::sequence<1,2,4,3>, \
    tmech::sequence<2,1,4,3>>; \
    symdiff::variable<tmech::tensor<ValueType, 3, Rank>, 0> x; \
    tmech::tensor<ValueType, 3, Rank> a{2.0306, 0.162, 2.0208, 0.162, 1.245, 0, 2.0208, 0, 2.561}; \
    auto f{tmech::Lambda(x)}; \
    auto f_var{std::make_tuple(a)}; \
    auto df = symdiff::derivative<1>(f, x); \
    auto lamb = [](auto const& x){return tmech::Lambda(x);};\
    auto df_num = tmech::num_diff_sym_central<Sym2x2, Sym4x4>(lamb, a);\
    EXPECT_EQ(true, tmech::almost_equal(df(f_var), df_num, 5e-6)); \
}

//Adaptor tensor
TEST(gtest, adaptorVoigt2Dsecond){
    //11 22 12
    double ptr[3]{1,2,3};
    tmech::adaptor<double, 2, 2, tmech::voigt<2>> a(ptr);
    tmech::tensor<double, 2, 2> b{1,3, 3,2};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorVoigt3Dsecond){
    //11 22 33 23 13 12
    double ptr[6]{1,2,3,4,5,6};
    tmech::adaptor<double, 3, 2, tmech::voigt<3>> a(ptr);
    tmech::tensor<double, 3, 2> b{1,6,5, 6,2,4, 5,4,3};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}


TEST(gtest, adaptorVoigt2DsecondShear){
    //11 22 12*2
    double ptr[3]{1,2,3*2};
    tmech::adaptor<double, 2, 2, tmech::voigt<2, true>> a(ptr);
    tmech::tensor<double, 2, 2> b{1,3, 3,2};
    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorVoigt3DsecondShear){
    //11 22 33 23*2 13*2 12*2
    double ptr[6]{1,2,3,4*2,5*2,6*2};
    tmech::adaptor<double, 3, 2, tmech::voigt<3, true>> a(ptr);
    tmech::tensor<double, 3, 2> b{1,6,5, 6,2,4, 5,4,3};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorVoigt2Dfourth){
    //1111 1122 1112
    //2211 2222 2212
    //1211 1222 1212
    double ptr[9]{1111,1122,1112,
                  2211,2222,2212,
                  1211,1222,1212};
    tmech::adaptor<double, 2, 4, tmech::voigt<2>> a(ptr);
    tmech::tensor<double, 2, 4> b{1111, 1112,
                                  1112, 1122,
                                  1211, 1212,
                                  1212, 1222,
                                  1211, 1212,
                                  1212, 1222,
                                  2211, 2212,
                                  2212, 2222};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorVoigt3Dfourth){
    //1111 1122 1133 1123 1113 1112
    //2211 2222 2233 2223 2213 2212
    //3311 3322 3333 3323 3313 3312
    //2311 2322 2333 2323 2313 2312
    //1311 1322 1333 1323 1313 1312
    //1211 1222 1233 1223 1213 1212
    double ptr[36]{1111, 1122, 1133, 1123, 1113, 1112,
                   2211, 2222, 2233, 2223, 2213, 2212,
                   3311, 3322, 3333, 3323, 3313, 3312,
                   2311, 2322, 2333, 2323, 2313, 2312,
                   1311, 1322, 1333, 1323, 1313, 1312,
                   1211, 1222, 1233, 1223, 1213, 1212};

    tmech::adaptor<double, 3, 4, tmech::voigt<3>> a(ptr);
    tmech::tensor<double, 3, 4> b{1111,  1112,  1113,
                                  1112,  1122,  1123,
                                  1113,  1123,  1133,
                                  1211,  1212,  1213,
                                  1212,  1222,  1223,
                                  1213,  1223,  1233,
                                  1311,  1312,  1313,
                                  1312,  1322,  1323,
                                  1313,  1323,  1333,
                                  1211,  1212,  1213,
                                  1212,  1222,  1223,
                                  1213,  1223,  1233,
                                  2211,  2212,  2213,
                                  2212,  2222,  2223,
                                  2213,  2223,  2233,
                                  2311,  2312,  2313,
                                  2312,  2322,  2323,
                                  2313,  2323,  2333,
                                  1311,  1312,  1313,
                                  1312,  1322,  1323,
                                  1313,  1323,  1333,
                                  2311,  2312,  2313,
                                  2312,  2322,  2323,
                                  2313,  2323,  2333,
                                  3311,  3312,  3313,
                                  3312,  3322,  3323,
                                  3313,  3323,  3333};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorFull2Dfirst){
    //1 2
    double ptr[4]{1,2};
    tmech::adaptor<double, 2, 1, tmech::full<2>> a(ptr);
    tmech::tensor<double, 2, 1> b{1,2};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorFull3Dfirst){
    //1 2 3
    double ptr[3]{1,2,3};
    tmech::adaptor<double, 3, 1, tmech::full<3>> a(ptr);
    tmech::tensor<double, 3, 1> b{1,2,3};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorFull2Dsecond){
    //11 12
    //21 22
    double ptr[4]{11,12,21,22};
    tmech::adaptor<double, 2, 2, tmech::full<2>> a(ptr);
    tmech::tensor<double, 2, 2> b{11,12, 21,22};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorFull3Dsecond){
    //11 12 13
    //21 22 23
    //31 32 33
    double ptr[9]{11, 12, 13, 21, 22, 23, 31, 32, 33};
    tmech::adaptor<double, 3, 2, tmech::full<3>> a(ptr);
    tmech::tensor<double, 3, 2> b{11, 12, 13, 21, 22, 23, 31, 32, 33};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorFull2Dfourth){
    //1111 1112 1121 1122
    //1211 1212 1221 1222
    //2111 2112 2121 2122
    //2211 2212 2221 2222

    double ptr[16]{1111, 1112, 1121, 1122,
                   1211, 1212, 1221, 1222,
                   2111, 2112, 2121, 2122,
                   2211, 2212, 2221, 2222};
    tmech::adaptor<double, 2, 4, tmech::full<2>> a(ptr);
    tmech::tensor<double, 2, 4> b{1111, 1112, 1121, 1122,
                                  1211, 1212, 1221, 1222,
                                  2111, 2112, 2121, 2122,
                                  2211, 2212, 2221, 2222};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, adaptorFull3Dfourth){
    //1111 1112 1113
    //1121 1122 1123
    //1131 1132 1133

    //1211 1212 1213
    //1221 1222 1223
    //1231 1232 1233

    //1311 1312 1313
    //1321 1322 1323
    //1331 1332 1333

    //2111 2112 2113
    //2121 2122 2123
    //2131 2132 2133

    //2211 2212 2213
    //2221 2222 2223
    //2231 2232 2233

    //2311 2312 2313
    //2321 2322 2323
    //2331 2332 2333

    //3111 3112 3113
    //3121 3122 3123
    //3131 3132 3133

    //3211 3212 3213
    //3221 3222 3223
    //3231 3232 3233

    //3311 3312 3313
    //3321 3322 3323
    //3331 3332 3333
    double ptr[81]{1111, 1112, 1113,
                   1121, 1122, 1123,
                   1131, 1132, 1133,

                   1211, 1212, 1213,
                   1221, 1222, 1223,
                   1231, 1232, 1233,

                   1311, 1312, 1313,
                   1321, 1322, 1323,
                   1331, 1332, 1333,

                   2111, 2112, 2113,
                   2121, 2122, 2123,
                   2131, 2132, 2133,

                   2211, 2212, 2213,
                   2221, 2222, 2223,
                   2231, 2232, 2233,

                   2311, 2312, 2313,
                   2321, 2322, 2323,
                   2331, 2332, 2333,

                   3111, 3112, 3113,
                   3121, 3122, 3123,
                   3131, 3132, 3133,

                   3211, 3212, 3213,
                   3221, 3222, 3223,
                   3231, 3232, 3233,

                   3311, 3312, 3313,
                   3321, 3322, 3323,
                   3331, 3332, 3333};
    tmech::adaptor<double, 3, 4, tmech::full<3>> a(ptr);
    tmech::tensor<double, 3, 4> b{1111, 1112, 1113,
                                  1121, 1122, 1123,
                                  1131, 1132, 1133,

                                  1211, 1212, 1213,
                                  1221, 1222, 1223,
                                  1231, 1232, 1233,

                                  1311, 1312, 1313,
                                  1321, 1322, 1323,
                                  1331, 1332, 1333,

                                  2111, 2112, 2113,
                                  2121, 2122, 2123,
                                  2131, 2132, 2133,

                                  2211, 2212, 2213,
                                  2221, 2222, 2223,
                                  2231, 2232, 2233,

                                  2311, 2312, 2313,
                                  2321, 2322, 2323,
                                  2331, 2332, 2333,

                                  3111, 3112, 3113,
                                  3121, 3122, 3123,
                                  3131, 3132, 3133,

                                  3211, 3212, 3213,
                                  3221, 3222, 3223,
                                  3231, 3232, 3233,

                                  3311, 3312, 3313,
                                  3321, 3322, 3323,
                                  3331, 3332, 3333};

    EXPECT_EQ(true, tmech::almost_equal(a, b, std::numeric_limits<double>::epsilon()));
}

TEST(gtest, exponential_map){
    tmech::tensor<double, 3, 2> A{ 9.064107e-01, -4.649874e-01,  4.431378e-01,
                                   1.557860e+00,  2.493285e-01, -3.458549e-01,
                                   1.747649e+00, -3.824761e-01, -8.110930e-01};

    tmech::tensor<double, 3, 2>Aexp{2.333496383103521e+00,  -9.424245337660712e-01,   6.174107765089896e-01,
                                    2.458154759604529e+00,   7.615487934000856e-01,   1.403592316302926e-01,
                                    1.701192794766057e+00,  -7.849258234991792e-01,   8.233614408596337e-01};
    const auto func_exp = [](auto const& tensor){return tmech::exp(tensor);};
    EXPECT_EQ(true, tmech::almost_equal(Aexp, tmech::exp(A), 5e-7));
    EXPECT_EQ(true, tmech::almost_equal(tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func_exp,A), tmech::exp(A).derivative(), 5e-7));
}

TEST(gtest, symdiff_numdiff_tensor_det){
    symdiff::variable<tmech::tensor<double, 3, 2>,0> Avar;
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::det(tensor);};
    auto dA = tmech::cof(A);
    auto dA_num = tmech::num_diff_central(func, A);
    auto dA_ana = symdiff::derivative<1>(tmech::det(Avar), Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA, dA_num, 5e-6));
    EXPECT_EQ(true, tmech::almost_equal(dA, dA_ana(std::make_tuple(A)), 5e-6));
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A)), 5e-6));
}

TEST(gtest, symdiff_numdiff_tensor_trace){
    symdiff::variable<tmech::tensor<double, 3, 2>,0> Avar;
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::trace(tensor);};
    auto dA_num = tmech::num_diff_central(func, A);
    auto dA_ana = symdiff::derivative<1>(tmech::trace(Avar), Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A)), 5e-6));
}

TEST(gtest, symdiff_numdiff_tensor_norm){
    symdiff::variable<tmech::tensor<double, 3, 2>,0> Avar;
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return std::sqrt(tmech::dcontract(tensor,tensor));};
    auto s_func = std::sqrt(tmech::inner_product<tmech::sequence<1,2>,tmech::sequence<1,2>>(Avar, Avar));
    auto dA_num = tmech::num_diff_central(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A)), 5e-6));
}

TEST(gtest, symdiff_numdiff_tensor_inv){
    symdiff::variable<tmech::tensor<double, 3, 2>,0> Avar;
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::inv(tensor);};
    auto s_func = tmech::inv(Avar);
    auto dA_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A)), 5e-6));
}

TEST(gtest, symdiff_numdiff_tensor_dcontract_2_2){
    symdiff::variable<tmech::tensor<double, 3, 2>,0> Avar;
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::dcontract(tensor, tensor);};
    auto s_func = tmech::dcontract(Avar, Avar);
    auto dA_num = tmech::num_diff_central(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A)), 5e-6));
}

TEST(gtest, symdiff_numdiff_tensor_dcontract_4_2){
    symdiff::variable<tmech::tensor<double, 3, 2>, 0> Avar;
    symdiff::variable<tmech::tensor<double, 3, 4>, 1> Bvar;
    tmech::tensor<double, 3, 2> A;
    tmech::tensor<double, 3, 4> B;
    A.randn();
    B.randn();
    auto func = [&](auto const& tensor){return tmech::dcontract(B, tensor);};
    auto s_func = tmech::dcontract(Bvar, Avar);
    auto dA_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A, B)), 5e-5));
}

TEST(gtest, symdiff_numdiff_tensor_dcontract_4_4){
    symdiff::variable<tmech::tensor<double, 3, 4>, 0> Avar;
    symdiff::variable<tmech::tensor<double, 3, 4>, 1> Bvar;
    tmech::tensor<double, 3, 4> A;
    tmech::tensor<double, 3, 4> B;
    A.randn();
    B.randn();
    auto func = [&](auto const& tensor){return tmech::dcontract(B, tensor);};
    auto s_func = tmech::dcontract(Bvar, Avar);
    auto dA_num = tmech::num_diff_central<tmech::sequence<1,2,3,4,5,6,7,8>>(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    std::cout<<"Hallo"<<std::endl;
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A, B)), 5e-6));
    std::cout<<"Hallo"<<std::endl;
}

TEST(gtest, symdiff_numdiff_tensor_dcontract_2_4){
    std::cout<<"Hallo"<<std::endl;
    symdiff::variable<tmech::tensor<double, 3, 4>, 0> Avar;
    symdiff::variable<tmech::tensor<double, 3, 2>, 1> Bvar;
    tmech::tensor<double, 3, 4> A;
    tmech::tensor<double, 3, 2> B;
    A.randn();
    B.randn();
    auto func = [&](auto const& tensor){return tmech::dcontract(B, tensor);};
    std::cout<<"Hallo"<<std::endl;
    auto s_func = tmech::dcontract(Bvar, Avar);
    auto dA_num = tmech::num_diff_central<tmech::sequence<1,2,3,4,5,6>>(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A, B)), 5e-6));
}

TEST(gtest, symdiff_numdiff_tensor_trans_2){
    symdiff::variable<tmech::tensor<double, 3, 2>,0> Avar;
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::trans(tensor);};
    auto s_func = tmech::trans(Avar);
    auto dA_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func, A);
    auto dA_ana = symdiff::derivative<1>(s_func, Avar);
    EXPECT_EQ(true, tmech::almost_equal(dA_num, dA_ana(std::make_tuple(A)), 5e-6));
}

TEST(gtest, numdiff_tensor_det){
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::det(tensor);};
    auto dA = tmech::cof(A);
    auto dA_num = tmech::num_diff_central(func, A);
    EXPECT_EQ(true, tmech::almost_equal(dA, dA_num, 5e-6));
}

TEST(gtest, numdiff_tensor_trace){
    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::trace(tensor);};
    auto dA = tmech::eye<double,3,2>();
    auto dA_num = tmech::num_diff_central(func, A);
    EXPECT_EQ(true, tmech::almost_equal(dA, dA_num, 5e-6));
}

TEST(gtest, numdiff_tensor_inv_2){
    using Sym2x2 = std::tuple<tmech::sequence<1,2>>;
    using Sym4x4 = std::tuple<tmech::sequence<1,2,3,4>, tmech::sequence<2,1,3,4>>;

    tmech::tensor<double, 3, 2> A;
    A.randn();
    auto func = [](auto const& tensor){return tmech::inv(tensor);};
    auto dA = -tmech::otimesu(tmech::inv(A), tmech::trans(tmech::inv(A)));
    auto dA_num = tmech::num_diff_central<tmech::sequence<1,2,3,4>>(func, A);
    EXPECT_EQ(true, tmech::almost_equal(dA, dA_num, 5e-6));
}

absFunction(double, 2, 1)
absFunction(double, 2, 2)
absFunction(double, 2, 4)
absFunction(double, 3, 1)
absFunction(double, 3, 2)
absFunction(double, 3, 4)
absFunction(float, 2, 1)
absFunction(float, 2, 2)
absFunction(float, 2, 4)
absFunction(float, 3, 1)
absFunction(float, 3, 2)
absFunction(float, 3, 4)

absFunctionEvaluate(double, 2, 1)
absFunctionEvaluate(double, 2, 2)
absFunctionEvaluate(double, 2, 4)
absFunctionEvaluate(double, 3, 1)
absFunctionEvaluate(double, 3, 2)
absFunctionEvaluate(double, 3, 4)
absFunctionEvaluate(float, 2, 1)
absFunctionEvaluate(float, 2, 2)
absFunctionEvaluate(float, 2, 4)
absFunctionEvaluate(float, 3, 1)
absFunctionEvaluate(float, 3, 2)
absFunctionEvaluate(float, 3, 4)


inv_function_2(double, 2)
inv_function_2(double, 3)
inv_function_2(float, 2)
inv_function_2(float, 3)

inv_function_4(double, 2)
inv_function_4(double, 3)
inv_function_4(float, 2)
inv_function_4(float, 3)

inv_full_function_4(double, 2)
inv_full_function_4(double, 3)
inv_full_function_4(float, 2)
inv_full_function_4(float, 3)

compareEqualOperator(double, 2, 1)
compareEqualOperator(double, 2, 2)
compareEqualOperator(double, 2, 4)
compareEqualOperator(double, 3, 1)
compareEqualOperator(double, 3, 2)
compareEqualOperator(double, 3, 4)
compareEqualOperator(float, 2, 1)
compareEqualOperator(float, 2, 2)
compareEqualOperator(float, 2, 4)
compareEqualOperator(float, 3, 1)
compareEqualOperator(float, 3, 2)
compareEqualOperator(float, 3, 4)
compareEqualOperator(complexd, 2, 1)
compareEqualOperator(complexd, 2, 2)
compareEqualOperator(complexd, 2, 4)
compareEqualOperator(complexd, 3, 1)
compareEqualOperator(complexd, 3, 2)
compareEqualOperator(complexd, 3, 4)
compareEqualOperator(complexf, 2, 1)
compareEqualOperator(complexf, 2, 2)
compareEqualOperator(complexf, 2, 4)
compareEqualOperator(complexf, 3, 1)
compareEqualOperator(complexf, 3, 2)
compareEqualOperator(complexf, 3, 4)

compareEqualOperatorFunction(double, 2, 1, abs)
compareEqualOperatorFunction(double, 2, 2, abs)
compareEqualOperatorFunction(double, 2, 4, abs)
compareEqualOperatorFunction(double, 3, 1, abs)
compareEqualOperatorFunction(double, 3, 2, abs)
compareEqualOperatorFunction(double, 3, 4, abs)
compareEqualOperatorFunction(float, 2, 1, abs)
compareEqualOperatorFunction(float, 2, 2, abs)
compareEqualOperatorFunction(float, 2, 4, abs)
compareEqualOperatorFunction(float, 3, 1, abs)
compareEqualOperatorFunction(float, 3, 2, abs)
compareEqualOperatorFunction(float, 3, 4, abs)

compareEqualOperatorFunction(double, 2, 2, inv)
compareEqualOperatorFunction(double, 2, 4, inv)
compareEqualOperatorFunction(double, 3, 2, inv)
compareEqualOperatorFunction(double, 3, 4, inv)
compareEqualOperatorFunction(float, 2, 2, inv)
compareEqualOperatorFunction(float, 2, 4, inv)
compareEqualOperatorFunction(float, 3, 2, inv)
compareEqualOperatorFunction(float, 3, 4, inv)


compareEqualOperatorEvaluate(double, 2, 1)
compareEqualOperatorEvaluate(double, 2, 2)
compareEqualOperatorEvaluate(double, 2, 4)
compareEqualOperatorEvaluate(double, 3, 1)
compareEqualOperatorEvaluate(double, 3, 2)
compareEqualOperatorEvaluate(double, 3, 4)
compareEqualOperatorEvaluate(float, 2, 1)
compareEqualOperatorEvaluate(float, 2, 2)
compareEqualOperatorEvaluate(float, 2, 4)
compareEqualOperatorEvaluate(float, 3, 1)
compareEqualOperatorEvaluate(float, 3, 2)
compareEqualOperatorEvaluate(float, 3, 4)
compareEqualOperatorEvaluate(complexd, 2, 1)
compareEqualOperatorEvaluate(complexd, 2, 2)
compareEqualOperatorEvaluate(complexd, 2, 4)
compareEqualOperatorEvaluate(complexd, 3, 1)
compareEqualOperatorEvaluate(complexd, 3, 2)
compareEqualOperatorEvaluate(complexd, 3, 4)
compareEqualOperatorEvaluate(complexf, 2, 1)
compareEqualOperatorEvaluate(complexf, 2, 2)
compareEqualOperatorEvaluate(complexf, 2, 4)
compareEqualOperatorEvaluate(complexf, 3, 1)
compareEqualOperatorEvaluate(complexf, 3, 2)
compareEqualOperatorEvaluate(complexf, 3, 4)


compareGreaterOperator(double, 2, 1)
compareGreaterOperator(double, 2, 2)
compareGreaterOperator(double, 2, 4)
compareGreaterOperator(double, 3, 1)
compareGreaterOperator(double, 3, 2)
compareGreaterOperator(double, 3, 4)
compareGreaterOperator(float, 2, 1)
compareGreaterOperator(float, 2, 2)
compareGreaterOperator(float, 2, 4)
compareGreaterOperator(float, 3, 1)
compareGreaterOperator(float, 3, 2)
compareGreaterOperator(float, 3, 4)

compareGreaterOperatorEvaluate(double, 2, 1)
compareGreaterOperatorEvaluate(double, 2, 2)
compareGreaterOperatorEvaluate(double, 2, 4)
compareGreaterOperatorEvaluate(double, 3, 1)
compareGreaterOperatorEvaluate(double, 3, 2)
compareGreaterOperatorEvaluate(double, 3, 4)
compareGreaterOperatorEvaluate(float, 2, 1)
compareGreaterOperatorEvaluate(float, 2, 2)
compareGreaterOperatorEvaluate(float, 2, 4)
compareGreaterOperatorEvaluate(float, 3, 1)
compareGreaterOperatorEvaluate(float, 3, 2)
compareGreaterOperatorEvaluate(float, 3, 4)


compareGreaterEqualOperator(double, 2, 1)
compareGreaterEqualOperator(double, 2, 2)
compareGreaterEqualOperator(double, 2, 4)
compareGreaterEqualOperator(double, 3, 1)
compareGreaterEqualOperator(double, 3, 2)
compareGreaterEqualOperator(double, 3, 4)
compareGreaterEqualOperator(float, 2, 1)
compareGreaterEqualOperator(float, 2, 2)
compareGreaterEqualOperator(float, 2, 4)
compareGreaterEqualOperator(float, 3, 1)
compareGreaterEqualOperator(float, 3, 2)
compareGreaterEqualOperator(float, 3, 4)

compareGreaterEqualOperatorEvaluate(double, 2, 1)
compareGreaterEqualOperatorEvaluate(double, 2, 2)
compareGreaterEqualOperatorEvaluate(double, 2, 4)
compareGreaterEqualOperatorEvaluate(double, 3, 1)
compareGreaterEqualOperatorEvaluate(double, 3, 2)
compareGreaterEqualOperatorEvaluate(double, 3, 4)
compareGreaterEqualOperatorEvaluate(float, 2, 1)
compareGreaterEqualOperatorEvaluate(float, 2, 2)
compareGreaterEqualOperatorEvaluate(float, 2, 4)
compareGreaterEqualOperatorEvaluate(float, 3, 1)
compareGreaterEqualOperatorEvaluate(float, 3, 2)
compareGreaterEqualOperatorEvaluate(float, 3, 4)


compareLessOperator(double, 2, 1)
compareLessOperator(double, 2, 2)
compareLessOperator(double, 2, 4)
compareLessOperator(double, 3, 1)
compareLessOperator(double, 3, 2)
compareLessOperator(double, 3, 4)
compareLessOperator(float, 2, 1)
compareLessOperator(float, 2, 2)
compareLessOperator(float, 2, 4)
compareLessOperator(float, 3, 1)
compareLessOperator(float, 3, 2)
compareLessOperator(float, 3, 4)

compareLessOperatorEvaluate(double, 2, 1)
compareLessOperatorEvaluate(double, 2, 2)
compareLessOperatorEvaluate(double, 2, 4)
compareLessOperatorEvaluate(double, 3, 1)
compareLessOperatorEvaluate(double, 3, 2)
compareLessOperatorEvaluate(double, 3, 4)
compareLessOperatorEvaluate(float, 2, 1)
compareLessOperatorEvaluate(float, 2, 2)
compareLessOperatorEvaluate(float, 2, 4)
compareLessOperatorEvaluate(float, 3, 1)
compareLessOperatorEvaluate(float, 3, 2)
compareLessOperatorEvaluate(float, 3, 4)


compareLessEqualOperator(double, 2, 1)
compareLessEqualOperator(double, 2, 2)
compareLessEqualOperator(double, 2, 4)
compareLessEqualOperator(double, 3, 1)
compareLessEqualOperator(double, 3, 2)
compareLessEqualOperator(double, 3, 4)
compareLessEqualOperator(float, 2, 1)
compareLessEqualOperator(float, 2, 2)
compareLessEqualOperator(float, 2, 4)
compareLessEqualOperator(float, 3, 1)
compareLessEqualOperator(float, 3, 2)
compareLessEqualOperator(float, 3, 4)

compareLessEqualOperatorEvaluate(double, 2, 1)
compareLessEqualOperatorEvaluate(double, 2, 2)
compareLessEqualOperatorEvaluate(double, 2, 4)
compareLessEqualOperatorEvaluate(double, 3, 1)
compareLessEqualOperatorEvaluate(double, 3, 2)
compareLessEqualOperatorEvaluate(double, 3, 4)
compareLessEqualOperatorEvaluate(float, 2, 1)
compareLessEqualOperatorEvaluate(float, 2, 2)
compareLessEqualOperatorEvaluate(float, 2, 4)
compareLessEqualOperatorEvaluate(float, 3, 1)
compareLessEqualOperatorEvaluate(float, 3, 2)
compareLessEqualOperatorEvaluate(float, 3, 4)


compareNoEqualOperator(double, 2, 1)
compareNoEqualOperator(double, 2, 2)
compareNoEqualOperator(double, 2, 4)
compareNoEqualOperator(double, 3, 1)
compareNoEqualOperator(double, 3, 2)
compareNoEqualOperator(double, 3, 4)
compareNoEqualOperator(float, 2, 1)
compareNoEqualOperator(float, 2, 2)
compareNoEqualOperator(float, 2, 4)
compareNoEqualOperator(float, 3, 1)
compareNoEqualOperator(float, 3, 2)
compareNoEqualOperator(float, 3, 4)
compareNoEqualOperator(complexd, 2, 1)
compareNoEqualOperator(complexd, 2, 2)
compareNoEqualOperator(complexd, 2, 4)
compareNoEqualOperator(complexd, 3, 1)
compareNoEqualOperator(complexd, 3, 2)
compareNoEqualOperator(complexd, 3, 4)
compareNoEqualOperator(complexf, 2, 1)
compareNoEqualOperator(complexf, 2, 2)
compareNoEqualOperator(complexf, 2, 4)
compareNoEqualOperator(complexf, 3, 1)
compareNoEqualOperator(complexf, 3, 2)
compareNoEqualOperator(complexf, 3, 4)

compareNoEqualOperatorEvaluate(double, 2, 1)
compareNoEqualOperatorEvaluate(double, 2, 2)
compareNoEqualOperatorEvaluate(double, 2, 4)
compareNoEqualOperatorEvaluate(double, 3, 1)
compareNoEqualOperatorEvaluate(double, 3, 2)
compareNoEqualOperatorEvaluate(double, 3, 4)
compareNoEqualOperatorEvaluate(float, 2, 1)
compareNoEqualOperatorEvaluate(float, 2, 2)
compareNoEqualOperatorEvaluate(float, 2, 4)
compareNoEqualOperatorEvaluate(float, 3, 1)
compareNoEqualOperatorEvaluate(float, 3, 2)
compareNoEqualOperatorEvaluate(float, 3, 4)
compareNoEqualOperatorEvaluate(complexd, 2, 1)
compareNoEqualOperatorEvaluate(complexd, 2, 2)
compareNoEqualOperatorEvaluate(complexd, 2, 4)
compareNoEqualOperatorEvaluate(complexd, 3, 1)
compareNoEqualOperatorEvaluate(complexd, 3, 2)
compareNoEqualOperatorEvaluate(complexd, 3, 4)
compareNoEqualOperatorEvaluate(complexf, 2, 1)
compareNoEqualOperatorEvaluate(complexf, 2, 2)
compareNoEqualOperatorEvaluate(complexf, 2, 4)
compareNoEqualOperatorEvaluate(complexf, 3, 1)
compareNoEqualOperatorEvaluate(complexf, 3, 2)
compareNoEqualOperatorEvaluate(complexf, 3, 4)

adjFunction(double, 2)
adjFunction(double, 3)
adjFunction(float, 2)
adjFunction(float, 3)

adjFunctionEvaluate(double, 2)
adjFunctionEvaluate(double, 3)
adjFunctionEvaluate(float, 2)
adjFunctionEvaluate(float, 3)


cofFunction(double, 2)
cofFunction(double, 3)
cofFunction(float, 2)
cofFunction(float, 3)

cofFunctionEvaluate(double, 2)
cofFunctionEvaluate(double, 3)
cofFunctionEvaluate(float, 2)
cofFunctionEvaluate(float, 3)


basisChange2(double, 2)
basisChange2(double, 3)
basisChange2(float, 2)
basisChange2(float, 3)
basisChange2(complexd, 2)
basisChange2(complexd, 3)
basisChange2(complexf, 2)
basisChange2(complexf, 3)

basisChangeEvaluate2(double, 2)
basisChangeEvaluate2(double, 3)
basisChangeEvaluate2(float, 2)
basisChangeEvaluate2(float, 3)
basisChangeEvaluate2(complexd, 2)
basisChangeEvaluate2(complexd, 3)
basisChangeEvaluate2(complexf, 2)
basisChangeEvaluate2(complexf, 3)

basisChange4(double, 2)
basisChange4(double, 3)
basisChange4(float, 2)
basisChange4(float, 3)
basisChange4(complexd, 2)
basisChange4(complexd, 3)
basisChange4(complexf, 2)
basisChange4(complexf, 3)

basisChangeEvaluate4(double, 2)
basisChangeEvaluate4(double, 3)
basisChangeEvaluate4(float, 2)
basisChangeEvaluate4(float, 3)
basisChangeEvaluate4(complexd, 2)
basisChangeEvaluate4(complexd, 3)
basisChangeEvaluate4(complexf, 2)
basisChangeEvaluate4(complexf, 3)

dotProduct(double,1)
dotProduct(double,2)
dotProduct(double,3)
dotProduct(float,1)
dotProduct(float,2)
dotProduct(float,3)
dotProduct(complexd,1)
dotProduct(complexd,2)
dotProduct(complexd,3)
dotProduct(complexf,1)
dotProduct(complexf,2)
dotProduct(complexf,3)

inner_product2(double, 1)
inner_product2(double, 2)
inner_product2(double, 3)
inner_product2(float, 1)
inner_product2(float, 2)
inner_product2(float, 3)
inner_product2(complexd, 1)
inner_product2(complexd, 2)
inner_product2(complexd, 3)
inner_product2(complexf, 1)
inner_product2(complexf, 2)
inner_product2(complexf, 3)

inner_product4(double, 1)
inner_product4(double, 2)
inner_product4(double, 3)
inner_product4(float, 1)
inner_product4(float, 2)
inner_product4(float, 3)
inner_product4(complexd, 1)
inner_product4(complexd, 2)
inner_product4(complexd, 3)
inner_product4(complexf, 1)
inner_product4(complexf, 2)
inner_product4(complexf, 3)

outer_product2(double, 1)
outer_product2(double, 2)
outer_product2(double, 3)
outer_product2(float, 1)
outer_product2(float, 2)
outer_product2(float, 3)
outer_product2(complexd, 1)
outer_product2(complexd, 2)
outer_product2(complexd, 3)
outer_product2(complexf, 1)
outer_product2(complexf, 2)
outer_product2(complexf, 3)

outer_product4(double, 1)
outer_product4(double, 2)
outer_product4(double, 3)
outer_product4(float, 1)
outer_product4(float, 2)
outer_product4(float, 3)
outer_product4(complexd, 1)
outer_product4(complexd, 2)
outer_product4(complexd, 3)
outer_product4(complexf, 1)
outer_product4(complexf, 2)
outer_product4(complexf, 3)

transposition_second_order_tensors(double, 1)
transposition_second_order_tensors(double, 2)
transposition_second_order_tensors(double, 3)
transposition_second_order_tensors(float, 1)
transposition_second_order_tensors(float, 2)
transposition_second_order_tensors(float, 3)
transposition_second_order_tensors(complexf, 1)
transposition_second_order_tensors(complexf, 2)
transposition_second_order_tensors(complexf, 3)
transposition_second_order_tensors(complexd, 1)
transposition_second_order_tensors(complexd, 2)
transposition_second_order_tensors(complexd, 3)

transposition_fourth_order_tensors(double, 1)
transposition_fourth_order_tensors(double, 2)
transposition_fourth_order_tensors(double, 3)
transposition_fourth_order_tensors(float, 1)
transposition_fourth_order_tensors(float, 2)
transposition_fourth_order_tensors(float, 3)
transposition_fourth_order_tensors(complexf, 1)
transposition_fourth_order_tensors(complexf, 2)
transposition_fourth_order_tensors(complexf, 3)
transposition_fourth_order_tensors(complexd, 1)
transposition_fourth_order_tensors(complexd, 2)
transposition_fourth_order_tensors(complexd, 3)

left_transposition_fourth_order_tensors(double, 1)
left_transposition_fourth_order_tensors(double, 2)
left_transposition_fourth_order_tensors(double, 3)
left_transposition_fourth_order_tensors(float, 1)
left_transposition_fourth_order_tensors(float, 2)
left_transposition_fourth_order_tensors(float, 3)
left_transposition_fourth_order_tensors(complexf, 1)
left_transposition_fourth_order_tensors(complexf, 2)
left_transposition_fourth_order_tensors(complexf, 3)
left_transposition_fourth_order_tensors(complexd, 1)
left_transposition_fourth_order_tensors(complexd, 2)
left_transposition_fourth_order_tensors(complexd, 3)


powFunction(double, 2)
powFunction(double, 3)
powFunction(float, 2)
powFunction(float, 3)
powFunction(complexd, 2)
powFunction(complexd, 3)
powFunction(complexf, 2)
powFunction(complexf, 3)

powFunctionEvaluate(double, 2)
powFunctionEvaluate(double, 3)
powFunctionEvaluate(float, 2)
powFunctionEvaluate(float, 3)
powFunctionEvaluate(complexd, 2)
powFunctionEvaluate(complexd, 3)
powFunctionEvaluate(complexf, 2)
powFunctionEvaluate(complexf, 3)

crossProduct(double, 3)
crossProduct(float, 3)
crossProduct(complexf, 3)
crossProduct(complexd, 3)

symmetricPart(float, 2)
symmetricPart(float, 3)
symmetricPart(double, 2)
symmetricPart(double, 3)
symmetricPart(complexf, 2)
symmetricPart(complexf, 3)
symmetricPart(complexd, 2)
symmetricPart(complexd, 3)

symmetricPartEvaluate(float, 2)
symmetricPartEvaluate(float, 3)
symmetricPartEvaluate(double, 2)
symmetricPartEvaluate(double, 3)
symmetricPartEvaluate(complexf, 2)
symmetricPartEvaluate(complexf, 3)
symmetricPartEvaluate(complexd, 2)
symmetricPartEvaluate(complexd, 3)


skewSymmetricPart(float, 2)
skewSymmetricPart(float, 3)
skewSymmetricPart(double, 2)
skewSymmetricPart(double, 3)
skewSymmetricPart(complexf, 2)
skewSymmetricPart(complexf, 3)
skewSymmetricPart(complexd, 2)
skewSymmetricPart(complexd, 3)

skewSymmetricPartEvaluate(float, 2)
skewSymmetricPartEvaluate(float, 3)
skewSymmetricPartEvaluate(double, 2)
skewSymmetricPartEvaluate(double, 3)
skewSymmetricPartEvaluate(complexf, 2)
skewSymmetricPartEvaluate(complexf, 3)
skewSymmetricPartEvaluate(complexd, 2)
skewSymmetricPartEvaluate(complexd, 3)

Trace(float, 1)
Trace(float, 2)
Trace(float, 3)
Trace(double, 1)
Trace(double, 2)
Trace(double, 3)
Trace(complexf, 1)
Trace(complexf, 2)
Trace(complexf, 3)
Trace(complexd, 1)
Trace(complexd, 2)
Trace(complexd, 3)

deviatoricPart(float, 2)
deviatoricPart(float, 3)
deviatoricPart(double, 2)
deviatoricPart(double, 3)
deviatoricPart(complexf, 2)
deviatoricPart(complexf, 3)
deviatoricPart(complexd, 2)
deviatoricPart(complexd, 3)

deviatoricPartEvaluate(float, 2)
deviatoricPartEvaluate(float, 3)
deviatoricPartEvaluate(double, 2)
deviatoricPartEvaluate(double, 3)
deviatoricPartEvaluate(complexf, 2)
deviatoricPartEvaluate(complexf, 3)
deviatoricPartEvaluate(complexd, 2)
deviatoricPartEvaluate(complexd, 3)

volumetricPart(float, 1)
volumetricPart(float, 2)
volumetricPart(float, 3)
volumetricPart(double, 1)
volumetricPart(double, 2)
volumetricPart(double, 3)
volumetricPart(complexf, 1)
volumetricPart(complexf, 2)
volumetricPart(complexf, 3)
volumetricPart(complexd, 1)
volumetricPart(complexd, 2)
volumetricPart(complexd, 3)

DoubleContractionLeft(double,   2, 2, 4)
DoubleContractionLeft(double,   3, 2, 4)
DoubleContractionLeft(float,    2, 2, 4)
DoubleContractionLeft(float,    3, 2, 4)
DoubleContractionLeft(complexd, 2, 2, 4)
DoubleContractionLeft(complexd, 3, 2, 4)
DoubleContractionLeft(complexf, 2, 2, 4)
DoubleContractionLeft(complexf, 3, 2, 4)

DoubleContractionRight(double,   2, 2, 4)
DoubleContractionRight(double,   3, 2, 4)
DoubleContractionRight(float,    2, 2, 4)
DoubleContractionRight(float,    3, 2, 4)
DoubleContractionRight(complexd, 2, 2, 4)
DoubleContractionRight(complexd, 3, 2, 4)
DoubleContractionRight(complexf, 2, 2, 4)
DoubleContractionRight(complexf, 3, 2, 4)

DoubleContractionRight(double,   2, 4, 4)
DoubleContractionRight(double,   3, 4, 4)
DoubleContractionRight(float,    2, 4, 4)
DoubleContractionRight(float,    3, 4, 4)
DoubleContractionRight(complexd, 2, 4, 4)
DoubleContractionRight(complexd, 3, 4, 4)
DoubleContractionRight(complexf, 2, 4, 4)
DoubleContractionRight(complexf, 3, 4, 4)

SingleContraction(double,   2, 2, 2)
SingleContraction(double,   3, 2, 2)
SingleContraction(float,    2, 2, 2)
SingleContraction(float,    3, 2, 2)
SingleContraction(complexd, 2, 2, 2)
SingleContraction(complexd, 3, 2, 2)
SingleContraction(complexf, 2, 2, 2)
SingleContraction(complexf, 3, 2, 2)

symmetric_eigenvalue_problem(double, sqrt, std::sqrt)
symmetric_eigenvalue_problem(float,  sqrt, std::sqrt)
symmetric_eigenvalue_problem(double, log, std::log)
symmetric_eigenvalue_problem(float,  log, std::log)
symmetric_eigenvalue_problem(double, exp_sym, std::exp)
symmetric_eigenvalue_problem(float,  exp_sym, std::exp)
symmetric_eigenvalue_problem(double, negative, negative_value)
symmetric_eigenvalue_problem(float,  negative, negative_value)
symmetric_eigenvalue_problem(double, positive, positive_value)
symmetric_eigenvalue_problem(float,  positive, positive_value)

signDecomposition(double, 2)
signDecomposition(double, 3)
signDecomposition(float, 2)
signDecomposition(float, 3)
signDecompositionEvaluate(double, 2)
signDecompositionEvaluate(double, 3)
signDecompositionEvaluate(float, 2)
signDecompositionEvaluate(float, 3)

positive_and_negavtive_decomposition(double, 2)
positive_and_negavtive_decomposition(double, 3)
positive_and_negavtive_decomposition(float, 2)
positive_and_negavtive_decomposition(float, 3)

derivatives_polar_decomposition_eigen(double, 2)
derivatives_polar_decomposition_eigen(double, 3)

derivatives_polar_decomposition_newton(double, 2)
derivatives_polar_decomposition_newton(double, 3)


productRuleScalar(double)
sumRuleScalar(double)
minRuleScalar(double)
expVarScalar(double)
expFuncScalar(double)
logVarScalar(double)
logFuncScalar(double)
sqrtVarScalar(double)
sqrtFuncScalar(double)
absVarScalar(double)
absFuncScalar(double)
cosVarScalar(double)
cosFuncScalar(double)
sinVarScalar(double)
sinFuncScalar(double)
tanVarScalar(double)
tanFuncScalar(double)
powVaraScalar(double)
powFuncaScalar(double)
powVarbScalar(double)
powFuncbScalar(double)
powVarcScalar(double)
powFunccScalar(double)

productRuleScalar(float)
sumRuleScalar(float)
minRuleScalar(float)
expVarScalar(float)
expFuncScalar(float)
logVarScalar(float)
logFuncScalar(float)
sqrtVarScalar(float)
sqrtFuncScalar(float)
absVarScalar(float)
absFuncScalar(float)
cosVarScalar(float)
cosFuncScalar(float)
sinVarScalar(float)
sinFuncScalar(float)
tanVarScalar(float)
tanFuncScalar(float)
powVaraScalar(float)
powFuncaScalar(float)
powVarbScalar(float)
powFuncbScalar(float)
powVarcScalar(float)
powFunccScalar(float)

productRuleTensor(double, 3, 2)
productRuleTensor(double, 2, 2)
addRuleTensor(double, 3, 2)
addRuleTensor(double, 2, 2)
minRuleTensor(double, 3, 2)
minRuleTensor(double, 2, 2)

lambdaTensor(double, 3, 2, inv)
lambdaTensor(double, 2, 2, inv)
lambdaTensor(double, 3, 2, cof)
lambdaTensor(double, 2, 2, cof)
lambdaTensor(double, 3, 2, adj)
lambdaTensor(double, 2, 2, adj)
lambdaTensor(double, 3, 2, vol)
lambdaTensor(double, 2, 2, vol)
lambdaTensor(double, 3, 2, dev)
lambdaTensor(double, 2, 2, dev)

lambdaTensorSym_3(double, 2, sqrt)
lambdaTensorSym_3(double, 2, exp_sym)
lambdaTensorSym_3(double, 2, log)
lambdaTensorSym_3(double, 2, positive)
lambdaTensorSym_3(double, 2, negative)

//powFunction(double, 2)
//powFunction(double, 3)
//powFunction(float, 2)
//powFunction(float, 3)

//symmetric_eigenvalue_derivative_problem(double, sqrt)
//symmetric_eigenvalue_derivative_problem(double, log)
//symmetric_eigenvalue_derivative_problem(double, exp_sym)
//symmetric_eigenvalue_derivative_problem(double, negative)
//symmetric_eigenvalue_derivative_problem(double, positive)

//simple_contraction_fourth_second_order_tensors(double, 2)
//simple_contraction_fourth_second_order_tensors(double, 3)
//simple_contraction_fourth_second_order_tensors(float, 2)
//simple_contraction_fourth_second_order_tensors(float, 3)
//simple_contraction_fourth_second_order_tensors(complexd, 2)
//simple_contraction_fourth_second_order_tensors(complexf, 3)

//dcontract_fourth_order_tensors(double, 2)
//dcontract_fourth_order_tensors(double, 3)
//dcontract_fourth_order_tensors(float, 2)
//dcontract_fourth_order_tensors(float, 3)
//dcontract_fourth_order_tensors(complexd, 2)
//dcontract_fourth_order_tensors(complexf, 3)

//dcontract_fourth_second_order_tensors(double, 2)
//dcontract_fourth_second_order_tensors(double, 3)
//dcontract_fourth_second_order_tensors(float, 2)
//dcontract_fourth_second_order_tensors(float, 3)
//dcontract_fourth_second_order_tensors(complexd, 2)
//dcontract_fourth_second_order_tensors(complexf, 3)



//transposition_dcontract_fourth_order_tensors(double, 2)
//transposition_dcontract_fourth_order_tensors(double, 3)
//transposition_dcontract_fourth_order_tensors(float, 2)
//transposition_dcontract_fourth_order_tensors(float, 3)
//transposition_dcontract_fourth_order_tensors(complexd, 2)
//transposition_dcontract_fourth_order_tensors(complexf, 3)

//basis_rearrangement(double, 2)
//basis_rearrangement(double, 3)
//basis_rearrangement(float, 2)
//basis_rearrangement(float, 3)
//basis_rearrangement(complexd, 2)
//basis_rearrangement(complexf, 3)

//Symmetric eigenvalue problem for 2D
//Symmetric eigenvalue problem derivative for 2D
//inverse rank2 and inverse rank 4
//exp nonsymmetric tensors
//polar decomposition
//derivatives polar decomposition


#endif // TST_GTEST_H
