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
#ifndef POLAR_DECOMPOSITION_V_WRAPPER_BONES_H
#define POLAR_DECOMPOSITION_V_WRAPPER_BONES_H

namespace detail {

template <typename _Base, typename _Tensor>
class polar_decomposition_V_wrapper : public tensor_base<polar_decomposition_V_wrapper<_Base, _Tensor>>
{
public:
    using size_type  = std::size_t;
    using value_type = typename _Tensor::value_type;

    polar_decomposition_V_wrapper(_Base const& __base, _Tensor const& __V);

    polar_decomposition_V_wrapper(polar_decomposition_V_wrapper const & __data);

    constexpr inline auto const& operator()(size_type const i, size_type const j)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto raw_data()const;

    constexpr inline auto evaluate();

    constexpr inline auto derivative()const;

private:
    _Tensor const& _V;
    _Base const& _base;
};

} // NAMESPACE DETAIL

#endif // POLAR_DECOMPOSITION_V_WRAPPER_BONES_H
