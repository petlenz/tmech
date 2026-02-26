/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef NEGATIVE_TENSOR_WRAPPER_BONES_H
#define NEGATIVE_TENSOR_WRAPPER_BONES_H


namespace detail {

template <typename Tensor>
class negative_tensor_wrapper : public tensor_base<negative_tensor_wrapper<Tensor>>
{
    using data_type_tensor  = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;
    using basetype = tensor_base<negative_tensor_wrapper<Tensor>>;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    constexpr negative_tensor_wrapper(data_type_tensor const& data)noexcept;

    constexpr negative_tensor_wrapper(negative_tensor_wrapper const& data)noexcept;

    template<typename ...Indices>
    constexpr inline auto operator()(Indices ... indices)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

    template <typename U = data_type_tensor,
              std::enable_if_t<std::experimental::is_detected_v<has_direct_access_t, U>, int> = 0>
    constexpr inline auto direct_access(std::size_t __idx) const noexcept {
        return -_data.direct_access(__idx);
    }

#ifdef TMECH_HAS_XSIMD
    template <typename U = data_type_tensor,
              std::enable_if_t<std::experimental::is_detected_v<has_batch_access_t, U>, int> = 0>
    auto batch_access(std::size_t __idx) const noexcept {
        return -_data.batch_access(__idx);
    }
#endif

private:
    Tensor _data;
};

} // NAMESPACE DETAIL

#endif // NEGATIVE_TENSOR_WRAPPER_BONES_H
