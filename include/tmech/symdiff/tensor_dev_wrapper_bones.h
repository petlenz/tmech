#ifndef TENSOR_DEV_WRAPPER_BONES_H
#define TENSOR_DEV_WRAPPER_BONES_H

namespace detail {

template <typename _Expr>
class tensor_dev_wrapper : public variable_base<tensor_dev_wrapper<_Expr>>
{
    friend class variable_base<tensor_dev_wrapper<_Expr>>;
public:
    using data_type = typename _Expr::data_type;

    tensor_dev_wrapper();

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

  private:
    static data_type _data;
    _Expr _expr;
};

template <typename _Expr>
typename tensor_dev_wrapper<_Expr>::data_type tensor_dev_wrapper<_Expr>::_data;

}

#endif // TENSOR_DEV_WRAPPER_BONES_H
