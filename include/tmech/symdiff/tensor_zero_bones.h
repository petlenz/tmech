#ifndef TENSOR_ZERO_BONES_H
#define TENSOR_ZERO_BONES_H

template<typename _T>
class tensor_zero : public variable_base<tensor_zero<_T>>
{
    friend class variable_base<tensor_zero<_T>>;
public:
    using data_type = _T;

    static_assert (detail::is_tensor<data_type>::value, "tensor_zero:: data type is not a tensor");

    template<typename _Data>
    constexpr inline auto operator()(_Data const& /*x*/) const{
        return typename detail::get_zero_tensor<data_type>::type();
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<"0"<<detail::get_tensor_info<_T>::rank();
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;
private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& /*__data*/)const{
        return typename detail::get_zero_tensor<data_type>::type();
    }
};

#endif // TENSOR_ZERO_BONES_H
