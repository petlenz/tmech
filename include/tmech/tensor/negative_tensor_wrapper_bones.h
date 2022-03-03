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
#ifndef NEGATIVE_TENSOR_WRAPPER_BONES_H
#define NEGATIVE_TENSOR_WRAPPER_BONES_H


namespace detail {

template <typename Tensor>
class negative_tensor_wrapper : public tensor_base<negative_tensor_wrapper<Tensor>>
{
    using data_type_tensor  = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    constexpr negative_tensor_wrapper(data_type_tensor const& data);

    constexpr negative_tensor_wrapper(negative_tensor_wrapper const& data);

    template<typename ...Indices>
    constexpr inline auto operator ()(Indices ... indices)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto evaluate();

private:
    Tensor _data;
};

} // NAMESPACE DETAIL

#endif // NEGATIVE_TENSOR_WRAPPER_BONES_H
