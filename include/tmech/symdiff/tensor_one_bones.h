#ifndef TENSOR_ONE_BONES_H
#define TENSOR_ONE_BONES_H

template <typename _T>
class tensor_one : public variable_base<tensor_one<_T>>
{
    friend class variable_base<tensor_one<_T>>;
public:
    using data_type = typename detail::get_identity_tensor<_T>::data_type;

    static_assert (detail::is_tensor<data_type>::value, "tensor_zero:: data type is not a tensor");

    tensor_one() {}

    template<typename _Data>
    constexpr inline auto operator()(_Data const& /*x*/){
        return typename detail::get_identity_tensor<data_type>::type();
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<"I";
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& )const{
        return typename detail::get_identity_tensor<data_type>::type();
    }
};
#endif // TENSOR_ONE_BONES_H
