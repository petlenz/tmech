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
#ifndef SKEW_SYMMETRIC_PART_WRAPPER_BONES_H
#define SKEW_SYMMETRIC_PART_WRAPPER_BONES_H

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
class skew_symmetric_part_wrapper : public tensor_base<skew_symmetric_part_wrapper<Tensor>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
public:
    using size_type = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    skew_symmetric_part_wrapper(data_type_tensor const& __data);

    skew_symmetric_part_wrapper(skew_symmetric_part_wrapper const& __data);

    constexpr inline auto operator()(size_type const i, size_type const j)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto evaluate();

private:
    data_type_tensor _data;
};

} // NAMESPACE DETAIL

#endif // SKEW_SYMMETRIC_PART_WRAPPER_BONES_H
