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
#ifndef CONSTANT_BONES_H
#define CONSTANT_BONES_H

template <typename _T, std::size_t _ID>
class constant : public variable_base<constant<_T, _ID>>
{
    friend class variable_base<constant<_T, _ID>>;
public:
    using data_type = _T;

    constant();

    constant(data_type const& __data);

    constant(std::string const& __name);

    constant(data_type const& __data, std::string const& __name);

    ~constant();

    template<typename _Type>
    constexpr inline auto operator=(_Type const& __data);


    template<typename Data>
    constexpr inline auto const& operator()(Data const& __data) const;

    constexpr inline std::ostream& print(std::ostream & __os) const;

    template<typename _Data>
    constexpr inline  auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline  auto update(_Data) = delete;

    constexpr inline  auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& x)const;

    static std::string _name;
    static data_type _data;
};

template <typename _T, std::size_t _ID>
std::string constant<_T, _ID>::_name{};

template <typename _T, std::size_t _ID>
_T constant<_T, _ID>::_data;


#endif // CONSTANT_BONES_H
