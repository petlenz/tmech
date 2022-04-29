/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SIGN_TENSOR_WRAPPER_MEAT_H
#define SIGN_TENSOR_WRAPPER_MEAT_H


namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _Tensor>
sign_tensor_wrapper<_Tensor>::sign_tensor_wrapper(_Tensor __data, value_type const __eps, size_type __max_iter)noexcept:
    _eps(__eps),
    _max_iter(__max_iter),
    _S(),
    _N(),
    _is_init_S(false),
    _is_init_N(false),
    _data_tensor(__data)
{}

/**
 * Copy constructor
 */
template <typename _Tensor>
sign_tensor_wrapper<_Tensor>::sign_tensor_wrapper(sign_tensor_wrapper const& __data)noexcept:
    _eps(__data._eps),
    _max_iter(__data._max_iter),
    _S(),
    _N(),
    _is_init_S(false),
    _is_init_N(false),
    _data_tensor(__data._data_tensor)
{}
//@}

template <typename _Tensor>
template<typename ...Indicies>
constexpr inline auto sign_tensor_wrapper<_Tensor>::operator ()(Indicies ... indicies)const noexcept{
    return _S(indicies...);
}

template <typename _Tensor>
constexpr inline auto sign_tensor_wrapper<_Tensor>::decompose()noexcept{
    if(!_is_init_S){
        size_type iter{0};
        data_type _data_old;
        _S = _data_tensor;
        while (true) {
            _data_old = _S;
            _S = (_S + inv(_S))*0.5;

            const auto error{tmech::norm(_data_old - _S)};
            if(error <= _eps){break;}

            if(iter == _max_iter){break;}
            ++iter;
        }
        _is_init_S = true;
    }
}

} // NAMESPACE DETAIL
#endif // SIGN_TENSOR_WRAPPER_MEAT_H
