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
#ifndef TENSOR_ISOTROPIC_FUNCTION_WRAPPER_DERIVATIVE_BONES_H
#define TENSOR_ISOTROPIC_FUNCTION_WRAPPER_DERIVATIVE_BONES_H

namespace detail {

template <typename _Father>
class tensor_isotropic_function_wrapper_derivative : public variable_base<tensor_isotropic_function_wrapper_derivative<_Father>>
{
    friend class variable_base<tensor_isotropic_function_wrapper_derivative<_Father>>;
public:
    using tensor_info = get_tensor_info<typename _Father::data_type>;
    using data_type = typename get_tensor<typename tensor_info::value_type, tensor_info::dimension(), tensor_info::rank() + 2>::type;

    tensor_isotropic_function_wrapper_derivative();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& __data);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const;

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data);

    constexpr inline auto reset_imp();

    _Father _father;
};

}
#endif // TENSOR_ISOTROPIC_FUNCTION_WRAPPER_DERIVATIVE_BONES_H
