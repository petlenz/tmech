/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef VECTOR_of_FUNCTIONS_H
#define VECTOR_of_FUNCTIONS_H

#include <tuple>

namespace detail {

template <typename ...Functions>
class vector_of_functions
{
public:
    using Data = std::tuple<Functions...>;
    using size_type = std::size_t;
    using value_type = typename std::tuple_element<0, Data>::type::data_type;

    vector_of_functions():
        _data()
    {}

    template<typename Data>
    constexpr inline auto const& operator()(Data const& data){
        update_details(data);
        return _vector;
    }

    template<typename Data>
    constexpr inline auto update(Data const& data){
        update_details(data);
    }

    template<size_type ID>
    static constexpr inline auto get_function(){
        return std::get<ID>(Data());
    }

    static constexpr size_type size{sizeof...(Functions)};

    constexpr inline auto const& data()const{
        return _vector;
    }

private:
    template<size_type I = 0, typename Data>
    constexpr inline auto update_details(Data const& data){
        if constexpr (I < size){
            auto f{get_function<I>()};
            _vector[I] = f(data);
            update_details<I+1>(data);
        }
    }

    Data _data;
    std::array<value_type, size> _vector;
};
}
#endif // VECTOR_of_FUNCTIONS_H
