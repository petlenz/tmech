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
