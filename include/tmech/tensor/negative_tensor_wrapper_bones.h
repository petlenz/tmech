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
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    constexpr negative_tensor_wrapper(data_type_tensor const& data);

    constexpr negative_tensor_wrapper(negative_tensor_wrapper const& data);

    template<typename ...Indices>
    constexpr inline auto operator ()(Indices ... indices)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto evaluate();

private:
    Tensor _data;
};

} // NAMESPACE DETAIL

#endif // NEGATIVE_TENSOR_WRAPPER_BONES_H
