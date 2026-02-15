/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef FUNCTION_WRAPPER_BONES_H
#define FUNCTION_WRAPPER_BONES_H

namespace detail {

template <typename _Expr, typename _Func>
class function_wrapper : public variable_base<function_wrapper<_Expr, _Func>>
{
    friend class variable_base<function_wrapper<_Expr, _Func>>;
public:
    using data_type = decltype (_Func::apply(typename _Expr::data_type()));

    explicit function_wrapper();

    template<typename Data>
    constexpr inline auto const& operator()(Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

  private:
    static data_type _value;
    _Expr _expr;
};

template <typename _Expr, typename _Func>
typename function_wrapper<_Expr, _Func>::data_type function_wrapper<_Expr, _Func>::_value;

}
#endif // FUNCTION_WRAPPER_BONES_H
