/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef INVERSE_WRAPPER_BONES_H
#define INVERSE_WRAPPER_BONES_H

//#include <experimental/type_traits>

namespace detail {

/**
* @class inverse_wrapper
* @brief Wrapper for the determination of the
* inverse of a tensor expression.
*
* @tparam Tensor Tensor expression from which the
* inverse should be determined.
*/
template <typename _Tensor, typename ..._Sequences>
class inverse_wrapper : public tensor_base<inverse_wrapper<_Tensor, _Sequences...>>
{
    using basetype = tensor_base<inverse_wrapper<_Tensor, _Sequences...>>;
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;
    using _Tuple = decltype (std::tuple<_Sequences...>(_Sequences()...));

    static_assert(data_type_tensor::rank() != 2      || data_type_tensor::rank() != 4,      "inverse_wrapper: no matching rank");
    static_assert(data_type_tensor::dimension() == 2 || data_type_tensor::dimension() == 3, "inverse_wrapper: no matching dimension");

    constexpr inverse_wrapper(_Tensor __tensor_data)noexcept;

    constexpr inverse_wrapper(inverse_wrapper const& __data)noexcept;

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies... __indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto evaluate()noexcept;

    template<typename _Result>
    constexpr inline auto evaluate(_Result const& __result)noexcept;

    constexpr inline auto raw_data()const noexcept;

private:
    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result const& __result)noexcept;

    static constexpr inline auto evaluate_imp(value_type const* __result, value_type const * __data)noexcept;

    template<std::size_t Rows>
    static constexpr auto lu_detail(value_type const*  __A)noexcept;

    template<std::size_t Rows>
    static constexpr auto inv_lu(value_type * __Ainv, value_type const * const __Afac)noexcept;

    static constexpr inline auto invert_2_2(value_type * result, value_type const A11, value_type const A12,
                                                                 value_type const A21, value_type const A22)noexcept;


    static constexpr inline auto invert_3_3(value_type * result, value_type const A0, value_type const A1, value_type const A2,
                                                                 value_type const A3, value_type const A4, value_type const A5,
                                                                 value_type const A6, value_type const A7, value_type const A8)noexcept;

    tmech::tensor<value_type, dimension(), rank()> data;
    _Tensor tensor_data;
};


} // NAMESPACE DETAIL

#endif // INVERSE_WRAPPER_BONES_H
