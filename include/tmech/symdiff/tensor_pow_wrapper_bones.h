// Copyright 2022 Peter Lenz
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
