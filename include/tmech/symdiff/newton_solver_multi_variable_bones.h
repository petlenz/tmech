/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef NEWTON_SOLVER_MULTI_VARIABLE_BONES_H
#define NEWTON_SOLVER_MULTI_VARIABLE_BONES_H

#include <exception>

namespace detail {




template <typename Jacobi, typename Functon>
class newton_solver_multi_variable
{
public:
    using value_type = typename Jacobi::value_type;
    using size_type = std::size_t;

    newton_solver_multi_variable():
        J(),
        b(),
        x(),
        max_iter(100),
        eps_tol(5e-14),
        error()
    {
#ifndef NDEBUG
        error.reserve(max_iter);
#endif
    }

    template<typename Data>
    inline auto solve(Data & data){

        size_type iter{0};
        J.update(data);
        b.update(data);
        const auto& bvec{b(data)};

#ifndef NDEBUG
        error.clear();
        error.reserve(max_iter);
#endif

        while (true) {
            auto Jinv{J(data)};

            //decomposte Jinv
            lu_detail(Jinv);

            //solve for x
            solve(Jinv, bvec);

            //update x
            update_data(data);

            //update function
            b.update(data);

#ifdef NDEBUG
            if(norm(bvec) <= eps_tol){
                break;
            }
#else
            error.push_back(norm(bvec));
            if(error.back() <= eps_tol){
                break;
            }
#endif

            ++iter;
            if(iter == max_iter){
                throw std::runtime_error("no convergence");
                break;
            }
        }
    }

    constexpr inline auto const& get_error()const{
        return error;
    }


private:
    template<typename T, std::size_t Size>
    constexpr inline auto norm(std::array<T, Size> const& vec){
        return std::sqrt(norm_details(vec));
    }

    template<size_type I = 0, typename T, std::size_t Size>
    constexpr inline auto norm_details(std::array<T, Size> const& vec){
        if constexpr (I < Size){
            return vec[I]*vec[I] + norm_details<I+1>(vec);
        }else{
            return T(0);
        }
    }

    template<size_type I = 0, typename Data>
    constexpr inline auto update_data(Data & data){
        if constexpr (I < Functon::size){
            constexpr auto ID{Jacobi::template get_var_ID<I>()};
            std::get<ID>(data) -= x[I];
            update_data<I+1>(data);
        }
    }

    template<typename T, std::size_t Rows>
    inline auto lu_detail(std::array<std::array<T, Rows>, Rows> & A){
        size_type p{0};

        for (size_type i = 0; i < Rows; ++i){
            ipiv[i] = i;
        }

        for(size_type i{0}; i<Rows; ++i){

            //serach pivot
            try{
                p = search_pivot_column(A, i);
            }catch (std::exception const& ){
                assert("\tMatrix numerically singular.");
            }

            if(p != i){
                for(std::size_t j{0};j<Rows;++j){
                    std::swap(A[i][j], A[p][j]);
                }
                std::swap(ipiv[i], ipiv[p]);
            }

            const T Akk = 1.0/A[i][i];
            for(std::size_t j{i+1};j<Rows;++j){
                A[j][i] *=Akk;
            }

            for(std::size_t j{i+1};j<Rows;++j){
                for(std::size_t k{i+1};k<Rows;++k){
                    A[j][k] -= A[j][i]*A[i][k];
                }
            }
        }
    }

    template<typename T, std::size_t Rows>
    inline auto solve(std::array<std::array<T, Rows>, Rows> const& A, std::array<T, Rows> const& b) {


        for(size_type i{0};i<Rows;i++) {
            x[i] = b[ipiv[i]];
            for(size_type k{0};k<i;k++){
                x[i] -= A[i][k] * x[k];
            }
        }

        for (long long int i = Rows - 1; i >= 0; --i) {
            for (long long int k = Rows - 1; k > i; --k){
                x[i] -= A[i][k] * x[k];
            }
            x[i] /= A[i][i];
        }
    }

    template<typename T, std::size_t Rows>
    inline auto search_pivot_column(std::array<std::array<T, Rows>, Rows> const& A, size_type const column){
        std::numeric_limits<T> real_info;
        auto p = column;

        for(size_type j{column + 1}; j<Rows; ++j){
            if(std::abs(A[p][column]) < std::abs(A[j][column])){
                p = j;
            }
        }
        if (std::abs(A[p][column]) < (real_info.epsilon())){
            throw std::runtime_error("singular matrix");
        }
        return p;
    }

    Jacobi J;
    Functon b;
    std::array<value_type, Functon::size> x;
    std::array<size_type, Functon::size> ipiv;
    size_type max_iter;
    value_type eps_tol;

    std::vector<value_type> error;
};


}

#endif // NEWTON_SOLVER_MULTI_VARIABLE_BONES_H
