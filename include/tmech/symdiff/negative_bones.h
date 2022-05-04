/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
    constexpr inline auto get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    _Expr _expr;
};

}
#endif // NEGATIVE_BONES_H
