/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef COFACTOR_WRAPPER_BONES_H
#define COFACTOR_WRAPPER_BONES_H

namespace detail {

template <typename _Tensor>
class cofactor_wrapper : public tensor_base<cofactor_wrapper<_Tensor>>
{
    using data_type_tensor  = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type = std::size_t;

    cofactor_wrapper(data_type_tensor const& data)noexcept;

    cofactor_wrapper(cofactor_wrapper const& data)noexcept;

    constexpr inline auto operator()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    template<typename _Result>
    constexpr inline auto evaluate(_Result & result)noexcept;

    constexpr inline auto raw_data()const noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result & result)noexcept;

    static constexpr inline auto evaluate_details(value_type * result, value_type const*const data)noexcept;

    static constexpr inline auto cofactors_details_22(value_type * result,
                                                      value_type const A11, value_type const A12,
                                                      value_type const A21, value_type const A22)noexcept;

    static constexpr inline auto cofactors_details_33(value_type * result,
                                                      value_type const A0, value_type const A1, value_type const A2,
                                                      value_type const A3, value_type const A4, value_type const A5,
                                                      value_type const A6, value_type const A7, value_type const A8)noexcept;
    tensor<value_type, dimension(), rank()> _data;
    _Tensor _data_basis;
};

} // NAMESPACE DETAIL

#endif // COFACTOR_WRAPPER_BONES_H
