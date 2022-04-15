/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
//#ifndef NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_MEAT_H
//#define NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_MEAT_H

//namespace tmech {
//namespace detail {

///**
//* @name Constructors
//*/
////@{
///**
//* @brief Default constructor.
//* @param func A function, which should be differentiate.
//* @param A A point, at which the funcion should be differentiate.
//* @param eps Represents a small change in %_A.
//*/
//template <typename Function, typename Direction>
//numerical_differentiation_central_sym_wrapper<Function, Direction>::numerical_differentiation_central_sym_wrapper(Function & func, Direction const& A, value_type const eps):
//    _func(func),
//    _A(A),
//    _result(),
//    _eps(eps)
//{}

///**
// * @brief Copy constructor. Note, that
// * only the function %_func, the point %_A and
// * the change in Point $_eps is copied.
// * @param data A %numerical_differentiation_central_sym_wrapper to copy.
// */
//template <typename Function, typename Direction>
//numerical_differentiation_central_sym_wrapper<Function, Direction>::numerical_differentiation_central_sym_wrapper(numerical_differentiation_central_sym_wrapper const& data):
//    _func(data._func),
//    _A(data._A),
//    _result(),
//    _eps(data._eps)
//{}
////@}

///**
//* @name Data access
//*/
////@{
///**
//* Returns a constant reference of the element at the specified position in the tensor.
//* @param indicies a list of indices specifying the position in the tensor. Indices
//* must be unsigned integers, the number of indices must be equal to the rank of the expression.
//* @return  Read reference to data.
//*/
//template <typename Function, typename Direction>
//template<typename ...Indicies>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::operator ()(Indicies const ... indices)const{
//    static_assert(sizeof...(indices) == data_type::rank(), "Nonrank matching indexing in tensor");
//    return _result(indices...);
//}

///**
// * @brief  Memory access to the data contained in the %numerical_differentiation_backward_sym_wrapper.
// * @return Read pointer to the underlying memory.
//*/
//template <typename Function, typename Direction>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::raw_data()const{
//    return _result.raw_data();
//}
////@}

///**
//* @name Dimension and rank
//*/
////@{
///**
//* Returns the dimension of the expression.
//*/
//template <typename Function, typename Direction>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::dimension(){
//    return data_type::dimension();
//}

///**
// * Returns the rank of the expression.
// */
//template <typename Function, typename Direction>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::rank(){
//    return data_type::rank();
//}
////@}


///**
//* @name Evaluation of expression
//*/
////@{
///**
//* @brief Uses %result as container for the computation.
//* The finial result is saved in %result. No internal memory is used.
//*
//* @param result External tensor to save the result.
//*/
//template <typename Function, typename Direction>
//template<typename Result>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::evaluate(Result & result){
//    evaluate_imp(result);
//}

///**
//* @brief Internal memory is used to compute the result.
//*/
//template <typename Function, typename Direction>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::evaluate(){
//    evaluate(_result);
//}

///**
//* @brief Implementation of the evaluation function.
//* The final result is computed and saved in result.
//*/
//template <typename Function, typename Direction>
//template<typename Result>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::evaluate_imp(Result & result){

//    if constexpr (std::is_fundamental<return_type>::value == true){
//        if constexpr (Direction::rank() == 2){
//            diff_0_2(result);
//        }
//    }else{
//        if constexpr (return_type::rank() == 2 && Direction::rank() == 2){
//            //dA_{ij}/dB_{mn} = C_{imnj}
//            diff_2_2(result);
//        }else if constexpr (return_type::rank() == 4 && Direction::rank() == 2){
//            //dA_{ijkl}/dB_{mn} = C_{ijmnkl}
//            diff_4_2(result);
//        }
//    }
//}
////@}

///**
//* @name Evaluation implementation
//*/
////@{
///**
//* @brief Numerical differentiation of scalar valued functions of
//* second-order tensors wrt. a second-order tensor.
//*
//* @param result Internal or external tensor to save the result.
//*/
//template <typename Function, typename Direction>
//template<typename Result>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::diff_0_2(Result & res){
//    constexpr auto Dim{Direction::dimension()};
//    const value_type eps_half{_eps};
//    const value_type inv_eps{1.0/(_eps*2)};
//    tensor<value_type, Dim, 2> Dp(_A), Dm(_A);
//    for(size_type m{0}; m < Dim; ++m){
//        for(size_type n{m}; n < Dim; ++n){
//            if(m == n){
//                Dp(m,n) += eps_half;
//                Dm(m,n) -= eps_half;
//            }else{
//                Dp(m,n) += eps_half*0.5;
//                Dp(n,m) += eps_half*0.5;
//                Dm(m,n) -= eps_half*0.5;
//                Dm(n,m) -= eps_half*0.5;
//            }
////            Dp(m,n) += eps_half;
////            Dp(n,m) += eps_half;
////            Dm(m,n) -= eps_half;
////            Dm(n,m) -= eps_half;
//            const auto ap{_func(Dp)};
//            const auto am{_func(Dm)};
////            Dp(m,n) -= eps_half;
////            Dp(n,m) -= eps_half;
////            Dm(m,n) += eps_half;
////            Dm(n,m) += eps_half;

//            if(m == n){
//                Dp(m,n) -= eps_half;
//                Dm(m,n) += eps_half;
//            }else{
//                Dp(m,n) -= eps_half*0.5;
//                Dp(n,m) -= eps_half*0.5;
//                Dm(m,n) += eps_half*0.5;
//                Dm(n,m) += eps_half*0.5;
//            }

//            if(n==m){
//                res(m,n) = (ap - am)*inv_eps;
//            }else{
//                res(n,m) = res(m,n) = (ap - am)*inv_eps;
//            }
//        }
//    }
//}

///**
//* @brief Numerical differentiation of second-order tensor
//* valued functions  wrt. a second-order tensor.
//*
//* @param result Internal or external tensor to save the result.
//*/
//template <typename Function, typename Direction>
//template<typename Result>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::diff_2_2(Result & res){
//    constexpr auto Dim{Direction::dimension()};
//    const value_type eps_half{_eps};
//    const value_type inv_eps{1.0/(_eps*2)};
//    tensor<value_type, Dim, 2> Dp(_A), Dm(_A);
//    //DC_{imnj} =  (C_ij + C_ji)*0.5 / A_mn
//    for(size_type m{0}; m < Dim; ++m){
//        for(size_type n{m}; n < Dim; ++n){
//            if(m == n){
//                Dp(m,n) += eps_half;
//                Dm(m,n) -= eps_half;
//            }else{
//                Dp(m,n) += eps_half*0.5;
//                Dp(n,m) += eps_half*0.5;
//                Dm(m,n) -= eps_half*0.5;
//                Dm(n,m) -= eps_half*0.5;
//            }

//            const tensor<value_type, Dim, 2> ap{_func(Dp)};
//            const tensor<value_type, Dim, 2> am{_func(Dm)};

//            if(m == n){
//                Dp(m,n) -= eps_half;
//                Dm(m,n) += eps_half;
//            }else{
//                Dp(m,n) -= eps_half*0.5;
//                Dp(n,m) -= eps_half*0.5;
//                Dm(m,n) += eps_half*0.5;
//                Dm(n,m) += eps_half*0.5;
//            }

//            for(size_type i{0}; i<Dim; ++i){
//                for(size_type j{i}; j<Dim; ++j){
//                    const auto value{(ap(i,j)-am(i,j))*inv_eps};
//                    if(i == j){
//                        if(m == n){
//                            res(i,m,n,j) = value;
//                        }else{
//                            res(i,m,n,j) = value;
//                            res(i,n,m,j) = value;
//                        }
//                    }else{
//                        if(m == n){
//                            res(i,m,n,j) = value;
//                            res(j,n,m,i) = value;
//                        }else{
//                            res(i,m,n,j) = value;
//                            res(j,m,n,i) = value;
//                            res(i,n,m,j) = value;
//                            res(j,n,m,i) = value;
//                        }
//                    }
//                }
//            }
//        }
//    }
//}

///**
//* @brief Numerical differentiation of fourth-order tensor
//* valued functions  wrt. a second-order tensor.
//*
//* @param result Internal or external tensor to save the result.
//*/
//template <typename Function, typename Direction>
//template<typename Result>
//constexpr inline auto numerical_differentiation_central_sym_wrapper<Function, Direction>::diff_4_2(Result & res){
//    constexpr auto Dim{Direction::dimension()};
//    const value_type eps_half{_eps*0.25};
//    tensor<value_type, Dim, 2> Dp(_A), Dm(_A);
//    const value_type inv_eps{1.0/_eps};
//    for(size_type m{0}; m<Dim; ++m){
//        for(size_type n{n}; n<Dim; ++n){
//            Dp(m,n) += eps_half;
//            Dp(n,m) += eps_half;
//            Dm(m,n) -= eps_half;
//            Dm(n,m) -= eps_half;
//            const tensor<value_type, Dim, 4> ap{_func(Dp)};
//            const tensor<value_type, Dim, 4> am{_func(Dm)};
//            Dp(m,n) -= eps_half;
//            Dp(n,m) -= eps_half;
//            Dm(m,n) += eps_half;
//            Dm(n,m) += eps_half;
//            for(size_type i{0}; i<Dim; ++i){
//                for(size_type j{0}; j<Dim; ++j){
//                    for(size_type k{0}; k<Dim; ++k){
//                        for(size_type l{0}; l<Dim; ++l){
//                            const auto value{(ap(i,j,k,l)-am(i,j,k,l))*inv_eps};
//                            res(i,j,m,n,k,l) = value;
//                            res(i,j,n,m,k,l) = value;
//                        }
//                    }
//                }
//            }
//        }
//    }
//}
////@}
//}
//}
//#endif // NUMERICAL_DIFFERENTIATION_CENTRAL_SYM_WRAPPER_MEAT_H
