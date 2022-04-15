/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
