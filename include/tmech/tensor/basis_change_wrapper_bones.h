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

#ifndef BASIS_CHANGE_WRAPPER_BONES_H
#define BASIS_CHANGE_WRAPPER_BONES_H

namespace detail {

template <typename _Tensor, typename _Sequence>
class basis_change_wrapper
        : public tensor_base<basis_change_wrapper<_Tensor, _Sequence>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
    using sequence         = min_value_squence_t<_Sequence, 1>;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    static_assert (_Sequence::size()== data_type_tensor::rank(), "basis_change_wrapper: no matching number of indices and rank");
    static_assert (_Sequence::size() != 0, "basis_change_wrapper: lhs sequence is empty");
    static_assert (!tmech::detail::check_duplicated_in_sequence_v<_Sequence>, "basis_change_wrapper: duplicated values in sequence");

    basis_change_wrapper(_Tensor __data);

    basis_change_wrapper(basis_change_wrapper const& __data);

    template<typename _Derived/*, typename = std::enable_if<!std::is_const_v<_Tensor> && std::is_reference_v<_Tensor>, bool>::type*/>
    constexpr inline auto operator=(tensor_base<_Derived> const& __data_base){
        using function_loop  = typename meta_for_loop_deep<dimension(), rank()-1>::type;
        const auto& __data{__data_base.convert()};
        if constexpr(std::experimental::is_detected<detail::has_evaluate, _Derived>::value){
            const_cast<_Derived&>(__data).evaluate();
        }

        auto func = [&](auto ... numbers){
                tuple_call(_data, std::make_tuple(numbers...), sequence()) = __data(numbers...);
            };
        function_loop::loop(func);
    }

    //template<typename _Derived, typename = std::enable_if<!(std::is_const_v<_Tensor> && std::is_reference_v<_Tensor>), bool>::type>
    //constexpr inline auto operator=(tensor_base<_Derived> const&) = delete;

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies... indices)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto evaluate();

private:
    _Tensor _data;
};

} // NAMESPACE DETAIL

#endif // BASIS_CHANGE_WRAPPER_BONES_H
