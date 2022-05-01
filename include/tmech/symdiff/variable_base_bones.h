/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef VARIABLE_BASE_BONES_H
#define VARIABLE_BASE_BONES_H

template <typename _Derived>
class variable_base
{
    template<typename _DerivedT>
    friend class variable_base;

    template<typename _Type, typename... _Arguments>
    using has_update_imp = decltype(_Type().update_imp(std::declval<_Arguments>()...));

    template<typename _Type, typename... _Arguments>
    using has_reset_imp =  decltype(_Type().reset_imp(std::declval<_Arguments>()...));

public:
    constexpr variable_base();

    constexpr auto convert() const;

    template<typename _Data>
    constexpr inline auto operator()(_Data const& x);

    constexpr inline auto operator-()const;

    template<typename _DerivedT>
    friend std::ostream& operator << (std::ostream const& stream, variable_base<_DerivedT> const& var_base);

    template<typename _Data>
    constexpr inline decltype(auto) value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update(_Data const& __data);

    constexpr inline auto reset();

protected:
    static bool _update;
};

template <typename _Derived>
bool variable_base<_Derived>::_update{false};


#endif // VARIABLE_BASE_BONES_H
