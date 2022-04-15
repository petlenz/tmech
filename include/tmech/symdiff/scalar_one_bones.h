/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
