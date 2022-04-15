/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SCALAR_ZERO_BONES_H
#define SCALAR_ZERO_BONES_H

template<typename T>
class scalar_zero : public variable_base<scalar_zero<T>>
{
public:
    using data_type = T;

    constexpr scalar_zero();

    template<typename Data>
    constexpr inline data_type operator()(Data const& __data) const;

    inline std::string get_string()const;

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline data_type get_value(_Data const& __data)const;
};

#endif // SCALAR_ZERO_BONES_H
