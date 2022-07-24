/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef VOLUMETRIC_WRAPPER_BONES_H
#define VOLUMETRIC_WRAPPER_BONES_H

namespace detail {

/**
* @class volumetric_wrapper
* @brief Wrapper for the determination of the
* volumetric part of a tensor expression.
*
* @tparam Tensor Tensor expression from which the
* volumetric part should be determined.
*/
template <typename Tensor>
class volumetric_wrapper : public tensor_base<volumetric_wrapper<Tensor>>
{
    using basetype = tensor_base<volumetric_wrapper<Tensor>>;
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    constexpr volumetric_wrapper(data_type_tensor const& data)noexcept;

    constexpr volumetric_wrapper(volumetric_wrapper const& data)noexcept;

    constexpr inline auto operator ()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    value_type _trace;
    Tensor _data;
};

} // NAMESPACE DETAIL

#endif // VOLUMETRIC_WRAPPER_BONES_H
