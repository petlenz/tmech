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
#ifndef TENSOR_INV_BONES_H
#define TENSOR_INV_BONES_H

namespace detail {

template <typename _Expr, typename ..._Sequences>
class inv_tensor : public variable_base<inv_tensor<_Expr, _Sequences...>>
{
    friend class variable_base<inv_tensor<_Expr, _Sequences...>>;
public:
    using data_type = typename _Expr::data_type;

    inv_tensor();

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    static data_type _data;
    _Expr _expr;
};

template <typename _Expr, typename ..._Sequences>
typename inv_tensor<_Expr, _Sequences...>::data_type inv_tensor<_Expr, _Sequences...>::_data;

}


#endif // TENSOR_INV_BONES_H
