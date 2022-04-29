/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef RANDN_WRAPPER_MEAT_H
#define RANDN_WRAPPER_MEAT_H

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank>
randn<_T, _Dim, _Rank>::randn(el_type mean, el_type stddev):
    rng(dev()),
    dist(mean, stddev)
{}

/**
 * Copy constructor
 */
template <typename _T, std::size_t _Dim, std::size_t _Rank>
randn<_T, _Dim, _Rank>::randn(randn const& data):
    rng(data.rng),
    dist(data.dist)
{}
//@}


template <typename _T, std::size_t _Dim, std::size_t _Rank>
template<typename ...Indices>
constexpr inline auto randn<_T, _Dim, _Rank>::operator()(const Indices... indices)const{
    return _data(indices...);
}

template <typename _T, std::size_t _Dim, std::size_t _Rank>
constexpr inline auto randn<_T, _Dim, _Rank>::rank(){
    return _Rank;
}

template <typename _T, std::size_t _Dim, std::size_t _Rank>
constexpr inline auto randn<_T, _Dim, _Rank>::dimension(){
    return _Dim;
}

template <typename _T, std::size_t _Dim, std::size_t _Rank>
constexpr inline auto randn<_T, _Dim, _Rank>::evaluate(){
    if(!this->_is_init){
        auto gen = [&](){return dist(rng);};
        std::generate(std::begin(_data.data()), std::end(_data.data()), gen);
        this->_is_init = true;
    }
}

#endif // RANDN_WRAPPER_MEAT_H
