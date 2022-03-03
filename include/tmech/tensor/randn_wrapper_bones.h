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
#ifndef RANDN_WRAPPER_BONES_H
#define RANDN_WRAPPER_BONES_H

#include "traits.h"

template <typename _T, std::size_t _Dim, std::size_t _Rank>
class randn : public tensor_base<randn<_T, _Dim, _Rank>>
{
public:
    using value_type = _T;
    using el_type = typename detail::element_type<_T>::value_type;

    randn(el_type mean = 0, el_type stddev = 1):
        rng(dev()),
        dist(mean, stddev)
    {}

    randn(randn const& data):
        rng(data.rng),
        dist(data.dist)
    {}

    template<typename ...Indices>
    constexpr inline auto operator()(const Indices... indices)const{
        return _data(indices...);
    }

    static constexpr inline auto rank(){
        return _Rank;
    }

    static constexpr inline auto dimension(){
        return _Dim;
    }

    constexpr inline auto evaluate(){
        auto gen = [&](){return dist(rng);};
        std::generate(std::begin(_data.data()), std::end(_data.data()), gen);
    }
private:
    std::random_device dev;
    std::mt19937 rng;
    std::normal_distribution<el_type> dist;
    tensor<value_type, _Dim, _Rank> _data;
};

#endif // RANDN_WRAPPER_BONES_H
