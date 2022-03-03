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
#ifndef SCALAR_BONES_H
#define SCALAR_BONES_H

namespace detail {

/**
* @class scalar
* @brief Wrapper for scalar value in a tensor expression.
*
* @tparam _T Tensor expression from which the
* inverse should be determined.
*/
template<typename _T>
class scalar
{
public:
    using value_type = _T;

    constexpr scalar(value_type const scalar);

    constexpr scalar(scalar const& scalar);

    constexpr inline value_type operator()()const;

    template<typename ...Numbers>
    constexpr inline value_type operator()(Numbers...)const;

private:
    value_type const scalar_;
};

} // NAMESPACE DETAIL

#endif // SCALAR_BONES_H
