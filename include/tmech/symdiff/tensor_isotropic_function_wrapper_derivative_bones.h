/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
