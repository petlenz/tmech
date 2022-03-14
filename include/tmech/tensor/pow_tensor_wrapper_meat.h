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
template <typename _Tensor>
pow_tensor_wrapper<_Tensor>::pow_tensor_wrapper(data_type_tensor const& __data_expr, size_type const __exponent):
    _data(),
    _data_expr(__data_expr),
    _derivative(),
    _data_i(),
    _expo(__exponent)
{}
/**
 * Copy constructor
 */
template <typename _Tensor>
pow_tensor_wrapper<_Tensor>::pow_tensor_wrapper(pow_tensor_wrapper const& data):
    _data(),
    _data_expr(data._data_expr),
    _derivative(),
    _data_i(),
    _expo(data._expo)
{}
//@}


template <typename _Tensor>
template<typename ...Indices>
constexpr inline auto pow_tensor_wrapper<_Tensor>::operator ()(Indices const... indices)const{
    return _data(indices...);
}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::dimension(){
    return data_type_tensor::dimension();
}
/**
 * Returns the rank.
 */
template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::rank(){
    return data_type_tensor::rank();
}
//@}

template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::derivative(){
    return pow_tensor_derivative_wrapper<this_type, decltype (_derivative)>(*this, _derivative);
}

template <typename _Tensor>
template<typename Result>
constexpr inline auto pow_tensor_wrapper<_Tensor>::evaluate(Result & result){
    evaluate_imp(result);
}

template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::evaluate(){
    evaluate_imp(_data);
}

template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::raw_data()const{
    return _data.raw_data();
}

template <typename _Tensor>
template<typename Result>
constexpr inline auto pow_tensor_wrapper<_Tensor>::evaluate_imp(Result & result){
    if(_expo == 0){
        result.fill(0);
    }else{
        result = _data_expr;
        if(_expo == 1){
            }else if(_expo == 2){
                result = eval(result*result);
            }else{
                tensor<value_type, dimension(), 2> temp{result};
                for(size_type i{0};i<_expo-1;++i){
                    result = eval(result * temp);
                }
            }
    }
}

//template <typename _Tensor>
//constexpr inline auto pow_tensor_wrapper<_Tensor>::sym_derivative(){
//    const auto n{_expo-1};
//    for(size_type i{0}; i<_expo; ++i){
//        _derivative += (otimes(_data_i[i], _data_i[n - i]) + boxtimes(_data_i[i], _data_i[n - i]))*0.5;
//    }
//}

//template <typename _Tensor>
//constexpr inline auto pow_tensor_wrapper<_Tensor>::skew_derivative(){
//    const auto n{_expo-1};
//    for(size_type i{0}; i<_expo; ++i){
//        _derivative += (otimes(_data_i[i], _data_i[n - i]) - boxtimes(_data_i[i], _data_i[n - i]))*0.5;
//    }
//}

template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::evaluate_derivatives(){
    _derivative.fill(0);
    if(_expo > 0){
        const tensor<value_type, dimension(), 2> temp{_data_expr};
        std::cout<<std::endl;
        std::cout<<temp<<std::endl;
        _data_i.clear();
        _data_i.reserve(_expo);
        _data_i.emplace_back(eye<value_type, dimension(), 2>());
        _data_i.emplace_back(temp);

        for(size_type i{0};i<_expo-1;++i){
            _data_i.emplace_back(_data_i[i+1]*temp);
        }
        normal_derivative();
    }
}

template <typename _Tensor>
constexpr inline auto pow_tensor_wrapper<_Tensor>::normal_derivative(){
    for(size_type i{1}; i<=_expo; ++i){
        _derivative += otimesu(_data_i[i-1], trans(_data_i[_expo - i]));
    }
}


} // NAMESPACE DETAIL
#endif // POW_TENSOR_WRAPPER_MEAT_H
