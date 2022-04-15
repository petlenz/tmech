/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_POW_WRAPPER_BONES_H
#define TENSOR_POW_WRAPPER_BONES_H

namespace detail {

template <typename _Expr, typename _RealExpo>
class tensor_pow_wrapper : public variable_base<tensor_pow_wrapper<_Expr, _RealExpo>>
{
    friend class variable_base<tensor_pow_wrapper<_Expr, _RealExpo>>;
    friend class tensor_pow_wrapper_derivative<tensor_pow_wrapper<_Expr, _RealExpo>>;
public:
    using data_type = typename _Expr::data_type;

    tensor_pow_wrapper();

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    static tmech::detail::pow_tensor_wrapper<data_type const&> _pow_data;
    static data_type _data;
    _Expr _expr;
};

template <typename _Expr, typename _RealExpo>
tmech::detail::pow_tensor_wrapper<typename tensor_pow_wrapper<_Expr, _RealExpo>::data_type const&>
tensor_pow_wrapper<_Expr, _RealExpo>::_pow_data(tensor_pow_wrapper<_Expr, _RealExpo>::_data, _RealExpo()(std::tuple<>()));

template <typename _Expr, typename _RealExpo>
typename tensor_pow_wrapper<_Expr, _RealExpo>::data_type tensor_pow_wrapper<_Expr, _RealExpo>::_data;

}

#endif // TENSOR_POW_WRAPPER_BONES_H
