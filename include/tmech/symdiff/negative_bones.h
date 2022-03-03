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
#ifndef NEGATIVE_BONES_H
#define NEGATIVE_BONES_H

namespace detail {

template<typename _Expr>
class negative : public variable_base<negative<_Expr>>
{
    friend class variable_base<negative<_Expr>>;
public:
    using data_type = typename _Expr::data_type;

    negative();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline  auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline  auto update(_Data) = delete;

    constexpr inline  auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data);

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    _Expr _expr;
};

}
#endif // NEGATIVE_BONES_H
