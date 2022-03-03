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
#ifndef FUNCTION_WRAPPER_BONES_H
#define FUNCTION_WRAPPER_BONES_H

namespace detail {

template <typename _Expr, typename _Func>
class function_wrapper : public variable_base<function_wrapper<_Expr, _Func>>
{
    friend class variable_base<function_wrapper<_Expr, _Func>>;
public:
    using data_type = decltype (_Func::apply(typename _Expr::data_type()));

    explicit function_wrapper();

    template<typename Data>
    constexpr inline auto const& operator()(Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    static data_type _value;
    _Expr _expr;
};

template <typename _Expr, typename _Func>
typename function_wrapper<_Expr, _Func>::data_type function_wrapper<_Expr, _Func>::_value;

}
#endif // FUNCTION_WRAPPER_BONES_H
