/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SIGN_TENSOR_WRAPPER_BONES_H
#define SIGN_TENSOR_WRAPPER_BONES_H

namespace detail {


template <typename _Father>
class sign_S_wrapper : public tensor_base<sign_S_wrapper<_Father>>
{
public:
    using value_type = typename _Father::data_type::value_type;

    sign_S_wrapper(_Father const& __father):
        _father(__father)
    {}

    sign_S_wrapper(sign_S_wrapper const& __data):
        _father(__data._father)
    {}

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies ... indicies)const{
        return _father._S(indicies...);
    }

    static constexpr inline auto dimension(){
        return _Father::data_type::dimension();
    }

    static constexpr inline auto rank(){
        return _Father::data_type::rank();
    }

    constexpr inline auto evaluate(){
        const_cast<_Father&>(_father).decompose_S();
    }

    constexpr inline auto raw_data()const{
        return _father._S.raw_data();
    }
private:
    _Father const& _father;
};


template <typename _Father>
class sign_N_wrapper : public tensor_base<sign_N_wrapper<_Father>>
{
public:
    using value_type = typename _Father::data_type::value_type;

    sign_N_wrapper(_Father const& __father):
        _father(__father)
    {}

    sign_N_wrapper(sign_N_wrapper const& __data):
        _father(__data._father)
    {}

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies ... indicies)const{
        return _father._N(indicies...);
    }

    static constexpr inline auto dimension(){
        return _Father::data_type::dimension();
    }

    static constexpr inline auto rank(){
        return _Father::data_type::rank();
    }

    constexpr inline auto evaluate(){
        const_cast<_Father&>(_father).decompose_N();
    }

    constexpr inline auto raw_data()const{
        return _father._N.raw_data();
    }
private:
    _Father const& _father;
};

/**
* @class sign_tensor_wrapper
* @brief The tensor sign decomposition A = S*N, S = sign(A), N = sqrt(A*A)
* https://www.maths.manchester.ac.uk/~higham/narep/narep225.pdf
*
* @tparam _Tensor Tensor expression from which the
* sign decomposition should be determined.
*/
template <typename _Tensor>
class sign_tensor_wrapper
{
    friend class sign_S_wrapper<sign_tensor_wrapper<_Tensor>>;
    friend class sign_N_wrapper<sign_tensor_wrapper<_Tensor>>;

    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using size_type = std::size_t;
    using value_type = typename data_type_tensor::value_type;
    using data_type = tensor<value_type, data_type_tensor::dimension(), data_type_tensor::rank()>;

    sign_tensor_wrapper(_Tensor data, value_type const __eps, size_type __max_iter);

    sign_tensor_wrapper(sign_tensor_wrapper const& data);

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies ... indicies)const;

    constexpr inline auto N(){
        return sign_N_wrapper<sign_tensor_wrapper<_Tensor>>(*this);
    }

    constexpr inline auto S(){
        return sign_S_wrapper<sign_tensor_wrapper<_Tensor>>(*this);
    }

private:
    constexpr inline auto decompose_S(){
        if(!_is_init_S){
            decompose();
            _is_init_S = true;
        }
    }

    constexpr inline auto decompose_N(){
        if(!_is_init_N){
            decompose();
            _N = inv(_S)*_data_tensor;
            _is_init_N = true;
        }
    }

    constexpr inline auto decompose();

    //A = S*N
    //S = sign(A)
    //N = sqrt(A*A) = inv(S)*A = S*A
    value_type _eps;
    size_type _max_iter;
    data_type _S;
    data_type _N;
    bool _is_init_S;
    bool _is_init_N;
    _Tensor _data_tensor;
};


} // NAMESPACE DETAIL

#endif // SIGN_TENSOR_WRAPPER_BONES_H
