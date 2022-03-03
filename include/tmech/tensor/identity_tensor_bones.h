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
#ifndef IDENTITY_TENSOR_BONES_H
#define IDENTITY_TENSOR_BONES_H


/**
* @class eyejoseleon6
* @brief Identity tensor. Only for a Rank which is a multiplier of two.
*
* @tparam T The type of the tensor holding the elements.
* @tparam Dim Dimension of the underlying physical problem.
* @tparam Rank Rank of the tensor. Rank needs to be a multiplier of two.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
class eye : public tensor_base<eye<T, Dim, Rank>>
{
public:
    using value_type = T;
    using size_type = std::size_t;

    static_assert(Rank % 2 == 0,
    "eye: No matching Rank. Only for a Rank which is a multiplier of two");

    constexpr eye();

    ~eye(){}

    template<typename ...Indices>
    constexpr inline auto operator ()(const Indices... indices)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

private:
    template<typename ...Indicies>
    static constexpr inline auto value(size_type const i, size_type const j, Indicies const ... indices);

    static constexpr inline auto value(size_type const i, size_type const j);
};

#endif // IDENTITY_TENSOR_BONES_H
