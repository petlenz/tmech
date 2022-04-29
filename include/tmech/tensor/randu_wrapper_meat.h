/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef RANDU_WRAPPER_MEAT_H
#define RANDU_WRAPPER_MEAT_H

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank>
randu<_T, _Dim, _Rank>::randu(el_type __a, el_type __b):
    rng(dev()),
    dist(__a, __b)
{}

/**
 * Copy constructor
 */
template <typename _T, std::size_t _Dim, std::size_t _Rank>
randu<_T, _Dim, _Rank>::randu(randu const& data):
    rng(data.rng),
    dist(data.dist)
{}
//@}


template <typename _T, std::size_t _Dim, std::size_t _Rank>
template<typename ...Indices>
constexpr inline auto randu<_T, _Dim, _Rank>::operator()(const Indices... indices)const{
    return _data(indices...);
}

template <typename _T, std::size_t _Dim, std::size_t _Rank>
constexpr inline auto randu<_T, _Dim, _Rank>::rank(){
    return _Rank;
}

template <typename _T, std::size_t _Dim, std::size_t _Rank>
constexpr inline auto randu<_T, _Dim, _Rank>::dimension(){
    return _Dim;
}

template <typename _T, std::size_t _Dim, std::size_t _Rank>
constexpr inline auto randu<_T, _Dim, _Rank>::evaluate(){
    if(!this->_is_init){
        auto gen = [&](){return dist(rng);};
        std::generate(std::begin(_data.data()), std::end(_data.data()), gen);
        this->_is_init = true;
    }
}

#endif // RANDU_WRAPPER_MEAT_H
