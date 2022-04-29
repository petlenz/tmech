/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef GEMM_WRAPPER_BONES_H
#define GEMM_WRAPPER_BONES_H


namespace detail {

template <typename LHS, typename RHS, typename RESULT, std::size_t RowsLHS, std::size_t ColumnsLHS, std::size_t RowsRHS, std::size_t ColumnsRHS>
class gemm_wrapper
{
public:
    using size_type = std::size_t;

    static constexpr inline void evaluate(LHS const* __lhs, RHS const* __rhs, RESULT* __result){

        gemm(__lhs, __rhs, __result);
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


    static constexpr inline auto gemm(LHS const*const __lhs, RHS const*const __rhs, RESULT * __result){
        for(size_type i{0}; i<RowsLHS; ++i){
            for(size_type j{0}; j<ColumnsRHS; ++j){
                RESULT sum{0};
                for(size_type k{0}; k<RowsRHS; ++k){
                    sum += __lhs[i*ColumnsLHS+k]*__rhs[k*ColumnsRHS+j];
                }
                __result[i*ColumnsRHS+j] = sum;
            }
        }
    }

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


//    static constexpr inline auto gemv_blocked_sse(double const* __lhs, double const* __rhs, double const* __result){

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

};



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
