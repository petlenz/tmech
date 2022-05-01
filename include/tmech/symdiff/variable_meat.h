/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef VARIABLE_MEAT_H
#define VARIABLE_MEAT_H

template <typename _T, std::size_t _VarID>
constexpr variable<_T, _VarID>::variable(){}

template <typename _T, std::size_t _VarID>
constexpr variable<_T, _VarID>::variable(std::string const& __var_name){
    _var_name = __var_name;
}

template <typename _T, std::size_t _VarID>
constexpr variable<_T, _VarID>::variable(std::string && __var_name){
    _var_name = std::move(__var_name);
}

template <typename _T, std::size_t _VarID>
variable<_T, _VarID>::~variable(){}

template <typename _T, std::size_t _VarID>
template<typename _Data>
constexpr inline decltype(auto) variable<_T, _VarID>::operator()(_Data const& __data) const{
    return get_value(__data);
}


template <typename _T, std::size_t _VarID>
constexpr inline auto variable<_T, _VarID>::get_string()const{
    if(_var_name.empty()){
        return std::string("var")+std::to_string(_VarID);
    }else{
        return _var_name;
    }
}

template <typename _T, std::size_t _VarID>
constexpr inline std::ostream& variable<_T, _VarID>::print(std::ostream & __os)const{
    if(_var_name.empty()){
        __os<<"var"<<_VarID;
    }else{
        __os<<_var_name;
    }
    return __os;
}

template <typename _T, std::size_t _VarID>
template<typename _Data>
constexpr inline decltype(auto) variable<_T, _VarID>::get_value(_Data const& __data)const{
    return std::get<_VarID>(__data);
}

#endif // VARIABLE_MEAT_H
