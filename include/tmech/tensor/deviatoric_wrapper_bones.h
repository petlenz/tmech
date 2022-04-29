/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef DEVIATORIC_WRAPPER_BONES_H
#define DEVIATORIC_WRAPPER_BONES_H

namespace detail {

/**
* @class deviatoric_wrapper
* @brief Deivatoric part of two second order tensors.
*
* @tparam Tensor Tensor expression from which the
* deviatoric part should be determined.
*/
template <typename _Tensor>
class deviatoric_wrapper : public tensor_base<deviatoric_wrapper<_Tensor>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    constexpr deviatoric_wrapper(data_type_tensor const& data)noexcept;

    constexpr deviatoric_wrapper(deviatoric_wrapper const& data)noexcept;

    constexpr inline auto operator ()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    value_type _trace;
    _Tensor _data;
};

} // NAMESPACE DETAIL

#endif // DEVIATORIC_WRAPPER_BONES_H
