// Copyright 2021 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------
#ifndef EXP_WRAPPER_BONES_H
#define EXP_WRAPPER_BONES_H

namespace detail {

template <typename _Father, typename _Tensor>
class exp_derivative_tensor_wrapper;

template <typename Tensor>
class exp_tensor_wrapper : public tensor_base<exp_tensor_wrapper<Tensor>>
{
    using data_type_tensor     = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
    static constexpr auto Dim  = data_type_tensor::dimension();
    static constexpr auto Rank = data_type_tensor::rank();
    friend class exp_derivative_tensor_wrapper<exp_tensor_wrapper<Tensor>, tensor<typename data_type_tensor::value_type, Dim, 4>>;

public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;
    using data_type  = tensor<value_type, Dim, 2>;

    static_assert(Dim == 2 || Dim == 3, "exp_tensor_wrapper only for Dim == 2, 3");
    static_assert(Rank == 2,            "exp_tensor_wrapper only only for Rank == 2");

    exp_tensor_wrapper(data_type_tensor const& data);

    exp_tensor_wrapper(exp_tensor_wrapper const& data);

    template<typename ...Indices>
    constexpr inline auto operator()(Indices const... indices)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

    constexpr inline auto derivative()const;

    constexpr inline auto evaluate();

private:
    constexpr inline auto evaluate_derivative(){
        std::vector<data_type> _data_i;
        _data_i.reserve(15);
        const value_type tol{1e-14};
        size_type iter{1}, max_iter{50};
        value_type n_fac{1};
        data_type exp_start{_data_base};
        data_type exp_n{exp_start};
        const auto I{eye<value_type, data_type::dimension(), data_type::rank()>()};
        _data_i.emplace_back(I);
        _data_i.emplace_back(exp_start);
        _data = I + exp_start;
        while (true) {
            n_fac *= (iter+1);
            exp_n = eval(exp_n*exp_start);
            _data_i.emplace_back(exp_n);
            _data += (exp_n/n_fac);
            const auto error{tmech::norm(exp_n)/n_fac};

            ++iter;

            if(error <= tol){break;}

            if(iter == max_iter){
                break;
                //throw std::runtime_error("exp_tensor_wrapper::derivative(): no convergence");
            }
        }

        n_fac = 1;
        iter = 1;
        //{I, A, A*A, A*A*A}
        // 0  1  2    3
        // I       = (1/0!)*otimesu(0,0)
        // A       = (1/1!)*otimesu(I,I)
        // A*A     = (1/2!)*otimesu(I,I)*A + A*otimesu(I,I)
        // A*A*A   = (1/3!)*otimesu(I,I)*A*A + A*otimesu(I,I)*A + A*A*otimesu(I,I)
        // ...
        _derivative = otimesu(I,I);
        for(size_type i{1}; i<_data_i.size()-1; ++i){
            n_fac *= (iter+1);
            const auto inv_n_fac{(1./n_fac)};
            for(size_type r{0}; r<=i; ++r){
                _derivative += otimesu(_data_i[r], trans(_data_i[i - r]))*inv_n_fac;
            }
            iter++;
        }
    }

    tensor<value_type, Dim, 2> _data;
    tensor<value_type, Dim, 4> _derivative;
    size_type _num_of_iter;
    Tensor _data_base;
};


template <typename _Father, typename _Tensor>
class exp_derivative_tensor_wrapper : public tensor_base<exp_derivative_tensor_wrapper<_Father, _Tensor>>
{
public:
    using value_type = typename _Tensor::value_type;
    using size_type  = std::size_t;

    exp_derivative_tensor_wrapper(_Father const& __father, _Tensor const& __tensor):
        _father(__father),
        _dexp(__tensor)
    {}

    exp_derivative_tensor_wrapper(exp_derivative_tensor_wrapper const& __data):
        _father(__data._father),
        _dexp(__data._dexp)
    {}

    template<typename ...Indices>
    constexpr inline auto operator()(Indices const... indices)const{
        return _dexp(indices...);
    }

    static constexpr inline auto rank(){
        return _Tensor::rank();
    }

    static constexpr inline auto dimension(){
        return _Tensor::dimension();
    }

    constexpr inline auto evaluate(){
        const_cast<_Father&>(_father).evaluate_derivative();
    }

private:
    _Father const& _father;
    _Tensor const& _dexp;
};

} // NAMESPACE DETAIL

#endif // EXP_WRAPPER_BONES_H
