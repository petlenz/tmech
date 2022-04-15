/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
