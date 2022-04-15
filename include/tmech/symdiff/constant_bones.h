/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
