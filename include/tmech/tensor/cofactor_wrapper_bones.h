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

    cofactor_wrapper(data_type_tensor const& data);

    cofactor_wrapper(cofactor_wrapper const& data);

    constexpr inline auto operator()(size_type const i, size_type const j)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    template<typename _Result>
    constexpr inline auto evaluate(_Result & result);

    constexpr inline auto raw_data()const;

    constexpr inline auto evaluate();

private:
    template<typename _Result>
    constexpr inline auto evaluate_imp(_Result & result);

    static constexpr inline auto evaluate_details(value_type * result, value_type const*const data);

    static constexpr inline auto cofactors_details_22(value_type * result,
                                                      value_type const A11, value_type const A12,
                                                      value_type const A21, value_type const A22);

    static constexpr inline auto cofactors_details_33(value_type * result,
                                                      value_type const A0, value_type const A1, value_type const A2,
                                                      value_type const A3, value_type const A4, value_type const A5,
                                                      value_type const A6, value_type const A7, value_type const A8);
    tensor<value_type, dimension(), rank()> _data;
    _Tensor _data_basis;
};

} // NAMESPACE DETAIL

#endif // COFACTOR_WRAPPER_BONES_H
