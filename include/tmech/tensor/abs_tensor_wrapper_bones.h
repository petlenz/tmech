/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ABS_TENSOR_WRAPPER_BONES_H
#define ABS_TENSOR_WRAPPER_BONES_H

namespace detail {

/**
* @class abs_tensor_wrapper
* @brief Element-wise absolute value.
*
* @tparam _Tensor Tensor expression from which the
* element-wise absolute value is to be taken.
*/
template <typename _Tensor>
class abs_tensor_wrapper : public tensor_base<abs_tensor_wrapper<_Tensor>>
{
    using basetype = tensor_base<abs_tensor_wrapper<_Tensor>>;
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    constexpr abs_tensor_wrapper(data_type_tensor const& __data)noexcept;

    constexpr abs_tensor_wrapper(abs_tensor_wrapper const& __data)noexcept;

    template<typename ...Indicies>
    constexpr inline auto operator ()(Indicies ... __indicies)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    _Tensor _data;
};

} // NAMESPACE DETAIL

#endif // ABS_TENSOR_WRAPPER_BONES_H
