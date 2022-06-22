#ifndef TENSOR_FOURTH_ORDER_IDENTITY_WRAPPER_BONES_H
#define TENSOR_FOURTH_ORDER_IDENTITY_WRAPPER_BONES_H

namespace symdiff {
namespace detail {

template <typename _T>
class tensor_sym_identity_wrapper : public variable_base<tensor_sym_identity_wrapper<_T>>
{
    friend class variable_base<tensor_sym_identity_wrapper<_T>>;
    using tensor_info = get_tensor_info<typename _T::data_type>;
public:
    using data_type = typename get_tensor<typename tensor_info::value_type, tensor_info::dimension(), 4>::type;

    static_assert (detail::is_tensor<data_type>::value, "tensor_sym_identity_wrapper:: data type is not a tensor");

    tensor_sym_identity_wrapper();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& /*__data*/);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& /*__data*/)const;
};

template <typename _T>
class tensor_dev_identity_wrapper : public variable_base<tensor_dev_identity_wrapper<_T>>
{
    friend class variable_base<tensor_dev_identity_wrapper<_T>>;
    using tensor_info = get_tensor_info<typename _T::data_type>;
public:
    using data_type = typename get_tensor<typename tensor_info::value_type, tensor_info::dimension(), 4>::type;

    static_assert (detail::is_tensor<data_type>::value, "tensor_dev_identity_wrapper:: data type is not a tensor");

    tensor_dev_identity_wrapper();

    template<typename _Data>
    constexpr inline decltype(auto) operator()(_Data const& /*__data*/);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline decltype(auto) get_value(_Data const& /*__data*/)const;
};

template <typename _T>
class tensor_vol_identity_wrapper : public variable_base<tensor_vol_identity_wrapper<_T>>
{
    friend class variable_base<tensor_vol_identity_wrapper<_T>>;
    using tensor_info = get_tensor_info<typename _T::data_type>;
public:
    using data_type = typename get_tensor<typename tensor_info::value_type, tensor_info::dimension(), 4>::type;

    static_assert (detail::is_tensor<data_type>::value, "tensor_vol_identity_wrapper:: data type is not a tensor");

    tensor_vol_identity_wrapper();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& /*__data*/);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& /*__data*/)const;

};

}
}
#endif // TENSOR_FOURTH_ORDER_IDENTITY_WRAPPER_BONES_H
