/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_FUNCTION_WRAPPER_BONES_H
#define TENSOR_FUNCTION_WRAPPER_BONES_H

namespace detail {

template <typename _Expr, typename _Func>
class tensor_function_wrapper : public variable_base<tensor_function_wrapper<_Expr, _Func>>
{
    friend class variable_base<tensor_function_wrapper<_Expr, _Func>>;
public:
    using data_type = typename _Expr::data_type;

    tensor_function_wrapper();

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;


private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    static data_type _value;
    _Expr _expr;
};

template <typename _Expr, typename _Func>
typename tensor_function_wrapper<_Expr, _Func>::data_type tensor_function_wrapper<_Expr, _Func>::_value;

}
#endif // TENSOR_FUNCTION_WRAPPER_BONES_H
