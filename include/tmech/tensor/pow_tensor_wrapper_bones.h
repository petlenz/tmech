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
#ifndef POW_TENSOR_WRAPPER_BONES_H
#define POW_TENSOR_WRAPPER_BONES_H

namespace detail {

/**
* @class inverse_wrapper
* @brief Wrapper for the determination of the
* inverse of a tensor expression.
*
* @tparam Tensor Tensor expression from which the
* inverse should be determined.
*/
template <typename Tensor>
class pow_tensor_wrapper : public tensor_base<pow_tensor_wrapper<Tensor>>
{
    using data_type_tensor = typename result_expression_type<Tensor>::result_type;
public:
    using data_type = typename Tensor::data_type;
    using value_type = typename Tensor::value_type;
    using size_type = std::size_t;

    pow_tensor_wrapper(Tensor const& data, size_type const exponent);

    pow_tensor_wrapper(pow_tensor_wrapper const& data);

    template<typename ...Indices>
    constexpr inline auto operator ()(Indices const... indices)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

    constexpr inline auto derivative();

    template<typename Result>
    constexpr inline auto evaluate(Result & result);

    constexpr inline auto evaluate();

    constexpr inline auto raw_data()const;

private:
    template<typename Result>
    constexpr inline auto evaluate_imp(Result & result);

    constexpr inline auto sym_derivative();

    constexpr inline auto skew_derivative();

    constexpr inline auto normal_derivative();

    static constexpr inline auto is_symmetric(value_type const*const ptr);

    static constexpr inline auto is_skewsymmetric(value_type const*const ptr);

    data_type _data;
    data_type_tensor _data_expr;
    data_type _data_deriv;
    tensor<value_type, Tensor::dimension(), Tensor::rank()*2> _derivative;
    std::vector<data_type> _data_i;
    size_type const _expo;
};


} // NAMESPACE DETAIL

#endif // POW_TENSOR_WRAPPER_BONES_H
