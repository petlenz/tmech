/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef NEWTON_SOLVER_SINGLE_VARIABLE_BONES_H
#define NEWTON_SOLVER_SINGLE_VARIABLE_BONES_H

namespace detail {

template <typename Function, typename Variable>
class newton_solver_single_variable
{
public:
    using size_type = std::size_t;
    using value_type = typename Variable::data_type;

    newton_solver_single_variable(Function const& f, Variable const& var):
        _f(f),
        _var(var),
        max_iter(500),
        eps_tol(5e-10),
        error()
    {}

    template<typename Data>
    inline auto solve(Data & data){
        size_type iter{0};

        auto _df{derivative(_f, _var)};


#ifndef NDEBUG
        std::cout<<_df<<std::endl;
        error.clear();
        error.reserve(max_iter);
#endif

        while (true) {
            //update derivative
            const auto df_val{_df(data)};

            //update function
            _f.reset();
            _f.update(data);
            const auto f_val{_f(data)};

#ifdef NDEBUG
            if(std::abs(f_val) <= eps_tol){
                break;
            }
#else
            error.push_back(std::abs(f_val));
            if(error.back() <= eps_tol){
                break;
            }
#endif
            //update data

            std::get<Variable::get_ID()>(data) -= f_val/df_val;

            ++iter;
            if(iter == max_iter){
                throw "no convergence";
                break;
            }
        }
#ifdef NDEBUG
        return error;
#endif
    }

private:
    Function _f;
    Variable _var;
    size_type max_iter;
    value_type eps_tol;
    std::vector<value_type> error;
};
}
#endif // NEWTON_SOLVER_SINGLE_VARIABLE_BONES_H
