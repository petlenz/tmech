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
#ifndef EXP_WRAPPER_MEAT_H
#define EXP_WRAPPER_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename Tensor>
exp_tensor_wrapper<Tensor>::exp_tensor_wrapper(data_type_tensor const& data):
    _data(),
    _derivative(),
    _num_of_iter(0),
    _data_base(data)
{}

/**
 * @brief Copy constructor
 */
template <typename Tensor>
exp_tensor_wrapper<Tensor>::exp_tensor_wrapper(exp_tensor_wrapper const& data):
    _data(),
    _derivative(),
    _num_of_iter(0),
    _data_base(data._data_base)
{}
//@}

template <typename Tensor>
template<typename ...Indices>
constexpr inline auto exp_tensor_wrapper<Tensor>::operator()(Indices const... indices)const{
    return _data(indices...);
}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
*/
template <typename Tensor>
constexpr inline auto exp_tensor_wrapper<Tensor>::dimension(){
    return data_type::dimension();
}
/**
 * @brief Returns the rank.
 */
template <typename Tensor>
constexpr inline auto exp_tensor_wrapper<Tensor>::rank(){
    return data_type::rank();
}
//@}

template <typename Tensor>
constexpr inline auto exp_tensor_wrapper<Tensor>::derivative()const{
    return exp_derivative_tensor_wrapper<exp_tensor_wrapper<Tensor>, decltype (_derivative)>(*this, _derivative);
}

template <typename Tensor>
constexpr inline auto exp_tensor_wrapper<Tensor>::evaluate(){
    if(!this->_is_init){
        const value_type tol{1e-14};
        size_type iter{1}, max_iter{50};
        value_type n_fac{1};
        data_type exp_start{_data_base};
        data_type exp_n{exp_start};
        _data = eye<value_type, data_type::dimension(), data_type::rank()>() + exp_start;
        //expA = (1/0!)*I + (1/1!)*A + (1/2!)*A*A + (1/3!)*A*A*A
        while (true) {
            n_fac *= (iter+1);
            exp_n = eval(exp_n*exp_start);
            _data += (exp_n/n_fac);
            const auto error{tmech::norm(exp_n)/n_fac};
            if(error <= tol){break;}

            if(iter == max_iter){
                break;
                //throw std::runtime_error("exp_tensor_wrapper::evaluate(): no convergence");
            }
            ++iter;
        }
        //for later use.... deriv
        _num_of_iter = iter;
        this->_is_init = true;
    }
}

} // NAMESPACE DETAIL

#endif // EXP_WRAPPER_MEAT_H
