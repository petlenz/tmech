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
#ifndef SCALAR_ONE_BONES_H
#define SCALAR_ONE_BONES_H


template<typename T>
class scalar_one : public variable_base<scalar_one<T>>
{
    friend class variable_base<scalar_one<T>>;
public:
    using data_type = T;

    static_assert (detail::is_scalar<T>::value, "scalar_one: no matching data type");

    constexpr scalar_one();

    template<typename _Data>
    constexpr inline data_type operator()(_Data const& __data) const;

    inline std::string get_string()const;

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline data_type get_value(_Data const& /*__data*/)const;
};

#endif // SCALAR_ONE_BONES_H
