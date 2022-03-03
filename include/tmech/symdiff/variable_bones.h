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
#ifndef VARIABLE_BONES_H
#define VARIABLE_BONES_H


template <typename T, std::size_t VarID>
class variable : public variable_base<variable<T, VarID>>
{
    friend class variable_base<variable<T, VarID>>;
public:
    using data_type = T;

    constexpr variable(){}

    constexpr variable(std::string const& var_name){
        _var_name = var_name;
    }

    constexpr variable(std::string && var_name){
        _var_name = std::move(var_name);
    }

    ~variable(){}

    template<typename Data, typename std::enable_if_t<std::tuple_size_v<Data> != 0> * = nullptr>
    constexpr inline auto const& operator()(Data const& x) const{
        return get_value(x);
    }

    static constexpr inline auto get_ID(){
        return VarID;
    }

    constexpr inline auto get_string()const{
        if(_var_name.empty()){
            return std::string("var")+std::to_string(VarID);
        }else{
            return _var_name;
        }
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        if(_var_name.empty()){
            os<<"var"<<VarID;
        }else{
            os<<_var_name;
        }
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data, typename std::enable_if_t<std::tuple_size_v<_Data> != 0> * = nullptr>
    constexpr inline auto const& get_value(_Data const& __data)const{
        return std::get<VarID>(__data);
    }

    static std::string _var_name;
};

template <typename T, std::size_t VarID>
std::string variable<T, VarID>::_var_name{};


#endif // VARIABLE_BONES_H
