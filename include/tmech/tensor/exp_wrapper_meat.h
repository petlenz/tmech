/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
template <typename _Tensor>
exp_tensor_wrapper<_Tensor>::exp_tensor_wrapper(data_type_tensor const& data)noexcept:
    _data(),
    _derivative(),
    _num_of_iter(0),
    _data_base(data)
{}

/**
 * @brief Copy constructor
 */
template <typename _Tensor>
exp_tensor_wrapper<_Tensor>::exp_tensor_wrapper(exp_tensor_wrapper const& data)noexcept:
    _data(),
    _derivative(),
    _num_of_iter(0),
    _data_base(data._data_base)
{}
//@}

template <typename _Tensor>
template<typename ...Indices>
constexpr inline auto exp_tensor_wrapper<_Tensor>::operator()(Indices const... indices)const noexcept{
    return _data(indices...);
}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the dimension.
*/
template <typename _Tensor>
constexpr inline auto exp_tensor_wrapper<_Tensor>::dimension()noexcept{
    return data_type::dimension();
}
/**
 * @brief Returns the rank.
 */
template <typename _Tensor>
constexpr inline auto exp_tensor_wrapper<_Tensor>::rank()noexcept{
    return data_type::rank();
}
//@}

template <typename _Tensor>
constexpr inline auto exp_tensor_wrapper<_Tensor>::derivative()const noexcept{
    return exp_derivative_tensor_wrapper<exp_tensor_wrapper<_Tensor>, decltype (_derivative)>(*this, _derivative);
}

template <typename _Tensor>
constexpr inline auto exp_tensor_wrapper<_Tensor>::evaluate()noexcept{
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

template <typename _Tensor>
constexpr inline auto exp_tensor_wrapper<_Tensor>::evaluate_derivative()noexcept{
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

} // NAMESPACE DETAIL

#endif // EXP_WRAPPER_MEAT_H
