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
#ifndef RANDU_WRAPPER_BONES_H
#define RANDU_WRAPPER_BONES_H

namespace detail {

template <typename T, bool IsFloatingPoint = false>
struct uniform_distribution
{
    using type = std::uniform_int_distribution<T>;
};

template <typename T>
struct uniform_distribution<T, true>
{
    using type = std::uniform_real_distribution<T>;
};

} // NAMESPACE DETAIL

template <typename T, std::size_t Dim, std::size_t Rank>
class randu : public tensor_base<randu<T, Dim, Rank>>
{
    using uniform_distribution = typename detail::uniform_distribution<T, std::is_floating_point<T>::value>::type ;
public:
    using value_type = T;

    randu(value_type __a, value_type __b = value_type(1)):
        rng(dev()),
        dist(__a, __b)
    {}

    randu(randu const& data):
        rng(data.rng),
        dist(data.dist)
    {}

    template<typename ...Indices>
    constexpr inline auto operator()(const Indices... /*indices*/)const{
        return const_cast<uniform_distribution&>(dist)(const_cast<std::mt19937&>(rng));
    }

    static constexpr inline auto rank(){
        return Rank;
    }

    static constexpr inline auto dimension(){
        return Dim;
    }

private:
    std::random_device dev;
    std::mt19937 rng;
    uniform_distribution dist;
};

#endif // RANDU_WRAPPER_BONES_H
