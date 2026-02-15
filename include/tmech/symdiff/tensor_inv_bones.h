/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_INV_BONES_H
#define TENSOR_INV_BONES_H

namespace detail {

template <typename _Expr, typename ..._Sequences>
class inv_tensor : public variable_base<inv_tensor<_Expr, _Sequences...>>
{
    friend class variable_base<inv_tensor<_Expr, _Sequences...>>;
public:
    using data_type = typename _Expr::data_type;

    inv_tensor();

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

  private:
    static data_type _data;
    _Expr _expr;
};

template <typename _Expr, typename ..._Sequences>
typename inv_tensor<_Expr, _Sequences...>::data_type inv_tensor<_Expr, _Sequences...>::_data;

}


#endif // TENSOR_INV_BONES_H
