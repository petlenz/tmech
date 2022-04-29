/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef SYMMETRIC_PART_WRAPPER_BONES_H
#define SYMMETRIC_PART_WRAPPER_BONES_H

namespace detail {

/**
* @class symmetric_part_wrapper
* @brief Wrapper for the determination of the
* inverse of a tensor expression.
*
* @tparam Tensor Tensor expression from which the
* inverse should be determined.
*/
template <typename Tensor>
class symmetric_part_wrapper : public tensor_base<symmetric_part_wrapper<Tensor>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    symmetric_part_wrapper(data_type_tensor const& data)noexcept;

    symmetric_part_wrapper(symmetric_part_wrapper const& data)noexcept;

    constexpr inline auto operator ()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    Tensor _data;
};

} // NAMESPACE DETAIL

#endif // SYMMETRIC_PART_WRAPPER_BONES_H
