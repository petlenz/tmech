/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_POW_DERIVATIVE_WRAPPER_MEAT_H
#define TENSOR_POW_DERIVATIVE_WRAPPER_MEAT_H

namespace detail {

template <typename _Father>
tensor_pow_wrapper_derivative<_Father>::tensor_pow_wrapper_derivative():
    _father(_Father())
{}

template <typename _Father>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Father>
constexpr inline std::ostream& tensor_pow_wrapper_derivative<_Father>::print(std::ostream & __os)const{
    __os<<"d";
    _father.print(__os);
    return __os;
}

template <typename _Father>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::get_value(_Data const& __data)const{
    return _father._pow_data.derivative();
}

template <typename _Father>
template<typename _Data>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::update_imp(_Data const& __data){
    _father.update_imp(__data);
}

template <typename _Father>
constexpr inline auto tensor_pow_wrapper_derivative<_Father>::reset_imp(){
    _father.reset_imp();
}

}
#endif // TENSOR_POW_DERIVATIVE_WRAPPER_MEAT_H
