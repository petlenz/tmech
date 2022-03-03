// Copyright 2021 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------
#ifndef INVERSE_WRAPPER_BONES_H
#define INVERSE_WRAPPER_BONES_H

#include <experimental/type_traits>

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
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;
    using _Tuple = decltype (std::tuple<_Sequences...>(_Sequences()...));

    static_assert(data_type_tensor::rank() != 2      || data_type_tensor::rank() != 4,      "inverse_wrapper: no matching rank");
    static_assert(data_type_tensor::dimension() == 2 || data_type_tensor::dimension() == 3, "inverse_wrapper: no matching dimension");

    constexpr inverse_wrapper(_Tensor __tensor_data);

    constexpr inverse_wrapper(inverse_wrapper const& __data);

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies... __indices)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

    constexpr inline auto evaluate();

    template<typename _Result>
    constexpr inline auto evaluate(_Result const& __result);

    constexpr inline auto raw_data()const;

private:
    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result const& __result);

    static constexpr inline auto evaluate_imp(value_type const* __result, value_type const * __data);

    template<std::size_t Rows>
    static constexpr auto lu_detail(value_type const*  __A);

    template<std::size_t Rows>
    static constexpr auto inv_lu(value_type * __Ainv, value_type const * const __Afac);

    static constexpr inline auto invert_2_2(value_type * result, value_type const A11, value_type const A12,
                                                                 value_type const A21, value_type const A22);


    static constexpr inline auto invert_3_3(value_type * result, value_type const A0, value_type const A1, value_type const A2,
                                                                 value_type const A3, value_type const A4, value_type const A5,
                                                                 value_type const A6, value_type const A7, value_type const A8);

    tmech::tensor<value_type, dimension(), rank()> data;
    _Tensor tensor_data;
};


} // NAMESPACE DETAIL

#endif // INVERSE_WRAPPER_BONES_H
