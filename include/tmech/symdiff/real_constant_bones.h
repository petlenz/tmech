/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef REAL_CONSTANT_BONES_H
#define REAL_CONSTANT_BONES_H

template<typename _T, long long int _L, long long int _R, long long int _Exp>
class real : public variable_base<real<_T, _L, _R, _Exp>>
{
    friend class variable_base<real<_T, _L, _R, _Exp>>;
public:
    using data_type = _T;

    real();

    real(real const& __data);

    template<typename _Data>
    constexpr inline auto operator()(_Data const& __data) const;

    inline std::string get_string()const;

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& /*__data*/)const;

    double _value;
};

#endif // REAL_CONSTANT_BONES_H
