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
#ifndef VARIABLE_BASE_MEAT_H
#define VARIABLE_BASE_MEAT_H

template <typename _Derived>
constexpr variable_base<_Derived>::variable_base()
{}

template <typename _Derived>
constexpr auto variable_base<_Derived>::convert() const {return *static_cast<const _Derived*>(this);}

template <typename _Derived>
template<typename _Data>
constexpr inline auto&& variable_base<_Derived>::operator()(_Data const& __data){
    if constexpr(std::experimental::is_detected<detail::has_update, _Derived, _Data>::value){
        static_cast<const _Derived*>(this)->reset();
    }
    if constexpr(std::experimental::is_detected<detail::has_reset, _Derived>::value){
        static_cast<const _Derived*>(this)->update(__data);
    }
    return static_cast<const _Derived*>(this)->get_value(__data);
}

template <typename _Derived>
constexpr inline auto variable_base<_Derived>::operator-()const{
    return detail::negative<_Derived>();
}

template <typename _Derived>
std::ostream& operator << (std::ostream & stream, variable_base<_Derived> const& var_base){
    return var_base.convert().print(stream);
}

#endif // VARIABLE_BASE_MEAT_H
