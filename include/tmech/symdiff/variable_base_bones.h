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
#ifndef VARIABLE_BASE_BONES_H
#define VARIABLE_BASE_BONES_H

template <typename _Derived>
class variable_base
{
    template<typename _DerivedT>
    friend class variable_base;

    template<typename _Type, typename... _Arguments>
    using has_update_imp = decltype(_Type().update_imp(std::declval<_Arguments>()...));

    template<typename _Type, typename... _Arguments>
    using has_reset_imp =  decltype(_Type().reset_imp(std::declval<_Arguments>()...));


//    template <typename _T>
//    struct get_data{using type = _T;};

//    template <typename ..._T>
//    struct get_data<std::tuple<_T...>>{using type = ..._T;};

public:
    constexpr variable_base();

    constexpr auto convert() const;

    template<typename _Data>
    constexpr inline auto&& operator()(_Data const& x);

    constexpr inline auto operator-()const;

    template<typename _DerivedT>
    friend std::ostream& operator << (std::ostream const& stream, variable_base<_DerivedT> const& var_base);

    template<typename _Data, typename __Derived = _Derived, typename std::enable_if_t<std::is_lvalue_reference_v<decltype (__Derived().get_value(_Data()))>> * = nullptr>
    constexpr inline auto const& value(_Data const& __data)const{
        return static_cast<const _Derived*>(this)->get_value(__data);
    }

    template<typename _Data, typename __Derived = _Derived, typename std::enable_if_t<!std::is_lvalue_reference_v<decltype (__Derived().get_value(_Data()))>> * = nullptr>
    constexpr inline auto value(_Data const& __data)const{
        return static_cast<const _Derived*>(this)->get_value(__data);
    }

    template<typename _Data>
    constexpr inline auto update(_Data const& __data){
        if constexpr(std::experimental::is_detected<has_update_imp, _Derived, _Data>::value){
            static_cast<_Derived*>(this)->update_imp(__data);
        }
    }

    constexpr inline auto reset(){
        if constexpr(std::experimental::is_detected<has_reset_imp, _Derived>::value){
            static_cast<_Derived*>(this)->reset_imp();
        }
    }

protected:
    static bool _update;
};

template <typename _Derived>
bool variable_base<_Derived>::_update{false};


#endif // VARIABLE_BASE_BONES_H
