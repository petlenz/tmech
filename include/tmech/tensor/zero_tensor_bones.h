// Copyright 2022 Peter Lenz
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
#ifndef ZERO_TENSOR_BONES_H
#define ZERO_TENSOR_BONES_H


/**
* @class zeros
* @brief Wrapper for a tensor, which contains only zeros.
* No memory is initialised.
*
* @tparam T The type of the tensor holding the elements.
* @tparam Dim Dimension of the underlying physical problem.
* @tparam Rank Rank of the tensor.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
class zeros : public tensor_base<zeros<T, Dim, Rank>>
{
public:
    using value_type = T;
    using size_type  = std::size_t;

    constexpr zeros();

    template<typename ...Indices>
    constexpr inline auto operator ()(const Indices... /*indices*/)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();
};


#endif // ZERO_TENSOR_BONES_H
