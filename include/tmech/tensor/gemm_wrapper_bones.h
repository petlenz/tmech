/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_WRAPPER_BONES_H
#define GEMM_WRAPPER_BONES_H

#include <stdlib.h>
#include <array>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #include <immintrin.h>
    #define TMECH_X86_SIMD 1
#elif defined(__aarch64__) || defined(_M_ARM64)
    #include <arm_neon.h>
    #define TMECH_ARM_NEON 1
#endif

namespace detail {

template <typename LHS, typename RHS, typename RESULT, std::size_t RowsLHS, std::size_t ColumnsLHS, std::size_t RowsRHS, std::size_t ColumnsRHS>
class gemm_wrapper
{
    //static std::array<RHS, RowsRHS*ColumnsRHS> temp;
public:
    using size_type = std::size_t;

    static constexpr inline void evaluate(LHS const* __lhs, RHS const* __rhs, RESULT* __result)noexcept{

        if constexpr (ColumnsLHS == 1){
            gevm(__lhs, __rhs, __result);
            return ;
        }

        if constexpr (ColumnsRHS == 1){
//            if constexpr (RowsLHS == 4 && ColumnsLHS == 4){
//#if defined(__AVX__)
//                if constexpr (std::is_same_v<double, LHS> && std::is_same_v<double, RHS> && std::is_same_v<double, RESULT>){
//                    gemv_d_4x4_2(__lhs, __rhs, __result);
//                    return ;
//                }
//#endif
//            }
            gemv(__lhs, __rhs, __result);
            return ;
        }
//        if constexpr (ColumnsLHS >= 81){
//            //ddouble contranction of two eight order tensors
//            gemm_impl9(__lhs, __rhs, __result);
//            return ;
//        }

        //special rule for double contraction of two eight order tensors
        constexpr std::size_t size{3*3*3*3};
        if constexpr (RowsLHS >= size && ColumnsLHS >= size &&
                      RowsRHS >= size && ColumnsRHS >= size){
            gemm_simple_bigger(__lhs, __rhs, __result);
//            char *TRANSA,
//                char *TRANSB,
//                int *M,
//                int *N,
//                int *K,
//                double *ALPHA,
//                double **A,
//                int *LDA,
//                double **B,
//                int *LDB,
//                double *BETA,
//                double **C,
//                int *LDC
            //double alpha{1}, beta{0};
            //dgemm_("N","N",RowsLHS,ColumnsLHS,ColumnsRHS);
            return ;
        }


        gemm_simple(__lhs, __rhs, __result);


        //        if constexpr (ColumnsLHS <= 4){

        //        }else{
        //            gemm(__lhs, __rhs, __result);
        //        }

        //an improvement for larger data is needed
        //        if constexpr (RowsLHS > 16){
        //            gemm_block(__lhs, __rhs, __result);
        //        }else{
        //            gemm(__lhs, __rhs, __result);
        //        }


        return ;


        //        //check if matrix vector multiplication
        //        if constexpr (ColumnsRHS == 1){
        //            gemv(__lhs, __rhs, __result);
        //            return ;

        ////#if (defined(__SSE__) && !defined(__AVX__))
        ////            if constexpr (std::is_same_v<double, LHS> && std::is_same_v<double, RHS> && std::is_same_v<double, RESULT>){
        ////                gemv_blocked_sse(__lhs, __rhs, __result);
        ////            } else if constexpr (std::is_same_v<float, LHS> && std::is_same_v<float, RHS> && std::is_same_v<float, RESULT>){
        ////                gemv(__lhs, __rhs, __result);
        ////            }else{
        ////                gemv(__lhs, __rhs, __result);
        ////            }
        ////#elif __AVX__

        ////            if constexpr (std::is_same_v<double, LHS> && std::is_same_v<double, RHS> && std::is_same_v<double, RESULT>){
        ////                gemv_blocked_avx(__lhs, __rhs, __result);
        ////            } else if constexpr (std::is_same_v<float, LHS> && std::is_same_v<float, RHS> && std::is_same_v<float, RESULT>){
        ////                gemv(__lhs, __rhs, __result);
        ////            }else{
        ////                gemv(__lhs, __rhs, __result);
        ////            }
        ////#else
        ////            gemv(__lhs, __rhs, __result);
        ////#endif
        ////            return ;
        //        }

        //        //check if vector matrix multiplication
        //        if constexpr (ColumnsLHS == 1){
        //            gevm(__lhs, __rhs, __result);
        //            return ;
        //        }

        //        //general matrix matrix multiplication
        //#if (defined(__SSE__) || defined(__AVX__))
        //        if constexpr ((RowsLHS == 3 && ColumnsLHS == 3 && RowsRHS == 3 && ColumnsRHS == 3)){
        //            gemm(__lhs, __rhs, __result);
        //            //irgendwas mit load??
        //            //computation_3_3(__lhs, __rhs, __result);
        //            return ;
        //        }
        //#endif



        //#if (defined(__SSE__) && !defined(__AVX__))
        //        if constexpr (std::is_same_v<double, LHS> && std::is_same_v<double, RHS> && std::is_same_v<double, RESULT>){
        //            gemm_blocked<2>(__lhs, __rhs, __result);
        //        } else if constexpr (std::is_same_v<float, LHS> && std::is_same_v<float, RHS> && std::is_same_v<float, RESULT>){
        //            gemm_blocked<4>(__lhs, __rhs, __result);
        //        }else{
        //            gemm(__lhs, __rhs, __result);
        //        }
        //        return ;
        //#elif __AVX__
        //        if constexpr (std::is_same_v<double, LHS> && std::is_same_v<double, RHS> && std::is_same_v<double, RESULT>){
        //            constexpr size_type Size{((RowsRHS >= 4 && ColumnsRHS >= 4 && RowsLHS >= 4 && ColumnsLHS >= 4) ? 4 : 2)};
        //            gemm_blocked<Size>(__lhs, __rhs, __result);
        //        } else if constexpr (std::is_same_v<float, LHS> && std::is_same_v<float, RHS> && std::is_same_v<float, RESULT>){
        //            constexpr size_type Size{((RowsRHS >= 8 && ColumnsRHS >= 8 && RowsLHS >= 8 && ColumnsLHS >= 8) ? 8 : 4)};
        //            gemm_blocked<Size>(__lhs, __rhs, __result);
        //        }else{
        //            gemm(__lhs, __rhs, __result);
        //        }
        //        return ;
        //#else
        //        gemm(__lhs, __rhs, __result);
        //        return ;
        //#endif
    }


    template<std::size_t BI, std::size_t BJ, std::size_t BK, std::size_t _LDA, std::size_t _LDB, std::size_t _LDC, typename T>
    static inline auto gemm_micro_kernal(T const* _A, T const* _B, T* _C)noexcept{

        for (std::size_t j = 0; j < BJ; ++j) {
            alignas(alignof(T)) std::array<T, BI> Ci;
            for (std::size_t i = 0; i < BI; ++i) {Ci[i] = _C[i*_LDC+j];}


            for (std::size_t k = 0; k < BK; ++k) {
                alignas(alignof(T)) std::array<T, BI> Ai;
                for (std::size_t i = 0; i < BI; ++i) {
                    Ai[i] = _A[i*_LDC+k];
                }

                for (std::size_t i = 0; i < BI; ++i) {
                    //Ci[i] += _A[i*_LDC+k]*_B[k*_LDB+j];
                    Ci[i] += Ai[i]*_B[k*_LDB+j];
                }
            }
            for (std::size_t i = 0; i < BI; ++i) {
                _C[i*_LDC+j] = Ci[i];
            }
        }
    }


    template<std::size_t BI, std::size_t BJ, std::size_t BK, std::size_t _LDA, std::size_t _LDB, std::size_t _LDC, typename T>
    static inline auto gemm_micro_kernal_non(T const* _A, T const* _B, T* _C, std::size_t NI, std::size_t NJ, std::size_t NK)noexcept{

        for (std::size_t i{0}; i < NI; ++i) {
            for (std::size_t j = 0; j < NJ; ++j) {
                T sum{_C[i*_LDC+j]};
                for (std::size_t k{0}; k < NK; ++k) {
                    sum += _A[i*_LDC+k]*_B[k*_LDB+j];
                }
                _C[i*_LDC+j] = sum;
            }
        }
    }


    static inline auto gemm_impl9(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        // micro kernal
        //i --> RowsLHS
        //j --> ColumnsRHS
        //k --> ColumnsLHS
        constexpr std::size_t Bi{(8 < RowsLHS ? 8 : RowsLHS)};
        constexpr std::size_t Bj{(16 < ColumnsRHS ? 16 : ColumnsRHS)};
        constexpr std::size_t Bk{(16 < ColumnsLHS ? 16 : ColumnsLHS)};
        // macro kernal
        constexpr std::size_t BI{(32 < RowsLHS ? 32 : RowsLHS)};
        constexpr std::size_t BJ{(64 < ColumnsRHS ? 64 : ColumnsRHS)};
        constexpr std::size_t BK{(64*2 < ColumnsLHS ? 64*2 : ColumnsLHS)};

        //std::memset(_C, 0, _Size*_Size*sizeof(T));

        for (std::size_t ii = 0; ii < RowsLHS - Bi+1; ii+=BI){
            for (std::size_t kk = 0; kk < ColumnsLHS - Bk+1 ; kk+=BK){
                for (std::size_t jj = 0; jj < ColumnsRHS - Bj+1; jj+=BJ){
                    if(kk == 0){
                            for (std::size_t j = jj; j < std::min(jj + BJ, ColumnsRHS); j+=Bj){
                                for (std::size_t i = ii; i < std::min(ii + BI, RowsLHS); i+=Bi){
                                __result[i*ColumnsLHS+j] = 0;
                            }
                        }
                    }
                    for (std::size_t i = ii; i < std::min(ii + BI, RowsLHS); i+=Bi){
                        const auto SI{(RowsLHS < i + Bi ? RowsLHS - i : Bi)};
                        for (std::size_t k = kk; k < std::min(kk + BK, ColumnsLHS); k+=Bk){
                            const auto SK{(ColumnsLHS < k + Bk ? ColumnsLHS - k : Bk)};
                            for (std::size_t j = jj; j < std::min(jj + BJ, ColumnsRHS); j+=Bj){
                                const auto SJ{(ColumnsRHS < j + Bj ? ColumnsRHS - j : Bj)};
                                if(SI == Bi && SJ == Bj && SK == Bk){
                                    gemm_micro_kernal<Bi, Bj, Bk, RowsLHS, ColumnsRHS, ColumnsLHS>(__lhs+i*ColumnsLHS+k, __rhs+k*ColumnsRHS+j, __result+i*ColumnsLHS+j);
                                }else{
                                    gemm_micro_kernal_non<Bi, Bj, Bk, RowsLHS, ColumnsRHS, ColumnsLHS>(__lhs+i*ColumnsLHS+k, __rhs+k*ColumnsRHS+j, __result+i*ColumnsLHS+j, SI, SJ, SK);
                                }
                            }
                        }
                    }

                }
            }
        }
    }

    static constexpr inline auto gemm_simple_bigger(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        //std::array<RHS, RowsRHS> temp;
        for(size_type i{0}; i<RowsLHS; ++i){
            for(size_type j{0}; j<ColumnsRHS; ++j){
                __result[i*ColumnsRHS+j] = 0;
            }
            for(size_type k{0}; k<RowsRHS; ++k){
                for(size_type j{0}; j<ColumnsRHS; ++j){
                    __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
            }
        }

//        for(size_type i{0}; i<RowsLHS; ++i){
//            for(size_type j{0}; j<ColumnsRHS; ++j){
//                __result[i*ColumnsRHS+j] = 0;
//            }
//            for(size_type k{0}; k<RowsRHS; ++k){
//                for(size_type j{0}; j<ColumnsRHS; ++j){
//                    __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//            }
//        }
    }

    static constexpr inline auto gemm_simple(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        //RHS temp[RowsRHS];
        for(size_type j{0}; j<ColumnsRHS; ++j){
            for(size_type i{0}; i<RowsLHS; ++i){
                __result[i*ColumnsRHS+j] = 0;
                for(size_type k{0}; k<RowsRHS; ++k){
                    __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
            }
        }
    }

    static constexpr inline auto gemm_simple_temp(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        //RHS temp[RowsRHS];
        std::array<RHS, RowsRHS> temp;
        for(size_type j{0}; j<ColumnsRHS; ++j){
            for(size_type k{0}; k<RowsRHS; ++k){
                temp[k] = __rhs[k*ColumnsRHS+j];
            }
            for(size_type i{0}; i<RowsLHS; ++i){
                __result[i*ColumnsRHS+j] = 0;
                for(size_type k{0}; k<RowsRHS; ++k){
                    __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*temp[k];//__rhs[k*ColumnsRHS+j];
                }
            }
        }
    }


    static constexpr inline auto gemv(LHS const * __lhs, RHS const* __rhs, RESULT * __result)noexcept{

        //#pragma GCC ivdep
        //        for(size_type i{0}; i<RowsLHS; ++i){
        //            __result[i] = 0;
        //        }

        //#pragma GCC ivdep
        //        for(size_type i{0}; i<RowsLHS; i+=4){
        //            for(size_type j{0}; j<RowsRHS; j+=4){
        //                for(size_type jj{0}; jj<4; ++jj){
        //                    for(size_type ii{0}; ii<4; ++ii){
        //                        __result[i+ii] += __lhs[(i+ii)*ColumnsLHS+(j+jj)]*__rhs[(j+jj)];
        //                    }
        //                }
        //            }
        //        }

        //        alignas(32) std::array<RESULT,4> temp{0};
        //        for(size_type i{0}; i<RowsLHS; i+=4){
        //            for(size_type j{0}; j<RowsRHS; ++j){
        //                //for(size_type ii{0}; ii<4; ++ii){
        //                    temp[0] += __lhs[(i+0)*ColumnsLHS+j]*__rhs[j];
        //                    temp[1] += __lhs[(i+1)*ColumnsLHS+j]*__rhs[j];
        //                    temp[2] += __lhs[(i+2)*ColumnsLHS+j]*__rhs[j];
        //                    temp[3] += __lhs[(i+3)*ColumnsLHS+j]*__rhs[j];
        //                //}
        //            }
        //            for(size_type ii{0}; ii<4; ++ii){
        //                __result[i+ii] = temp[ii];
        //            }
        //            //__result[i] = sum;
        //        }

//        for(size_type i{0}; i<RowsLHS; ++i){
//            __result[i] = 0;
//        }

//        if constexpr (RowsRHS == 4 && RowsLHS == 4){
//            const auto r1{__rhs[0]}, r2{__rhs[1]}, r3{__rhs[2]}, r4{__rhs[3]};
//            __result[0] = __lhs[0]  * r1 + __lhs[1]  * r2 + __lhs[2]  * r3 + __lhs[3]  * r4;
//            __result[1] = __lhs[4]  * r1 + __lhs[5]  * r2 + __lhs[6]  * r3 + __lhs[7]  * r4;
//            __result[2] = __lhs[8]  * r1 + __lhs[9]  * r2 + __lhs[10] * r3 + __lhs[11] * r4;
//            __result[3] = __lhs[12] * r1 + __lhs[13] * r2 + __lhs[14] * r3 + __lhs[15] * r4;

//            return ;
//        }

//        if constexpr (RowsRHS == 2){
//            for(size_type i{0}; i<RowsLHS; ++i){
//                __result[i] = __lhs[i*ColumnsLHS+0]*__rhs[0] +__lhs[i*ColumnsLHS+1]*__rhs[1];
//            }

//            return 0;
//        }

//        if constexpr (RowsRHS == 4){
//            for(size_type i{0}; i<RowsLHS; ++i){
//                __result[i] = __lhs[i*ColumnsLHS+0]*__rhs[0] +__lhs[i*ColumnsLHS+1]*__rhs[1] +
//                              __lhs[i*ColumnsLHS+2]*__rhs[2] +__lhs[i*ColumnsLHS+3]*__rhs[3];
//            }

//            return 0;
//        }


            //const auto idx{i*ColumnsLHS};
            //RESULT sum{0};
            //__result[i] = 0;

//        for(size_type i{0}; i<RowsLHS; ++i){
//            __result[i] = 0;
//        }

//        //const auto idx{i*ColumnsLHS};
//        //RESULT sum{0};
//        //__result[i] = 0;
//        for(size_type j{0}; j<RowsRHS; ++j){
//            for(size_type i{0}; i<RowsLHS; ++i){
//                __result[i] += __lhs[i*ColumnsLHS+j]*__rhs[j];
//            }
//            //__result[i] = sum;
//        }

//        if constexpr (RowsLHS == 6 && RowsRHS == 6){
//            for(size_type i{0}; i<RowsLHS; i+=2){
//                RESULT sum1{0},sum2{0};
//                for(size_type j{0}; j<RowsRHS; j+=2){
//                    const auto temp1{__rhs[j]};
//                    const auto temp2{__rhs[j+1]};
//                    sum1 += __lhs[i*ColumnsLHS+j]*temp1     + __lhs[i*ColumnsLHS+j+1]*temp2;
//                    sum2 += __lhs[(i+1)*ColumnsLHS+j]*temp1 + __lhs[(i+1)*ColumnsLHS+j+1]*temp2;
//                }
//                __result[i] = sum1;
//                __result[i] = sum2;
//            }
//            return ;
//        }

//        if constexpr (RowsLHS == 9){
//            for(size_type i{0}; i<8; i+=4){
//                RESULT sum[4]{0};
//                for(size_type j{0}; j<RowsRHS; ++j){
//                    const auto temp{__rhs[j]};
//                    for(size_type I{0}; I<4; ++I){
//                    sum[I] += __lhs[(i+I)*ColumnsLHS+j]*temp;
//                    }
//                }
//                for(size_type I{0}; I<4; ++I){
//                    __result[i+I] = sum[I];
//                }
//            }
//            return ;
//        }

        // 4x4 * 4x1
        // 9x9 * 9x1

//        if constexpr (RowsLHS == 4 && ColumnsLHS == 4 && RowsRHS == 4 && ColumnsRHS == 1 && std::is_same_v<double, RESULT>){
//            gemv_d_4x4_2(__lhs, __rhs, __result);
//            return ;
//        }

////            for(std::size_t i{0}; i<4; ++i){
////                res += _mm256_set_pd(__rhs[i], __rhs[i+4], __rhs[i+8],__rhs[i+12]) * _mm256_set1_pd(__lhs[i]);
////            }
////            _mm256_storeu_pd(__result, res);


//            // Load the vector into an AVX register
//            const __m256d vec = _mm256_loadu_pd(__rhs);  // Load the 4x1 vector

//            for(std::size_t i{0}; i<4; ++i){
//                const __m256d row0 = _mm256_load_pd(__lhs + i*4);          // Load the first row of the matrix
//                auto res0 = _mm256_mul_pd(row0, vec);
//                res0 = _mm256_hadd_pd(res0, res0);
//                res0 = _mm256_hadd_pd(res0, res0);
//                __result[i] = _mm256_cvtsd_f64(res0);
//            }

////            // Process each row of the matrix
////            const __m256d row1 = _mm256_loadu_pd(__lhs + 4);      // Load the second row of the matrix
////            const __m256d row2 = _mm256_loadu_pd(__lhs + 8);      // Load the third row of the matrix
////            const __m256d row3 = _mm256_loadu_pd(__lhs + 12);     // Load the fourth row of the matrix

////            // Perform element-wise multiplication
////            auto res0 = _mm256_mul_pd(row0, vec);
////            auto res1 = _mm256_mul_pd(row1, vec);
////            auto res2 = _mm256_mul_pd(row2, vec);
////            auto res3 = _mm256_mul_pd(row3, vec);

////            // Horizontal addition to sum the products
////            res0 = _mm256_hadd_pd(res0, res0);
////            res1 = _mm256_hadd_pd(res1, res1);
////            res2 = _mm256_hadd_pd(res2, res2);
////            res3 = _mm256_hadd_pd(res3, res3);

////            // Sum the two pairs in each result
////            res0 = _mm256_hadd_pd(res0, res0);
////            res1 = _mm256_hadd_pd(res1, res1);
////            res2 = _mm256_hadd_pd(res2, res2);
////            res3 = _mm256_hadd_pd(res3, res3);

////            // Extract the first element of each result and store it in the output array
////            __result[0] = _mm256_cvtsd_f64(res0);
////            __result[1] = _mm256_cvtsd_f64(res1);
////            __result[2] = _mm256_cvtsd_f64(res2);
////            __result[3] = _mm256_cvtsd_f64(res3);

//            return ;
//        }

//        // Initialize the result vector to zero
//        std::memset(__result, 0, RowsLHS * sizeof(double));
//        constexpr std::size_t blockSize{3};
//        // Perform blocked matrix-vector multiplication

//            for (int j = 0; j < RowsRHS; j += blockSize) {
//                const auto minJ{std::min(j + blockSize, RowsRHS)};
//            for (int i = 0; i < RowsLHS; i += blockSize) {
//                const auto minI{std::min(i + blockSize, RowsLHS)};
//                // Compute the block multiplication
//                for (int ii = i; ii < minI; ++ii) {
//                    for (int jj = j; jj <minJ; ++jj) {
//                        __result[ii] += __lhs[ii * ColumnsLHS + jj] * __rhs[jj];
//                    }
//                }
//            }
//        }

//        if constexpr (RowsLHS == 9 && ColumnsLHS == 9 && RowsRHS == 9 && ColumnsRHS == 1){
//            for(int i{0}; i<3; ++i){
//                for(int j{0}; j<3; ++j){
//                    //__result[i*3+j] = 0;
//                    RESULT sum{0};
//                    for(int k{0}; k<3; ++k){
//                        for(int l{0}; l<3; ++l){
//                            sum += __lhs[i*3*3*3+j*3*3+k*3+l]*__rhs[k*3+l];
//                        }
//                    }
//                    __result[i*3+j] = sum;
//                }
//            }
//            return ;
//        }

        //std::memset(__result, 0, RowsLHS * sizeof(RESULT));
        //std::array<RESULT, RowsLHS> temp;
//        for(size_type i{0}; i<RowsLHS; ++i){
//            for(size_type j{0}; j<ColumnsLHS; ++j){
//                temp[j*ColumnsLHS+i] = __lhs[i*ColumnsLHS+j];
//            }
//        }
//#pragma omp simd
        for(size_type i=0; i<RowsLHS; ++i){
            __result[i] = 0;
        for(size_type j=0; j<RowsRHS; ++j){
                __result[i] += __lhs[i*ColumnsLHS+j]*__rhs[j];
            }
        }


//        std::memset(__result, 0, RowsLHS * sizeof(RESULT));
//        for(size_type j{0}; j<RowsRHS; ++j){
//            const auto temp{__rhs[j]};
//            for(size_type i{0}; i<RowsLHS; ++i){
//                __result[i] += __lhs[i*ColumnsLHS+j]*temp;
//            }
//        }

        //        alignas(32) const LHS* lhs_ptr{__lhs};
        //        alignas(32) RESULT* result{__result};
        //        while(lhs_ptr != __lhs+RowsLHS*ColumnsLHS){
        //            alignas(32) const RHS* rhs_ptr{__rhs};
        //            *result = 0;
        //            while(rhs_ptr != __rhs+RowsRHS){
        //                *result += *lhs_ptr * *rhs_ptr;
        //                ++rhs_ptr;
        //                ++lhs_ptr;
        //            }
        //            ++result;
        //        }
    }

//    static inline auto transposed4x4(__m256d const& row1, __m256d const& row2, __m256d const& row3, __m256d const& row4)noexcept{
//        const auto rr1 = _mm256_unpacklo_pd(row1, row2);
//        const auto rr2 = _mm256_unpacklo_pd(row3, row4);
//        const auto rr3 = _mm256_unpackhi_pd(row1, row2);
//        const auto rr4 = _mm256_unpackhi_pd(row3, row4);
//        return std::make_tuple(_mm256_permute2f128_pd(rr1, rr2, 0x20),
//                               _mm256_permute2f128_pd(rr3, rr4, 0x20),
//                               _mm256_permute2f128_pd(rr1, rr2, 0x31),
//                               _mm256_permute2f128_pd(rr3, rr4, 0x31));
//    }

//    static constexpr inline auto gemv_d_4x4_2(LHS const* __lhs, RHS const* __rhs, RESULT * __result){
//        const auto [col1, col2, col3, col4]{transposed4x4(_mm256_load_pd(__lhs+0),
//                                                          _mm256_load_pd(__lhs+4),
//                                                          _mm256_load_pd(__lhs+8),
//                                                          _mm256_load_pd(__lhs+12))};
//        const auto v = _mm256_loadu_pd(__rhs);
//        _mm256_storeu_pd(__result, (col1*v + col2*v + col3*v + col4*v));
//    }

    static constexpr inline auto gevm(LHS const* __lhs, RHS const* __rhs, RESULT * __result)noexcept{
        for(size_type i{0}; i<RowsLHS; ++i){
            RESULT sum{0};
            for(size_type j{0}; j<RowsLHS; ++j){
                sum += __lhs[j]*__rhs[j*ColumnsRHS+i];
            }
            __result[i] = sum;
        }
    }


    //    static constexpr inline auto gemm(LHS const*const __lhs, RHS const*const __rhs, RESULT *const __result)noexcept{
    //        std::array<LHS, ColumnsLHS> temp{0};
    //        for(size_type i{0}; i<RowsLHS; ++i){
    //            for(size_type j{0}; j<ColumnsRHS; ++j){

    //            }
    //        }


    //        for(size_type i{0}; i<RowsLHS; ++i){
    //            for(size_type j{0}; j<ColumnsRHS; ++j){
    //                temp[j] = __lhs[i*ColumnsLHS+j];
    //            }
    //            size_type j{0};

    //            for(size_type jj{0}; j<ColumnsRHS-3; j+=4, ++jj){
    //                std::array<RESULT, 4> j_local{0};
    //                for(size_type jj{0}; j<ColumnsRHS-3; j+=4, ++jj){
    //                    for(size_type k{0}; k<RowsRHS; ++k){
    //                        j_local[jj] += temp[k]*__rhs[k*ColumnsRHS+j];
    //                    }
    //                }
    //            }
    //            for(std::size_t jj{0}; j<ColumnsRHS; ++j){

    //            }
    //            for(; j<ColumnsRHS; ++j){
    //                RESULT sum{0};
    //                for(size_type k{0}; k<RowsRHS; ++k){
    //                    sum += temp[k]*__rhs[k*ColumnsRHS+j];
    //                }
    //                __result[i*ColumnsRHS+j] = sum;
    //            }
    //        }
    //    }


//    static constexpr inline auto gemv_blocked_sse(double const* __lhs, double const* __rhs, double const* __result)noexcept{
//        constexpr auto Size{2ul};
//        for(std::size_t j{0}; j<(ColumnsRHS/Size)*Size; j+=Size){
//            __m128d yrow = _mm_loadu_pd(__rhs+j);
//            for(std::size_t i{0}; i<(RowsRHS/Size)*Size; i+=Size){
//                __m128d a = _mm_mul_pd(_mm_loadu_pd(__rhs + (i+0)*RowsRHS+j), yrow);
//                __m128d b = _mm_mul_pd(_mm_loadu_pd(__rhs + (i+1)*RowsRHS+j), yrow);
//                // {a[0]+a[1], b[0]+b[1]}
//                __m128d sumab = _mm_hadd_pd(a, b);
//                _mm_storeu_pd(const_cast<double*>(&__result[i]), sumab + _mm_loadu_pd(const_cast<double*>(&__result[i])));
//            }
//        }
//        //clean
//        for(std::size_t i{0}; i<RowsRHS; ++i){
//            RESULT sum{__result[i]};
//            for(std::size_t j{(ColumnsRHS/Size)*Size}; j<ColumnsRHS; ++j){
//                sum += __lhs[i*ColumnsLHS+j]*__rhs[j];
//            }
//            const_cast<double&>(__result[i]) = sum;
//        }
//    }


    template<std::size_t _Size>
    static constexpr inline auto gemm4x4_kernal(size_type __start_i, size_type __start_j, size_type __start_k, LHS const*const __lhs, RHS const*const __rhs, RESULT *const __result)noexcept{
        //std::array<std::array<RESULT,_Size>,_Size> temp{0};
        std::array<RESULT,_Size*_Size> temp{0};
        for(size_type i{0}; i<_Size; ++i){
            for(size_type j{0}; j<_Size; ++j){
                for(size_type k{0}; k<RowsRHS; ++k){
                    temp[i*_Size+j] += __lhs[(__start_i+i)*ColumnsLHS+k+__start_k]*__rhs[(k+__start_k)*ColumnsRHS+(j+__start_j)];
                }
            }
        }
        for(size_type i{0}; i<_Size; ++i){
            for(size_type j{0}; j<_Size; ++j){
                __result[(i+__start_i)*ColumnsRHS+j+__start_j] = temp[i*_Size+j];
            }
        }
    }


    static constexpr inline auto gemm(LHS const*const __lhs, RHS const*const __rhs, RESULT *const __result)noexcept{
        constexpr size_type BS{4};
        if constexpr (true/*RowsLHS/BS >= 1 && ColumnsRHS/BS >= 1*/){
            size_type i{0};
            for(; i<RowsLHS; i+=BS){
                for(size_type jj{0}; jj<ColumnsRHS; jj+=BS){
                    //for(size_type kk{0}; kk<RowsRHS; kk+=BS){
                    gemm4x4_kernal<BS>(i*BS, jj*BS, 0, __lhs, __rhs, __result);
                    //}
                }
            }

            //            for(; i<RowsLHS; i+=BS){
            //                for(size_type jj{0}; jj<ColumnsRHS; jj+=BS){
            //                    gemm4x4_kernal<BS>(i*BS, jj*BS, __lhs, __rhs, __result);
            //                }
            //            }

            for(; i<RowsLHS; ++i){
                for(size_type jj=0; jj<ColumnsRHS; ++jj){
                    RESULT sum{0};
                    for(size_type k{0}; k<RowsRHS; ++k){
                        sum += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+jj];
                    }
                    __result[i*ColumnsRHS+jj] = sum;
                }
            }
        }else{
            std::array<RHS, RowsRHS> temprhs{0};
            std::array<RHS, RowsRHS> templhs{0};
            for(size_type i{0}; i<RowsLHS; ++i){
                for(size_type k{0}; k<RowsRHS; ++k){
                    templhs[k] = __lhs[i*ColumnsLHS+k];
                }
                for(size_type j{0}; j<ColumnsRHS; ++j){
                    for(size_type k{0}; k<RowsRHS; ++k){
                        temprhs[k] = __rhs[k*ColumnsRHS+j];
                    }
                    RESULT sum{0};
                    for(size_type k{0}; k<RowsRHS; ++k){
                        //sum += __lhs[i*ColumnsLHS+k]*temprhs[k];//__rhs[k*ColumnsRHS+j];
                        sum += templhs[k]*temprhs[k];
                    }
                    __result[i*ColumnsRHS+j] = sum;
                }
            }
        }
    }

    static constexpr inline auto gemm2x2(LHS const*const __lhs, RHS const*const __rhs, RESULT * __result)noexcept{
        //aij = bik ckj
        //0 1
        //2 3
        __result[0] = __lhs[0]*__rhs[0] + __lhs[1]*__rhs[2];
        __result[1] = __lhs[0]*__rhs[1] + __lhs[1]*__rhs[3];
        __result[2] = __lhs[2]*__rhs[0] + __lhs[3]*__rhs[2];
        __result[3] = __lhs[2]*__rhs[1] + __lhs[3]*__rhs[3];
    }


    static constexpr inline auto gemm_block(LHS const*const __lhs, RHS const*const __rhs, RESULT * __result)noexcept{
        constexpr std::size_t NB = 16;

        for(size_type II{0}; II<RowsLHS; II+=NB){
            const auto i_size{std::min(II+NB, RowsLHS)};
            for(size_type JJ{0}; JJ<ColumnsRHS; JJ+=NB){
                const auto j_size{std::min(JJ+NB, ColumnsRHS)};
                for(size_type KK{0}; KK<RowsRHS; KK+=NB){
                    const auto k_size{std::min(KK+NB, RowsRHS)};
                    for(size_type k{KK}; k<k_size; ++k){
                        for(size_type j{JJ}; j<j_size; ++j){
                            for(size_type i{II}; i<i_size; ++i){
                                __result[i*ColumnsRHS+j] += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
                            }
                        }
                    }
                }
            }
        }
    }

};
//template <typename LHS, typename RHS, typename RESULT, std::size_t RowsLHS, std::size_t ColumnsLHS, std::size_t RowsRHS, std::size_t ColumnsRHS>
//std::array<RHS, RowsRHS*ColumnsRHS> gemm_wrapper<LHS, RHS, RESULT, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS >::temp;



//    template<size_type Size>
//    static constexpr inline auto gemm_blocked(double const* __lhs, double const* __rhs, double const* __result){

//        constexpr size_type IB{RowsLHS/Size};
//        constexpr size_type JB{ColumnsRHS/Size};
//        constexpr size_type KB{RowsRHS/Size};

//        for(size_type i{0}; i<RowsLHS*ColumnsRHS; ++i){
//            const_cast<double*>(__result)[i]=0;
//        }

//        for(size_type j{0}; j<JB*Size; j+=Size){
//            for(size_type i{0}; i<IB*Size; i+=Size){
//                for(size_type k{0}; k<KB*Size; k+=Size){
//                    if constexpr (Size == 2){
//                        gemm_block_sse(__lhs, __rhs, __result, i, j, k);
//                    }else{
//                        gemm_block_avx(__lhs, __rhs, __result, i, j, k);
//                    }
//                }
//            }
//        }

//        //clean k
//        if constexpr (KB*Size != RowsRHS){
//            for(size_type i{0}; i<IB*Size; ++i){
//                for(size_type j{0}; j<JB*Size; ++j){
//                    double sum{__result[i*ColumnsRHS+j]};
//                    for(size_type k{KB*Size}; k<RowsRHS; ++k){
//                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                    }
//                    const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
//                }
//            }
//        }

//        //clean j
//        if constexpr (JB*Size != ColumnsRHS){
//            for(size_type i{0}; i<RowsLHS; ++i){
//                for(size_type j{JB*Size}; j<ColumnsRHS; ++j){
//                    double sum{0};
//                    for(size_type k{0}; k<RowsRHS; ++k){
//                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                    }
//                    const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
//                }
//            }
//        }


//        //clean i
//        if constexpr (IB*Size != RowsLHS){
//            for(size_type i{IB*Size}; i<RowsLHS; ++i){
//                for(size_type j{0}; j<JB*Size; ++j){
//                    double sum{0};
//                    for(size_type k{0}; k<RowsRHS; ++k){
//                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                    }
//                    const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
//                }
//            }
//        }
//    }

//    template<size_type Size>
//    static constexpr inline auto gemm_blocked(float const* __lhs, float const* __rhs, float const* __result){
//        constexpr size_type IB{RowsLHS/Size};
//        constexpr size_type JB{ColumnsRHS/Size};
//        constexpr size_type KB{RowsRHS/Size};


//        for(size_type j{0}; j<JB*Size; j+=Size){
//            for(size_type i{0}; i<IB*Size; i+=Size){
//                for(size_type k{0}; k<KB*Size; k+=Size){
//                    if constexpr (Size == 4){
//                        gemm_block_sse(__lhs, __rhs, __result, i, j, k);
//                    }else{
//                        gemm_block_avx(__lhs, __rhs, __result, i, j, k);
//                    }
//                }
//            }
//        }

//        //clean k
//        for(size_type i{0}; i<IB*Size; ++i){
//            for(size_type j{0}; j<JB*Size; ++j){
//                float sum{__result[i*ColumnsRHS+j]};
//                for(size_type k{KB*Size}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                const_cast<float&>(__result[i*ColumnsRHS+j]) = sum;
//            }
//        }

//        //clean j
//        for(size_type i{0}; i<RowsLHS; ++i){
//            for(size_type j{JB*Size}; j<ColumnsRHS; ++j){
//                float sum{0};
//                for(size_type k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                const_cast<float&>(__result[i*ColumnsRHS+j]) = sum;
//            }
//        }

//        //clean i
//        for(size_type i{IB*Size}; i<RowsLHS; ++i){
//            for(size_type j{0}; j<JB*Size; ++j){
//                float sum{0};
//                for(size_type k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                const_cast<float&>(__result[i*ColumnsRHS+j]) = sum;
//            }
//        }
//    }

//    static constexpr inline auto gevm(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){
//        for(size_type j{0}; j<RowsLHS; ++j){
//            for(size_type i{0}; i<ColumnsLHS; ++i){
//                const_cast<RESULT&>(__result[i]) += __rhs[j]*__lhs[j*ColumnsLHS+i];
//            }
//        }
//    }



//    static constexpr inline auto gemv(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){
//        for(size_type i{0}; i<RowsLHS; ++i){
//            RESULT sum{0};
//            for(size_type j{0}; j<ColumnsLHS; ++j){
//                sum += __lhs[i*ColumnsLHS+j]*__rhs[j];
//            }
//            const_cast<RESULT&>(__result[i]) = sum;
//        }
//    }


//    static constexpr inline auto gemv_blocked_avx(double const* __lhs, double const* __rhs, double const* __result){
//        //kernal see https://gist.github.com/kbarbary/9efb3650f1b69b2b6b18e34ad347777b

//        constexpr auto Size{4ul};
//        for(std::size_t j{0}; j<(ColumnsRHS/Size)*Size; j+=Size){
//            __m256d yrow = _mm256_loadu_pd(__rhs+j);
//            for(std::size_t i{0}; i<(RowsRHS/Size)*Size; i+=Size){
//                __m256d a = _mm256_mul_pd(_mm256_loadu_pd(__rhs + (i+0)*RowsRHS+j), yrow);
//                __m256d b = _mm256_mul_pd(_mm256_loadu_pd(__rhs + (i+1)*RowsRHS+j), yrow);
//                __m256d c = _mm256_mul_pd(_mm256_loadu_pd(__rhs + (i+2)*RowsRHS+j), yrow);
//                __m256d d = _mm256_mul_pd(_mm256_loadu_pd(__rhs + (i+3)*RowsRHS+j), yrow);
//                // our task now is to get {sum(a), sum(b), sum(c), sum(d)}
//                // This is tricky because there is no hadd instruction for avx

//                // {a[0]+a[1], b[0]+b[1], a[2]+a[3], b[2]+b[3]}
//                __m256d sumab = _mm256_hadd_pd(a, b);

//                // {c[0]+c[1], d[0]+d[1], c[2]+c[3], d[2]+d[3]}
//                __m256d sumcd = _mm256_hadd_pd(c, d);

//                // {a[0]+a[1], b[0]+b[1], c[2]+c[3], d[2]+d[3]}
//                __m256d blend = _mm256_blend_pd(sumab, sumcd, 0b1100);

//                // {a[2]+a[3], b[2]+b[3], c[0]+c[1], d[0]+d[1]}
//                __m256d perm = _mm256_permute2f128_pd(sumab, sumcd, 0x21);

//                // {sum(a), sum(b), sum(c), sum(d)}
//                __m256d Ay = _mm256_add_pd(perm, blend);

//                _mm256_storeu_pd(const_cast<double*>(&__result[i]), Ay + _mm256_loadu_pd(const_cast<double*>(&__result[i])));
//            }
//        }

//        //clean
//        for(std::size_t i{0}; i<RowsRHS; ++i){
//            double sum{__result[i]};
//            for(std::size_t j{(ColumnsRHS/Size)*Size}; j<ColumnsRHS; ++j){
//                sum += __lhs[i*ColumnsLHS+j]*__rhs[j];
//            }
//            const_cast<double&>(__result[i]) = sum;
//        }
//    }




//    static constexpr inline void gemm_block_sse(double const* __lhs, double const* __rhs, double const* __result, std::size_t const I, std::size_t const J, std::size_t const K) {
//        __m128d row1 = _mm_loadu_pd(&__rhs[(K+0)*ColumnsRHS]);//two entries
//        __m128d row2 = _mm_loadu_pd(&__rhs[(K+1)*ColumnsRHS]);
//        for(int i=0; i<2; i++) {
//            __m128d row = _mm_loadu_pd(&__result[(i+I)*ColumnsRHS+J]);
//            __m128d brod1 = _mm_set1_pd(__lhs[(i+I)*ColumnsLHS + 0]);
//            __m128d brod2 = _mm_set1_pd(__lhs[(i+I)*ColumnsLHS + 1]);
//            row += row1*brod1 + row2*brod2;
//            _mm_storeu_pd(const_cast<double*>(&__result[(i+I)*ColumnsRHS+J]), row);
//        }
//    }

//    static constexpr inline void gemm_block_avx(double const* __lhs, double const* __rhs, double const* __result, std::size_t const I, std::size_t const J, std::size_t const K) {
//        const __m256d row1 = _mm256_loadu_pd(&__rhs[(K+0)*ColumnsRHS + J]);
//        const __m256d row2 = _mm256_loadu_pd(&__rhs[(K+1)*ColumnsRHS + J]);
//        const __m256d row3 = _mm256_loadu_pd(&__rhs[(K+2)*ColumnsRHS + J]);
//        const __m256d row4 = _mm256_loadu_pd(&__rhs[(K+3)*ColumnsRHS + J]);
//        for(int i=0; i<4; i++) {
//            __m256d row         = _mm256_loadu_pd(&__result[(i+I)*ColumnsRHS + J]);
//            const __m256d brod1 = _mm256_set1_pd(__lhs[(i+I)*ColumnsLHS + K + 0]);
//            const __m256d brod2 = _mm256_set1_pd(__lhs[(i+I)*ColumnsLHS + K + 1]);
//            const __m256d brod3 = _mm256_set1_pd(__lhs[(i+I)*ColumnsLHS + K + 2]);
//            const __m256d brod4 = _mm256_set1_pd(__lhs[(i+I)*ColumnsLHS + K + 3]);

//            row += brod1*row1 + brod2*row2 + brod3*row3 + brod4*row4;

//            _mm256_storeu_pd(const_cast<double*>(&__result[(i+I)*ColumnsRHS + J]), row);
//        }
//    }


//    static constexpr inline void gemm_block_sse(float const* __lhs, float const* __rhs, float const* __result, std::size_t const I, std::size_t const J, std::size_t const K) {
//        const __m128 row1 = _mm_loadu_ps(&__rhs[(K+0)*ColumnsRHS + J]);
//        const __m128 row2 = _mm_loadu_ps(&__rhs[(K+1)*ColumnsRHS + J]);
//        const __m128 row3 = _mm_loadu_ps(&__rhs[(K+2)*ColumnsRHS + J]);
//        const __m128 row4 = _mm_loadu_ps(&__rhs[(K+3)*ColumnsRHS + J]);
//        for(int i=0; i<4; i++) {
//            __m128 row         = _mm_loadu_ps(&__result[(i+I)*ColumnsRHS + J]);
//            const __m128 brod1 = _mm_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 0]);
//            const __m128 brod2 = _mm_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 1]);
//            const __m128 brod3 = _mm_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 2]);
//            const __m128 brod4 = _mm_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 3]);

//            row += brod1*row1 + brod2*row2 + brod3*row3 + brod4*row4;

//            _mm_storeu_ps(const_cast<float*>(&__result[(i+I)*ColumnsRHS + J]), row);
//        }
//    }

//    static constexpr inline void gemm_block_avx(float const* __lhs, float const* __rhs, float const* __result, std::size_t const I, std::size_t const J, std::size_t const K) {
//        const __m256 row1 = _mm256_loadu_ps(&__rhs[(K+0)*ColumnsRHS + J]);
//        const __m256 row2 = _mm256_loadu_ps(&__rhs[(K+1)*ColumnsRHS + J]);
//        const __m256 row3 = _mm256_loadu_ps(&__rhs[(K+2)*ColumnsRHS + J]);
//        const __m256 row4 = _mm256_loadu_ps(&__rhs[(K+3)*ColumnsRHS + J]);
//        const __m256 row5 = _mm256_loadu_ps(&__rhs[(K+4)*ColumnsRHS + J]);
//        const __m256 row6 = _mm256_loadu_ps(&__rhs[(K+5)*ColumnsRHS + J]);
//        const __m256 row7 = _mm256_loadu_ps(&__rhs[(K+6)*ColumnsRHS + J]);
//        const __m256 row8 = _mm256_loadu_ps(&__rhs[(K+7)*ColumnsRHS + J]);

//        for(int i=0; i<8; i++) {
//            __m256 row         = _mm256_loadu_ps(&__result[(i+I)*ColumnsRHS + J]);
//            const __m256 brod1 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 0]);
//            const __m256 brod2 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 1]);
//            const __m256 brod3 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 2]);
//            const __m256 brod4 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 3]);
//            const __m256 brod5 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 4]);
//            const __m256 brod6 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 5]);
//            const __m256 brod7 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 6]);
//            const __m256 brod8 = _mm256_set1_ps(__lhs[(i+I)*ColumnsLHS + K + 7]);


//            row += brod1*row1 + brod2*row2 + brod3*row3 + brod4*row4 + brod5*row5 + brod6*row6 + brod7*row7 + brod8*row8;

//            _mm256_storeu_ps(const_cast<float*>(&__result[(i+I)*ColumnsRHS + J]), row);
//        }
//    }


//    static constexpr inline void gemm_2x2_sse(double const* __lhs, double const* __rhs, double const* __result) {
//        __m128d row1 = _mm_load_pd(&__rhs[0]);
//        __m128d row2 = _mm_load_pd(&__rhs[2]);
//        for(int i=0; i<2; i++) {
//            __m128d brod1 = _mm_set1_pd(__lhs[i*ColumnsLHS + 0]);
//            __m128d brod2 = _mm_set1_pd(__lhs[i*ColumnsLHS + 1]);
//            _mm_store_pd(const_cast<double*>(&__result[i*ColumnsRHS]), _mm_mul_pd(brod1, row1) + _mm_mul_pd(brod2, row2));
//        }
//    }


//    static constexpr inline void gemm_4x4_avx(double const* __lhs, double const* __rhs, double const* __result) {
//        __m256d row1 = _mm256_load_pd(&__rhs[0]);//four entries
//        __m256d row2 = _mm256_load_pd(&__rhs[4]);
//        __m256d row3 = _mm256_load_pd(&__rhs[8]);
//        __m256d row4 = _mm256_load_pd(&__rhs[12]);
//        for(int i=0; i<4; i++) {
//            __m256d brod1 = _mm256_set1_pd(__lhs[i*ColumnsLHS + 0]);
//            __m256d brod2 = _mm256_set1_pd(__lhs[i*ColumnsLHS + 1]);
//            __m256d brod3 = _mm256_set1_pd(__lhs[i*ColumnsLHS + 2]);
//            __m256d brod4 = _mm256_set1_pd(__lhs[i*ColumnsLHS + 3]);
//            _mm256_store_pd(const_cast<double*>(&__result[i*ColumnsRHS]), brod1*row1 + brod2*row2 + brod3*row3 + brod4*row4);
//        }
//    }





//    static constexpr inline auto computation_3_3(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){
//        constexpr auto Size{2ul};
//        for(int i=0; i<3; i++) {
//            const __m128d rows[3]{_mm_set1_pd(__lhs[(i)*ColumnsLHS + 0]),
//                        _mm_set1_pd(__lhs[(i)*ColumnsLHS + 1]),
//                        _mm_set1_pd(__lhs[(i)*ColumnsLHS + 2])};

//            __m128d row{_mm_setzero_pd()};
//            for(int j=0; j<3; j++) {
//                row += _mm_load_pd(&__rhs[j*ColumnsRHS])*rows[j];
//            }
//            _mm_storeu_pd(const_cast<RESULT*>(&__result[(i)*ColumnsRHS]), row);
//        }

//        //clean j loop
//        for(std::size_t j{(ColumnsRHS/Size)*Size}; j<ColumnsRHS; ++j){
//            for(std::size_t i{0}; i<RowsLHS; ++i){
//                RESULT sum{0};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                const_cast<RESULT&>(__result[i*ColumnsRHS+j]) = sum;
//            }
//        }
//    }


//old stuff

/*

    static constexpr inline auto evaluate(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){

        if constexpr (ColumnsRHS == 1){
            computation_matrix_vector_product(__lhs, __rhs, __result);
        }

        if constexpr ((RowsLHS == 2 && ColumnsLHS == 2 && RowsRHS == 2 && ColumnsRHS == 2)){
//            _mm_prefetch((char*)(__result+4),_MM_HINT_T0);
//            _mm_prefetch((char*)(__lhs+4),_MM_HINT_T0);
//            _mm_prefetch((char*)(__rhs+4),_MM_HINT_T0);
            M2x2_SSE(__lhs, __rhs, __result);
            return ;
        }

        if constexpr ((RowsLHS == 3 && ColumnsLHS == 3 && RowsRHS == 3 && ColumnsRHS == 3)){
            computation_3_3(__lhs, __rhs, __result);
            return ;
        }

        if constexpr ((RowsLHS == 4 && ColumnsLHS == 4 && RowsRHS == 4 && ColumnsRHS == 4)){
//            _mm_prefetch((char*)(__result+16),_MM_HINT_T0);
//            _mm_prefetch((char*)(__lhs+16),_MM_HINT_T0);
//            _mm_prefetch((char*)(__rhs+16),_MM_HINT_T0);
            M4x4_SSE(__lhs, __rhs, __result);
            return ;
        }

        if constexpr ((RowsLHS == 8 && ColumnsLHS == 8 && RowsRHS == 8 && ColumnsRHS == 8)){
            computation_8_8(__lhs, __rhs, __result);
            return ;
        }

        if constexpr ((RowsLHS == 9 && ColumnsLHS == 9 && RowsRHS == 9 && ColumnsRHS == 9)){
            computation_9_9(__lhs, __rhs, __result);
            return ;
        }

        general_computation(__lhs, __rhs, __result);
    }

private:
    static constexpr inline auto computation_2_2(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){
        for(std::size_t i{0}; i<2; ++i){
            for(std::size_t j{0}; j<2; ++j){
                RESULT sum{0};
                for(std::size_t k{0}; k<2; ++k){
                    sum = __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
                const_cast<double&>(__result[i*ColumnsRHS+j]) += sum;
            }
        }
    }

    static constexpr inline auto computation_4_4(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){
        for(std::size_t i{0}; i<4; ++i){
            for(std::size_t j{0}; j<4; ++j){
                RESULT sum{0};
                for(std::size_t k{0}; k<4; ++k){
                    sum = __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
                const_cast<double&>(__result[i*ColumnsRHS+j]) += sum;
            }
        }
    }



    static constexpr inline auto computation_8_8(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){

        //memset(__result, 0, RowsLHS*ColumnsRHS*sizeof(*__result));

        constexpr std::size_t StepSize{4};
        constexpr std::size_t IB{RowsLHS/StepSize};
        constexpr std::size_t JB{ColumnsRHS/StepSize};
        constexpr std::size_t KB{RowsRHS/StepSize};

        for(std::size_t i{0}; i<(RowsLHS/4)*4; i+=4){
            for(std::size_t j{0}; j<(ColumnsRHS/4)*4; j+=4){
                for(std::size_t k{0}; k<(RowsRHS/4)*4; k+=4){
                    M4x4_SSE_block(__lhs, __rhs, __result, i, j, k);
                }
            }
        }

//        //clean k
//        for(std::size_t i{0}; i<(RowsLHS/4)*4; ++i){
//            for(std::size_t j{0}; j<(ColumnsRHS/4)*4; ++j){
//                RESULT sum{__result[i*ColumnsRHS+j]};
//                for(std::size_t k{(RowsRHS/4)*4}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                __result[i*ColumnsRHS+j] = sum;
//            }
//        }

    }

    static constexpr inline auto compute_block_2_2(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){

        constexpr std::size_t StepSize{2};
        constexpr std::size_t IB{RowsLHS/StepSize};
        constexpr std::size_t JB{ColumnsRHS/StepSize};
        constexpr std::size_t KB{RowsRHS/StepSize};

        for(std::size_t j{0}; j<(ColumnsRHS/StepSize)*StepSize; j+=StepSize){
            for(std::size_t i{0}; i<(RowsLHS/StepSize)*StepSize; i+=StepSize){
                for(std::size_t k{0}; k<(RowsRHS/StepSize)*StepSize; k+=StepSize){
                    M2x2_SSE_block(__lhs, __rhs, __result, i, j, k);
                }
            }
        }

        //clean k
        for(std::size_t i{0}; i<(RowsLHS/StepSize)*StepSize; ++i){
            for(std::size_t j{0}; j<(ColumnsRHS/StepSize)*StepSize; ++j){
                RESULT sum{__result[i*ColumnsRHS+j]};
                for(std::size_t k{(RowsRHS/StepSize)*StepSize}; k<RowsRHS; ++k){
                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
                const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
            }
        }

        //clean j
        for(std::size_t i{0}; i<RowsLHS; ++i){
            for(std::size_t j{JB*StepSize}; j<ColumnsRHS; ++j){
                RESULT sum{__result[i*ColumnsRHS+j]};
                for(std::size_t k{0}; k<RowsRHS; ++k){
                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
                const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
            }
        }

        //clean i
        for(std::size_t i{IB*StepSize}; i<RowsLHS; ++i){
            for(std::size_t j{0}; j<JB*StepSize; ++j){
                RESULT sum{__result[i*ColumnsRHS+j]};
                for(std::size_t k{0}; k<RowsRHS; ++k){
                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
                const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
            }
        }
    }

    static constexpr inline auto computation_9_9(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){

        //memset(__result, 0, RowsLHS*ColumnsRHS*sizeof(*__result));

        //c11 = a11*b11 + a12*b21 + a13*b31 + a14*b31
        //c12 = a11*b12 + a12*b22 + a13*b32 + a14*b32
        //c13 = a11*b13 + a12*b23 + a13*b33 + a14*b33
        //c14 = a11*b14 + a12*b24 + a13*b34 + a14*b34

        //c21 = a21*b11 + a22*b21 + a23*b31;
        //c22 = a21*b12 + a22*b22 + a23*b32;
        //c23 = a21*b13 + a22*b23 + a23*b33;
        //c24 = a21*b14 + a22*b24 + a23*b34;

        //c31 = a31*b11 + a32*b21 + a33*b31;
        //c32 = a31*b12 + a32*b22 + a33*b32;
        //c33 = a31*b13 + a32*b23 + a33*b33;
        //c34 = a31*b14 + a32*b24 + a33*b34;

        //c15 = a11*b15 + a12*b25 + a13*b35;
        //c16 = a11*b16 + a12*b26 + a13*b36;
        //c17 = a11*b17 + a12*b27 + a13*b37;
        //c18 = a11*b18 + a12*b28 + a13*b38;

        constexpr std::size_t StepSize{4};
        constexpr std::size_t IB{RowsLHS/StepSize};
        constexpr std::size_t JB{ColumnsRHS/StepSize};
        constexpr std::size_t KB{RowsRHS/StepSize};

//        for(std::size_t JJ{0}; JJ<JB; ++JJ){
//            const auto SJ{JJ*StepSize};
//            for(std::size_t II{0}; II<IB; ++II){
//                const auto SI{II*StepSize};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    const auto rows{_mm256_loadu_pd(&__rhs[(k)*ColumnsRHS+JJ])};
//                    for(std::size_t i{SI}; i<SI+StepSize; ++i){
//                        __m256d row{_mm256_loadu_pd(&__result[(i)*ColumnsRHS+JJ])};
//                        row += rows*_mm256_set1_pd(__lhs[(i)*ColumnsLHS + k]);
//                        _mm256_storeu_pd(&__result[(i)*ColumnsRHS+JJ], row);
//                    }
//                }
//            }
//        }


//        for(std::size_t II{0}; II<IB; ++II){
//            const auto SI{II*StepSize};
//            for(std::size_t JJ{0}; JJ<JB; ++JJ){
//                const auto SJ{JJ*StepSize};
//                //for(std::size_t KK{0}; KK<KB; ++KK){
//                //    const auto SK{KK*StepSize};

//                    for(std::size_t i{SI}; i<SI+StepSize; ++i){
//                        for(std::size_t j{SJ}; j<SJ+StepSize; ++j){
//                            RESULT sum{__result[i*ColumnsRHS+j]};
//                            for(std::size_t k{0}; k<RowsRHS; ++k){
//                                sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                            }
//                            __result[i*ColumnsRHS+j] = sum;
//                        }
//                    }

//                //}
//            }
//        }

        //general_computation(__lhs, __rhs, __result);
        //return ;

//        general_computation_small(__lhs, __rhs, __result);
//        return ;

        constexpr size_type ROWS{9};


//        for(size_type i{0}; i<ROWS; i+=3){
//            __m256d sum1{_mm256_setzero_pd()};
//            __m256d sum2{_mm256_setzero_pd()};
//            __m256d sum3{_mm256_setzero_pd()};
//            __m256d sum4{_mm256_setzero_pd()};
//            __m256d sum5{_mm256_setzero_pd()};
//            __m256d sum6{_mm256_setzero_pd()};

//            for(size_type k{0}; k<ROWS; k+=3){
//                const __m256d rhs1{_mm256_loadu_pd(&__rhs[k*ROWS+0])};
//                const __m256d rhs2{_mm256_loadu_pd(&__rhs[k*ROWS+4])};

//                const __m256d rhs3{_mm256_loadu_pd(&__rhs[(k+1)*ROWS+0])};
//                const __m256d rhs4{_mm256_loadu_pd(&__rhs[(k+1)*ROWS+4])};

//                const __m256d rhs5{_mm256_loadu_pd(&__rhs[(k+2)*ROWS+0])};
//                const __m256d rhs6{_mm256_loadu_pd(&__rhs[(k+2)*ROWS+4])};

//                {
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[i*ROWS + k])};
//                        sum1 += val_vec * rhs1;
//                        sum2 += val_vec * rhs2;
//                    }
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[i*ROWS + k+1])};
//                        sum1 += val_vec * rhs3;
//                        sum2 += val_vec * rhs4;
//                    }
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[i*ROWS + k+2])};
//                        sum1 += val_vec * rhs5;
//                        sum2 += val_vec * rhs6;
//                    }
//                }
//                {
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[(i+1)*ROWS + k])};
//                        sum3 += val_vec * rhs1;
//                        sum4 += val_vec * rhs2;
//                    }
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[(i+1)*ROWS + k+1])};
//                        sum3 += val_vec * rhs3;
//                        sum4 += val_vec * rhs4;
//                    }
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[(i+1)*ROWS + k+2])};
//                        sum3 += val_vec * rhs5;
//                        sum4 += val_vec * rhs6;
//                    }
//                }
//                {
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[(i+2)*ROWS + k])};
//                        sum5 += val_vec * rhs1;
//                        sum6 += val_vec * rhs2;
//                    }
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[(i+2)*ROWS + k+1])};
//                        sum5 += val_vec * rhs3;
//                        sum6 += val_vec * rhs4;
//                    }
//                    {
//                        const __m256d val_vec{_mm256_set1_pd(__lhs[(i+2)*ROWS + k+2])};
//                        sum5 += val_vec * rhs5;
//                        sum6 += val_vec * rhs6;
//                    }
//                }
//            }
//            _mm256_storeu_pd(&__result[i*ROWS+0], sum1);
//            _mm256_storeu_pd(&__result[i*ROWS+4], sum2);

//            _mm256_storeu_pd(&__result[(i+1)*ROWS+0], sum3);
//            _mm256_storeu_pd(&__result[(i+1)*ROWS+4], sum4);

//            _mm256_storeu_pd(&__result[(i+2)*ROWS+0], sum5);
//            _mm256_storeu_pd(&__result[(i+2)*ROWS+4], sum6);
//        }


//        for(size_type i{0}; i<ROWS; i+=3){
//            __m256d sum1{_mm256_setzero_pd()};
//            __m256d sum2{_mm256_setzero_pd()};
//            __m256d sum3{_mm256_setzero_pd()};
//            __m256d sum4{_mm256_setzero_pd()};
//            __m256d sum5{_mm256_setzero_pd()};
//            __m256d sum6{_mm256_setzero_pd()};

//            for(size_type k{0}; k<ROWS; ++k){
//                const __m256d rhs1{_mm256_loadu_pd(&__rhs[k*ROWS+0])};
//                const __m256d rhs2{_mm256_loadu_pd(&__rhs[k*ROWS+4])};
//                {
//                    const __m256d val_vec{_mm256_set1_pd(__lhs[i*ROWS + k])};
//                    sum1 += val_vec * rhs1;
//                    sum2 += val_vec * rhs2;
//                }
//                {
//                    const __m256d val_vec{_mm256_set1_pd(__lhs[(i+1)*ROWS + k])};
//                    sum3 += val_vec * rhs1;
//                    sum4 += val_vec * rhs2;
//                }
//                {
//                    const __m256d val_vec{_mm256_set1_pd(__lhs[(i+2)*ROWS + k])};
//                    sum5 += val_vec * rhs1;
//                    sum6 += val_vec * rhs2;
//                }
//            }
//            _mm256_storeu_pd(&__result[i*ROWS+0], sum1);
//            _mm256_storeu_pd(&__result[i*ROWS+4], sum2);

//            _mm256_storeu_pd(&__result[(i+1)*ROWS+0], sum3);
//            _mm256_storeu_pd(&__result[(i+1)*ROWS+4], sum4);

//            _mm256_storeu_pd(&__result[(i+2)*ROWS+0], sum5);
//            _mm256_storeu_pd(&__result[(i+2)*ROWS+4], sum6);
//        }


        //_mm_prefetch((char*)(__rhs+RowsRHS*ColumnsRHS),_MM_HINT_T1);


//        for(size_type i{0}; i<ROWS; ++i){
////            if(i % 2 == 0){
////                _mm_prefetch((char*)(__result+i*ROWS+18),_MM_HINT_T0);
////                _mm_prefetch((char*)(__lhs+i*ROWS+18),_MM_HINT_T0);
////            }


//            __m256d sum1{_mm256_setzero_pd()};
//            __m256d sum2{_mm256_setzero_pd()};

//            for(size_type k{0}; k<ROWS; ++k){
//                //_mm_prefetch((char*)(__rhs+k*ROWS+8),_MM_HINT_T0);

//                const __m256d rhs1{_mm256_loadu_pd(&__rhs[k*ROWS+0])};
//                const __m256d rhs2{_mm256_loadu_pd(&__rhs[k*ROWS+4])};
//                    const __m256d val_vec{_mm256_set1_pd(__lhs[i*ROWS + k])};
//                    sum1 += val_vec * rhs1;
//                    sum2 += val_vec * rhs2;
//            }
//            _mm256_storeu_pd(const_cast<double*>(&__result[i*ROWS+0]), sum1);
//            _mm256_storeu_pd(const_cast<double*>(&__result[i*ROWS+4]), sum2);
//        }


//        //clean j
//        for(std::size_t i{0}; i<RowsLHS; ++i){
//            for(std::size_t j{JB*StepSize}; j<ColumnsRHS; ++j){
//                RESULT sum{__result[i*ColumnsRHS+j]};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
//            }
//        }

//        return ;



//        for(size_type i{0}; i<ROWS; ++i){
//            __m128d sum[4]{0};
//            for(size_type k{0}; k<ROWS; ++k){
//                const auto val_vec = _mm_set1_pd(__lhs[i*ROWS + k]);
//                for(size_type jj{0}; jj<4; ++jj){
//                    sum[jj] += val_vec * _mm_loadu_pd(&__rhs[k*ROWS+jj*2]);
//                }
//            }
//            for(size_type jj{0}; jj<4; ++jj){
//                _mm_storeu_pd(const_cast<double*>(&__result[i*ROWS+jj*2]), sum[jj]);
//            }
//        }

//        //clean j
//        for(std::size_t i{0}; i<RowsLHS; ++i){
//            for(std::size_t j{JB*StepSize}; j<ColumnsRHS; ++j){
//                RESULT sum{__result[i*ColumnsRHS+j]};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
//            }
//        }



//        return ;






        for(std::size_t j{0}; j<(ColumnsRHS/4)*4; j+=4){
            for(std::size_t i{0}; i<(RowsLHS/4)*4; i+=4){
            for(std::size_t k{0}; k<(RowsRHS/4)*4; k+=4){
                M4x4_SSE_block(__lhs, __rhs, __result, i, j, k);
            }
        }
    }

    //clean k
    for(std::size_t i{0}; i<(RowsLHS/4)*4; ++i){
        for(std::size_t j{0}; j<(ColumnsRHS/4)*4; ++j){
            RESULT sum{__result[i*ColumnsRHS+j]};
            for(std::size_t k{(RowsRHS/4)*4}; k<RowsRHS; ++k){
                sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
            }
            const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
        }
    }

    //clean j
    for(std::size_t i{0}; i<RowsLHS; ++i){
        for(std::size_t j{JB*StepSize}; j<ColumnsRHS; ++j){
            RESULT sum{__result[i*ColumnsRHS+j]};
            for(std::size_t k{0}; k<RowsRHS; ++k){
                sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
            }
            const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
        }
    }

    //clean i
    for(std::size_t i{IB*StepSize}; i<RowsLHS; ++i){
        for(std::size_t j{0}; j<JB*StepSize; ++j){
            RESULT sum{__result[i*ColumnsRHS+j]};
            for(std::size_t k{0}; k<RowsRHS; ++k){
                sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
            }
            const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
        }
    }

    return ;




//        for(std::size_t II{0}; II<IB+1; ++II){
//            const auto SI{II*StepSize};
//            const auto MI{std::min(SI+StepSize, RowsLHS)};
//            for(std::size_t JJ{0}; JJ<JB+1; ++JJ){
//                const auto SJ{JJ*StepSize};
//                const auto MJ{std::min(SJ+StepSize, ColumnsRHS)};
//                for(std::size_t KK{0}; KK<KB+1; ++KK){
//                    const auto SK{KK*StepSize};
//                    const auto MK{std::min(SK+StepSize, RowsRHS)};
//                    for(std::size_t i{SI}; i<MI; ++i){
//                        for(std::size_t j{SJ}; j<MJ; ++j){
//                            RESULT sum{__result[i*ColumnsRHS+j]};
//                            for(std::size_t k{SK}; k<MK; ++k){
//                                sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                            }
//                            __result[i*ColumnsRHS+j] = sum;
//                        }
//                    }

//                }
//            }
//        }


        //return ;


//        constexpr auto StepsSize{4ul};

//            for(std::size_t j{0}; j<JB; ++j){
//                __m256d rows[9];
//                rows[0] = _mm256_loadu_pd(&__rhs[(j*StepsSize+0)*ColumnsRHS+j*StepsSize]);
//                rows[1] = _mm256_loadu_pd(&__rhs[(j*StepsSize+1)*ColumnsRHS+j*StepsSize]);
//                rows[2] = _mm256_loadu_pd(&__rhs[(j*StepsSize+2)*ColumnsRHS+j*StepsSize]);
//                rows[3] = _mm256_loadu_pd(&__rhs[(j*StepsSize+3)*ColumnsRHS+j*StepsSize]);
//                rows[4] = _mm256_loadu_pd(&__rhs[(j*StepsSize+4)*ColumnsRHS+j*StepsSize]);
//                rows[5] = _mm256_loadu_pd(&__rhs[(j*StepsSize+5)*ColumnsRHS+j*StepsSize]);
//                rows[6] = _mm256_loadu_pd(&__rhs[(j*StepsSize+6)*ColumnsRHS+j*StepsSize]);
//                rows[7] = _mm256_loadu_pd(&__rhs[(j*StepsSize+7)*ColumnsRHS+j*StepsSize]);
//                rows[8] = _mm256_loadu_pd(&__rhs[(j*StepsSize+8)*ColumnsRHS+j*StepsSize]);

//                for(std::size_t i{0}; i<9; ++i){
//                __m256d row{_mm256_loadu_pd(&__result[(i)*ColumnsRHS]+j*StepsSize)};
//                for(std::size_t k{0}; k<9; ++k){
//                    row += rows[k]*_mm256_set1_pd(__lhs[(i)*ColumnsLHS + k + j*StepsSize]);
//                }
//                _mm256_storeu_pd(const_cast<RESULT*>(&__result[(i)*ColumnsRHS+j*StepsSize]), row);
//            }
//        }

//            for(std::size_t i{0}; i<9; ++i){
//                for(std::size_t j{8}; j<9; ++j){
//                    RESULT sum{0};
//                    for(std::size_t k{0}; k<9; ++k){
//                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                    }
//                    const_cast<RESULT&>(__result[i*ColumnsRHS+j]) = sum;
//                }
//            }




//            for(std::size_t i{0}; i<9; ++i){
//                for(std::size_t j{0}; j<9; ++j){
//                RESULT sum{0};
//                for(std::size_t k{0}; k<9; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                __result[i*ColumnsRHS+j] = sum;
//            }
//        }


//        for(std::size_t ii{0}; ii<2; ++ii){
//            __m256d rows[4];
//            rows[0] = _mm256_loadu_pd(&__rhs[(ii*4+0)*ColumnsRHS]);
//            rows[1] = _mm256_loadu_pd(&__rhs[(ii*4+1)*ColumnsRHS]);
//            rows[2] = _mm256_loadu_pd(&__rhs[(ii*4+2)*ColumnsRHS]);
//            rows[3] = _mm256_loadu_pd(&__rhs[(ii*4+3)*ColumnsRHS]);
//            for(std::size_t i=0; i<4; i++) {
//                __m256d row{_mm256_loadu_pd(&__result[(ii*4+i)*ColumnsRHS])};
//                for(std::size_t j=0; j<4; j++) {
//                    row += rows[j]*_mm256_set1_pd(__lhs[(i)*ColumnsLHS + j]);
//                }
//                _mm256_storeu_pd(&__result[(ii*4+i)*ColumnsRHS], row);
//            }
//        }




//        //clean j loop
//        for(std::size_t i{0}; i<RowsLHS; ++i){
//            for(std::size_t j{(ColumnsRHS/StepSize)*StepSize}; j<ColumnsRHS; ++j){
//                RESULT sum{0};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                __result[i*ColumnsRHS+j] += sum;
//            }
//        }

//        alignas(32) RHS temp[ColumnsRHS];
//        for(std::size_t j{0}; j<ColumnsRHS; ++j){
//            for(std::size_t k{0}; k<RowsRHS; ++k){
//                temp[k] = __rhs[k*ColumnsRHS+j];
//            }
//            for(std::size_t i{0}; i<RowsLHS; ++i){
//                RESULT sum{0};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*temp[k];
//                }
//                __result[i*ColumnsRHS+j] = sum;
//            }
//        }
    }



    static constexpr inline auto general_computation_small(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){
            for(std::size_t i{0}; i<RowsLHS; ++i){
                for(std::size_t j{0}; j<ColumnsRHS; ++j){
                    RESULT sum{0};
                    for(std::size_t k{0}; k<RowsRHS; ++k){
                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
                    }
                    const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
                }
            }
    }
    static constexpr inline auto general_computation(LHS const* __lhs, RHS const* __rhs, RESULT const* __result){

//        //~130-135ns
//        alignas(32) RHS temp[ColumnsRHS];
//        alignas(32) RESULT temp_res[RowsRHS];

//        for(std::size_t j{0}; j<ColumnsRHS; ++j){
//            for(std::size_t k{0}; k<RowsRHS; ++k){
//                temp[k] = __rhs[k*ColumnsRHS+j];
//            }
//            for(std::size_t i{0}; i<RowsLHS; ++i){
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    temp_res[i] += __lhs[i*RowsLHS+k]*temp[k];
//                }
//            }

//            for(std::size_t k{0}; k<RowsRHS; ++k){
//                __result[k*ColumnsRHS+j] = temp_res[k];
//            }
//        }

//        return ;

//        //~120ns
//        RHS temp[ColumnsRHS*RowsRHS];
//        for(std::size_t j{0}; j<ColumnsRHS; ++j){
//            for(std::size_t k{0}; k<RowsRHS; ++k){
//                temp[k*ColumnsRHS+j] = __rhs[j*ColumnsRHS+k];
//            }
//        }

//        for(std::size_t j{0}; j<ColumnsRHS; ++j){
//            for(std::size_t i{0}; i<RowsLHS; ++i){
//                RESULT sum{0};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*temp[j*ColumnsRHS+k];
//                }
//                __result[i*ColumnsRHS+j] = sum;
//            }
//        }

//        //        return ;

//        //~50ns
//        for(std::size_t i{0}; i<RowsLHS; ++i){
//            for(std::size_t j{0}; j<ColumnsRHS; ++j){
//                RESULT sum{0};
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                }
//                __result[i*ColumnsRHS+j] = sum;
//            }
//        }
//        return ;



        //_mm_prefetch((char*)(__result+ColumnsRHS*RowsLHS),_MM_HINT_T1);
        //_mm_prefetch((char*)(__rhs+RowsRHS*ColumnsRHS),_MM_HINT_T1);
        //_mm_prefetch((char*)(__lhs+RowsLHS*ColumnsLHS),_MM_HINT_T1);



        //~40-45ns
        //alignas(32) RHS temp[ColumnsRHS];
        //            for(std::size_t k{0}; k<RowsRHS; ++k){
        //                temp[k] = __rhs[k*ColumnsRHS+j];
        //            }

        for(std::size_t j{0}; j<ColumnsRHS; ++j){
            for(std::size_t i{0}; i<RowsLHS; ++i){
                RESULT sum{0};
                for(std::size_t k{0}; k<RowsRHS; ++k){
                    sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];;
                }
                const_cast<double&>(__result[i*ColumnsRHS+j]) = sum;
            }
        }

        return ;





////        if constexpr (RowsLHS == 4 && ColumnsLHS == 4 && RowsRHS == 4 && ColumnsRHS == 4){
////            //std::cout<<"4x4"<<std::endl;
////            //memset(__result, 0, RowsLHS*ColumnsRHS*sizeof(*__result));
////            M4x4_SSE(__lhs, __rhs, __result);
//////            for(std::size_t i{0}; i<RowsLHS; ++i){
//////                for(std::size_t j{0}; j<ColumnsRHS; ++j){
//////                    RESULT sum{0};
//////                    for(std::size_t k{0}; k<RowsRHS; ++k){
//////                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//////                    }
//////                    __result[i*ColumnsRHS+j] = sum;
//////                }
//////            }
////            return ;
////        }

////        if constexpr (RowsLHS < 4 && ColumnsLHS < 4 && RowsRHS < 4 && ColumnsRHS < 4){
////            for(std::size_t i{0}; i<RowsLHS; ++i){
////                for(std::size_t j{0}; j<ColumnsRHS; ++j){
////                    RESULT sum{0};
////                    for(std::size_t k{0}; k<RowsRHS; ++k){
////                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
////                    }
////                    __result[i*ColumnsRHS+j] = sum;
////                }
////            }

////            return ;
////        }

////        if constexpr (RowsLHS == 9 && ColumnsLHS == 9 && RowsRHS == 9 && ColumnsRHS == 9){

////            RHS temp[ColumnsRHS];
////            for(std::size_t j{0}; j<ColumnsRHS; ++j){
////                for(int k{8}; k>=0; --k){
////                    temp[k] = __rhs[k*ColumnsRHS+j];
////                }
////            for(std::size_t i{0}; i<RowsLHS; ++i){
////                    RESULT sum{0};
////                    for(std::size_t k{0}; k<RowsRHS; ++k){
////                        sum += __lhs[i*RowsLHS+k]*temp[k];
////                    }
////                    __result[i*ColumnsRHS+j] = sum;
////                }
////            }

////            return ;
////        }
//        if constexpr (RowsLHS > 9 && ColumnsLHS > 9 && RowsRHS > 9 && ColumnsRHS > 9){
//            for(std::size_t i{0}; i<RowsLHS; ++i){
//                for(std::size_t j{0}; j<ColumnsRHS; ++j){
//                    RESULT sum{0};
//                    for(std::size_t k{0}; k<RowsRHS; ++k){
//                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
//                    }
//                    __result[i*ColumnsRHS+j] = sum;
//                }
//            }
//        }else{
////            RHS temp[ColumnsRHS];
////            for(std::size_t j{0}; j<ColumnsRHS; ++j){
////                for(std::size_t k{0}; k<RowsRHS; ++k){
////                    temp[k] = __rhs[k*ColumnsRHS+j];
////                }
////                for(std::size_t i{0}; i<RowsLHS; ++i){
////                    RESULT sum{0};
////                    for(std::size_t k{0}; k<RowsRHS; ++k){
////                        sum += __lhs[i*RowsLHS+k]*temp[k];
////                    }
////                    __result[i*ColumnsRHS+j] = sum;
////                }
////            }















//        if constexpr (RowsLHS >= 4 && ColumnsLHS >= 4 && RowsRHS >= 4 && ColumnsRHS >= 4){
//            memset(__result, 0, RowsLHS*ColumnsRHS*sizeof(*__result));


//            //        for(std::size_t i{0}; i<(RowsLHS/4)*4; i+=4){
//            //            for(std::size_t j{0}; j<(ColumnsRHS/4)*4; j+=4){
//            //                for(std::size_t k{0}; k<(RowsRHS/4)*4; k+=4){
//            //                    M4x4_SSE_block(__lhs, __rhs, __result, i, j, k);
//            //                }
//            //            }
//            //        }

//            for(std::size_t i{0}; i<(RowsLHS/4)*4; i+=4){
//                for(std::size_t j{0}; j<(ColumnsRHS/4)*4; j+=4){
//                    for(std::size_t k{0}; k<(RowsRHS/4)*4; k+=4){
//                        M4x4_SSE_block(__lhs, __rhs, __result, i, j, k);
//                    }
//                    //clean up for k
//                    if constexpr ((RowsRHS % 4) != 0){
//                        for(std::size_t ii{i}; ii<i+4; ++ii){
//                            for(std::size_t jj{j}; jj<j+4; ++jj){
//                                RESULT sum{0};
//                                for(std::size_t kk{(RowsRHS/4)*4}; kk<RowsRHS; ++kk){
//                                    sum += __lhs[ii*RowsLHS+kk]*__rhs[kk*ColumnsRHS+jj];
//                                }
//                                __result[ii*ColumnsRHS+jj] += sum;
//                            }
//                        }
//                    }
//                }
//                //clean up for j
//                if constexpr ((ColumnsRHS % 4) != 0){
//                    for(std::size_t ii{i}; ii<i+4; ++ii){
//                        for(std::size_t jj{(ColumnsRHS/4)*4}; jj<ColumnsRHS; ++jj){
//                            RESULT sum{0};
//                            for(std::size_t kk{0}; kk<RowsRHS; ++kk){
//                                sum += __lhs[ii*RowsLHS+kk]*__rhs[kk*ColumnsRHS+jj];
//                            }
//                            __result[ii*ColumnsRHS+jj] += sum;
//                        }
//                    }
//                }
//            }
//            //clean up for i
//            if constexpr ((RowsLHS % 4) != 0){
//                for(std::size_t ii{(RowsLHS/4)*4}; ii<RowsLHS; ++ii){
//                    for(std::size_t jj{0}; jj<ColumnsRHS; ++jj){
//                        RESULT sum{0};
//                        for(std::size_t kk{0}; kk<RowsRHS; ++kk){
//                            sum += __lhs[ii*RowsLHS+kk]*__rhs[kk*ColumnsRHS+jj];
//                        }
//                        __result[ii*ColumnsRHS+jj] += sum;
//                    }
//                }
//            }

//            return ;
//            //        }


//        }else{
//            RHS temp[ColumnsRHS];
//            for(std::size_t j{0}; j<ColumnsRHS; ++j){
//                for(std::size_t k{0}; k<RowsRHS; ++k){
//                    temp[k] = __rhs[k*ColumnsRHS+j];
//                }
//            for(std::size_t i{0}; i<RowsLHS; ++i){
//                    RESULT sum{0};
//                    for(std::size_t k{0}; k<RowsRHS; ++k){
//                        sum += __lhs[i*RowsLHS+k]*temp[k];
//                    }
//                    __result[i*ColumnsRHS+j] = sum;
//                }
//            }

////            RHS temp[ColumnsRHS];
////            for(std::size_t i{0}; i<RowsLHS; ++i){
////                for(std::size_t j{0}; j<ColumnsRHS; ++j){
////                    RESULT sum{0};
////                    for(std::size_t k{0}; k<RowsRHS; ++k){
////                        sum += __lhs[i*RowsLHS+k]*__rhs[k*ColumnsRHS+j];
////                    }
////                    __result[i*ColumnsRHS+j] = sum;
////                }
////            }
//        }


    }

    //A11 A12 A13 A14 A15
    //A21 A22 A23 A24 A25
    //A31 A32 A33 A34 A35
    //A41 A42 A43 A44 A45
    //A51 A52 A53 A54 A55



    static constexpr inline void M4x1_SSE_block(double const*A, double const*B, double const*C, std::size_t const I, std::size_t const J) {
//        __m128 acc0 = _mm_setzero_ps();
//        __m128 acc1 = _mm_setzero_ps();
//        __m128 acc2 = _mm_setzero_ps();
//        __m128 acc3 = _mm_setzero_ps();
//        for (int col = 0; col < ncols; col += 4) {
//            __m128 vec = _mm_load_ps(&v[col]);
//            __m128 mat0 = _mm_load_ps(&m[col + ncols * row]);
//            __m128 mat1 = _mm_load_ps(&m[col + ncols * (row + 1)]);
//            __m128 mat2 = _mm_load_ps(&m[col + ncols * (row + 2)]);
//            __m128 mat3 = _mm_load_ps(&m[col + ncols * (row + 3)]);
//            acc0 = _mm_add_ps(acc0, _mm_mul_ps(mat0, vec));
//            acc1 = _mm_add_ps(acc1, _mm_mul_ps(mat1, vec));
//            acc2 = _mm_add_ps(acc2, _mm_mul_ps(mat2, vec));
//            acc3 = _mm_add_ps(acc3, _mm_mul_ps(mat3, vec));
//        }
//        acc0 = _mm_hadd_ps(acc0, acc1);
//        acc2 = _mm_hadd_ps(acc2, acc3);
//        acc0 = _mm_hadd_ps(acc0, acc2);
//        _mm_store_ps(&C[I], acc0);



//        //A[i*Cols+k]*B[k*Cols+j]
//        const __m256d row1 = _mm256_loadu_pd(&B[J]);
//        for(int i=0; i<4; i++) {
//            __m256d row = _mm256_loadu_pd(&C[(i+I)*ColumnsRHS]);
//            const __m256d brod1 = _mm256_set1_pd(A[(i+I)*ColumnsLHS]);

//            row += brod1*row1;

//            _mm256_storeu_pd(const_cast<double*>(&C[(i+I)*ColumnsRHS + J]), row);
//        }
    }

    static constexpr inline void M4x4_SSE_block(double const*A, double const*B, double const*C, std::size_t const I, std::size_t const J, std::size_t const K) {
        //A[i*Cols+k]*B[k*Cols+j]
        const __m256d row1 = _mm256_loadu_pd(&B[(K+0)*ColumnsRHS + J]);
        const __m256d row2 = _mm256_loadu_pd(&B[(K+1)*ColumnsRHS + J]);
        const __m256d row3 = _mm256_loadu_pd(&B[(K+2)*ColumnsRHS + J]);
        const __m256d row4 = _mm256_loadu_pd(&B[(K+3)*ColumnsRHS + J]);
        for(int i=0; i<4; i++) {
            __m256d row = _mm256_loadu_pd(&C[(i+I)*ColumnsRHS + J]);
            const __m256d brod1 = _mm256_set1_pd(A[(i+I)*ColumnsLHS + K + 0]);
            const __m256d brod2 = _mm256_set1_pd(A[(i+I)*ColumnsLHS + K + 1]);
            const __m256d brod3 = _mm256_set1_pd(A[(i+I)*ColumnsLHS + K + 2]);
            const __m256d brod4 = _mm256_set1_pd(A[(i+I)*ColumnsLHS + K + 3]);

            row += brod1*row1 + brod2*row2 + brod3*row3 + brod4*row4;

            _mm256_storeu_pd(const_cast<double*>(&C[(i+I)*ColumnsRHS + J]), row);
        }
    }

    static constexpr inline void M2x2_SSE_block(double const*A, double const*B, double const*C, std::size_t const I, std::size_t const J, std::size_t const K) {
        __m128d row1 = _mm_loadu_pd(&B[(K+0)*ColumnsRHS]);//two entries
        __m128d row2 = _mm_loadu_pd(&B[(K+1)*ColumnsRHS]);
        for(int i=0; i<2; i++) {
            __m128d row = _mm_loadu_pd(&C[(i+I)*ColumnsRHS+J]);
            __m128d brod1 = _mm_set1_pd(A[(i+I)*ColumnsLHS + 0]);
            __m128d brod2 = _mm_set1_pd(A[(i+I)*ColumnsLHS + 1]);
            _mm_storeu_pd(const_cast<double*>(&C[(i+I)*ColumnsRHS+J]), row + row1*brod1 + row2*brod2);
        }
    }

    static constexpr inline void M1x2_SSE_block(double const*A, double const*B, double const*C, std::size_t const I, std::size_t const J, std::size_t const K) {
        __m128d row1 = _mm_loadu_pd(&B[(K+0)*ColumnsRHS]);//two entries
        __m128d row2 = _mm_loadu_pd(&B[(K+1)*ColumnsRHS]);
        for(int i=0; i<1; i++) {
            __m128d row = _mm_loadu_pd(&C[(i+I)*ColumnsRHS+J]);
            __m128d brod1 = _mm_set1_pd(A[(i+I)*ColumnsLHS + 0]);
            __m128d brod2 = _mm_set1_pd(A[(i+I)*ColumnsLHS + 1]);
            _mm_storeu_pd(const_cast<double*>(&C[(i+I)*ColumnsRHS+J]), row + row1*brod1 + row2*brod2);
        }
    }


    static constexpr inline void M4x4_SSE(double const*A, double const*B, double const*C) {
        __m256d row1 = _mm256_load_pd(&B[0]);//four entries
        __m256d row2 = _mm256_load_pd(&B[4]);
        __m256d row3 = _mm256_load_pd(&B[8]);
        __m256d row4 = _mm256_load_pd(&B[12]);
        for(int i=0; i<4; i++) {
            __m256d row = _mm256_loadu_pd(&C[i*ColumnsRHS]);
            __m256d brod1 = _mm256_set1_pd(A[i*ColumnsLHS + 0]);
            __m256d brod2 = _mm256_set1_pd(A[i*ColumnsLHS + 1]);
            __m256d brod3 = _mm256_set1_pd(A[i*ColumnsLHS + 2]);
            __m256d brod4 = _mm256_set1_pd(A[i*ColumnsLHS + 3]);
            row = _mm256_add_pd(
                        _mm256_add_pd(
                            _mm256_mul_pd(brod1, row1),
                            _mm256_mul_pd(brod2, row2)),
                        _mm256_add_pd(
                            _mm256_mul_pd(brod3, row3),
                            _mm256_mul_pd(brod4, row4)));
            _mm256_store_pd(const_cast<double*>(&C[i*ColumnsRHS]), row);
        }
    }



    static constexpr inline void M2x2_SSE(double const*A, double const*B, double const*C) {
        __m128d row1 = _mm_load_pd(&B[0]);//four entries
        __m128d row2 = _mm_load_pd(&B[2]);
        for(int i=0; i<2; i++) {
            __m128d brod1 = _mm_set1_pd(A[i*ColumnsLHS + 0]);
            __m128d brod2 = _mm_set1_pd(A[i*ColumnsLHS + 1]);
            _mm_store_pd(const_cast<double*>(&C[i*ColumnsRHS]), _mm_mul_pd(brod1, row1) + _mm_mul_pd(brod2, row2));
        }
    }
    */





//template <std::size_t RowsLHS, std::size_t ColumnsLHS, std::size_t RowsRHS, std::size_t ColumnsRHS>
//class matrix_matrix_multiplication<float, float, float, RowsLHS, ColumnsLHS, RowsRHS, ColumnsRHS>
//{
//public:
//    using value_type = float;

//    static constexpr inline auto evaluate(value_type const* __lhs, value_type const* __rhs, value_type const* __result){

//        //check if matrix vector multiplication
//        if constexpr (ColumnsRHS == 1){

//        }
//        //check if vector matrix multiplication
//        if constexpr (ColumnsLHS == 1){

//        }
//        //general matrix matrix multiplication
//    }

//    static constexpr inline void gemm_4x4_sse_f(value_type const* __lhs, value_type const* __rhs, value_type const* __result) {
//        __m128 row1 = _mm_load_ps(&__rhs[0]);
//        __m128 row2 = _mm_load_ps(&__rhs[4]);
//        __m128 row3 = _mm_load_ps(&__rhs[8]);
//        __m128 row4 = _mm_load_ps(&__rhs[12]);
//        for(int i=0; i<4; i++) {
//            __m128 brod1 = _mm_set1_ps(__lhs[4*i + 0]);
//            __m128 brod2 = _mm_set1_ps(__lhs[4*i + 1]);
//            __m128 brod3 = _mm_set1_ps(__lhs[4*i + 2]);
//            __m128 brod4 = _mm_set1_ps(__lhs[4*i + 3]);
//            _mm_store_ps(&__result[4*i], brod1*row1 + brod2*row2 + brod3*row3 + brod4*row4);
//        }
//    }

//};


} // NAMESPACE DETAIL

#endif // GEMM_WRAPPER_BONES_H
