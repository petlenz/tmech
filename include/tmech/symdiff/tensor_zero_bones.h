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
#ifndef TENSOR_ZERO_BONES_H
#define TENSOR_ZERO_BONES_H

template<typename _T>
class tensor_zero : public variable_base<tensor_zero<_T>>
{
    friend class variable_base<tensor_zero<_T>>;
public:
    using data_type = _T;

    tensor_zero();

    static_assert (detail::is_tensor<data_type>::value, "tensor_zero:: data type is not a tensor");

    template<typename _Data>
    constexpr inline auto operator()(_Data const& /*__data*/) const;

    constexpr inline std::ostream& print(std::ostream & __os) const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& /*__data*/) const;
};

#endif // TENSOR_ZERO_BONES_H
