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
#ifndef POW_TENSOR_WRAPPER_MEAT_H
#define POW_TENSOR_WRAPPER_MEAT_H


namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename Tensor>
pow_tensor_wrapper<Tensor>::pow_tensor_wrapper(Tensor const& data, size_type const exponent):_data(),
    _data_expr(data),
    _expo(exponent)
{}
/**
 * Copy constructor
 */
template <typename Tensor>
pow_tensor_wrapper<Tensor>::pow_tensor_wrapper(pow_tensor_wrapper const& data):
    _data(),
    _data_expr(data._data_expr),
    _expo(data._expo)
{}
//@}


template <typename Tensor>
template<typename ...Indices>
constexpr inline auto pow_tensor_wrapper<Tensor>::operator ()(Indices const... indices)const{
    return _data(indices...);
}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::dimension(){
    return data_type::dimension();
}
/**
 * Returns the rank.
 */
template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::rank(){
    return data_type::rank();
}
//@}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::derivative(){
    if(_expo > 1){
        const auto I{eye<value_type, data_type::dimension(), data_type::rank()>()};
        _data_deriv = _data_expr;
        _data_i.reserve(_expo);
        _data_i.emplace_back(I);
        _data_i.emplace_back(_data_deriv);

        if(_expo > 2){
            data_type temp{_data_deriv};
            for(size_type i{0};i<_expo-1;++i){
                _data_deriv = eval(_data_deriv * temp);
                _data_i.emplace_back(_data_deriv);
            }
        }
        normal_derivative();

//            if(is_symmetric(_data_deriv.raw_data())){
//                sym_derivative();
//            }else if (is_skewsymmetric(_data_deriv.raw_data())){
//                skew_derivative();
//            }else{
//                normal_derivative();
//            }

    }else{
        _derivative.fill(0);
    }

    return _derivative;
}

template <typename Tensor>
template<typename Result>
constexpr inline auto pow_tensor_wrapper<Tensor>::evaluate(Result & result){
    evaluate_imp(result);
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::evaluate(){
    evaluate_imp(_data);
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::raw_data()const{
    return _data.raw_data();
}

template <typename Tensor>
template<typename Result>
constexpr inline auto pow_tensor_wrapper<Tensor>::evaluate_imp(Result & result){
    result = _data_expr;
    if(_expo == 1){
    }else if(_expo == 2){
        result = eval(result*result);
    }else{
        data_type temp{_data};
        for(size_type i{0};i<_expo-1;++i){
            result = result * temp;
        }
    }
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::sym_derivative(){
    const auto n{_expo-1};
    for(size_type i{0}; i<_expo; ++i){
        _derivative += (otimes(_data_i[i], _data_i[n - i]) + boxtimes(_data_i[i], _data_i[n - i]))*0.5;
    }
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::skew_derivative(){
    const auto n{_expo-1};
    for(size_type i{0}; i<_expo; ++i){
        _derivative += (otimes(_data_i[i], _data_i[n - i]) - boxtimes(_data_i[i], _data_i[n - i]))*0.5;
    }
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::normal_derivative(){
    const auto n{_expo-1};
    for(size_type i{0}; i<_expo; ++i){
        _derivative += otimes(_data_i[i], _data_i[n - i]);
    }
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::is_symmetric(value_type const*const ptr){
    if constexpr (dimension() == 2){
        //return (ptr[1] == ptr[2]);
        const auto val1{ptr[1]}, val2{ptr[2]};
        return std::fabs(val1 - val2) <= std::numeric_limits<value_type>::epsilon()*std::max(1.0,std::max(val1, val2));
    }else if constexpr (dimension() == 3){
        const auto val1{ptr[1]}, val2{ptr[3]};
        const auto val3{ptr[2]}, val4{ptr[6]};
        const auto val5{ptr[5]}, val6{ptr[7]};

        // 0 1 2
        // 3 4 5
        // 6 7 8
        const bool test1{std::fabs(val1 - val2) <= std::numeric_limits<value_type>::epsilon()*std::max(1.0, std::max(val1, val2))};
        const bool test2{std::fabs(val3 - val4) <= std::numeric_limits<value_type>::epsilon()*std::max(1.0, std::max(val3, val4))};
        const bool test3{std::fabs(val5 - val6) <= std::numeric_limits<value_type>::epsilon()*std::max(1.0, std::max(val5, val6))};

        return (test1) && (test2) && (test3);
    }else{
        assert(0);
    }
}

template <typename Tensor>
constexpr inline auto pow_tensor_wrapper<Tensor>::is_skewsymmetric(value_type const*const ptr){
    if constexpr (dimension() == 2){
        return (ptr[1] == -ptr[2]);
        //const auto val1{prt[1]}, val2{ptr[2]};
        //return std::fabs(val1 - val2) <= std::numeric_limits<value_type>::epsilon()*std::max(1,std::max(val1, val2));
    }else if constexpr (dimension() == 3){
        //const bool test1{}
        return (ptr[1] == -ptr[3]) && (ptr[2] == -ptr[6]) && (ptr[5] == -ptr[7]);
    }else{
        assert(0);
    }
}


} // NAMESPACE DETAIL
#endif // POW_TENSOR_WRAPPER_MEAT_H
