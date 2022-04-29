/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_BASE_H
#define TENSOR_BASE_H

/**
* @class tensor_base
* @brief Base class of tensor objects.
*
* @tparam _Derived Derived tensor object
*/
template<typename _Derived>
class tensor_base
{
public:
    constexpr tensor_base()noexcept:
        _is_init(false)
    {}

    constexpr tensor_base(tensor_base const& __data)noexcept:
        _is_init(false)
    {}

    constexpr inline auto& convert() const noexcept{
        return *static_cast<const _Derived*>(this);
    }

    constexpr inline auto& convert()noexcept{
        return *static_cast<_Derived*>(this);
    }

    constexpr inline auto operator-()const noexcept{
        if constexpr (std::is_lvalue_reference_v<decltype (this)>){
            return detail::negative_tensor_wrapper<_Derived const&>(this->convert());
        }else{
            return detail::negative_tensor_wrapper<_Derived>(this->convert());
        }
    }

    template<typename DerivedOut>
    friend std::ostream & operator<<(std::ostream &os, const tensor_base<_Derived>& tensor);

    static constexpr inline auto size()noexcept{
        return detail::get_tensor_size<_Derived::dimension(), _Derived::rank()>::size;
    }

    //if constexpr(std::experimental::is_detected<detail::has_evaluate, Derived, decltype (*this)>::value){
    //template<typename _Data, typename __Derived = _Derived, typename std::enable_if_t<std::experimental::is_detected<detail::has_evaluate, __Derived, _Data>> * = nullptr>
    //constexpr inline auto evaluate(_Data & __data){
    //    if constexpr(std::experimental::is_detected<has_update_imp, _Derived, _Data>::value){
    //        return static_cast<_Derived*>(this)->update_imp(__data);
    //    }
    //}

protected:
    bool _is_init;
};

template<typename _Tensor,  std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
std::ostream & operator<<(std::ostream &os, _Tensor&& __tensor)
{
    using TensorType = typename std::decay<_Tensor>::type;

    tensor<typename TensorType::value_type, TensorType::dimension(), TensorType::rank()> temp{std::forward<_Tensor&&>(__tensor)};
    temp.print(os);
    return os;
}



#endif // TENSOR_BASE_H
