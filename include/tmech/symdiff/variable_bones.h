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
#ifndef VARIABLE_BONES_H
#define VARIABLE_BONES_H


template <typename T, std::size_t VarID>
class variable : public variable_base<variable<T, VarID>>
{
    friend class variable_base<variable<T, VarID>>;
public:
    using data_type = T;

    constexpr variable();

    constexpr variable(std::string const& __var_name);

    constexpr variable(std::string && __var_name);

    ~variable();

    template<typename _Data, typename std::enable_if_t<std::tuple_size_v<_Data> != 0> * = nullptr>
    constexpr inline auto const& operator()(_Data const& __data) const;

    constexpr inline auto get_string()const;

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data, typename std::enable_if_t<std::tuple_size_v<_Data> != 0> * = nullptr>
    constexpr inline auto const& get_value(_Data const& __data)const;

    static std::string _var_name;
};

template <typename _T, std::size_t _VarID>
std::string variable<_T, _VarID>::_var_name{};


#endif // VARIABLE_BONES_H
