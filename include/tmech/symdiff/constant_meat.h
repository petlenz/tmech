/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef CONSTANT_MEAT_H
#define CONSTANT_MEAT_H

template <typename _T, std::size_t _ID>
constant<_T, _ID>::constant(){}

template <typename _T, std::size_t _ID>
constant<_T, _ID>::constant(data_type const& __data){
    _data = __data;
}

template <typename _T, std::size_t _ID>
constant<_T, _ID>::constant(std::string const& __name){
    _name = __name;
}

template <typename _T, std::size_t _ID>
constant<_T, _ID>::constant(data_type const& __data, std::string const& __name){
    _name = __name;
    _data = __data;
}

template <typename _T, std::size_t _ID>
constant<_T, _ID>::~constant(){}

template <typename _T, std::size_t _ID>
template<typename _Type>
constexpr inline auto constant<_T, _ID>::operator=(_Type const& __data){
    _data = __data;
}

template <typename _T, std::size_t _ID>
template<typename Data>
constexpr inline auto const& constant<_T, _ID>::operator()(Data const& __data) const{
    return get_value(__data);
}

template <typename _T, std::size_t _ID>
constexpr inline std::ostream& constant<_T, _ID>::print(std::ostream & __os)const{
    if(_name.empty()){
        __os<<"constant"<<_ID;
    }else{
        __os<<_name;
    }
    return __os;
}

template <typename _T, std::size_t _ID>
template<typename _Data>
constexpr inline auto const& constant<_T, _ID>::get_value(_Data const& x)const{
    return _data;
}

#endif // CONSTANT_MEAT_H
