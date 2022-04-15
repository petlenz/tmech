/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_ONE_BONES_H
#define TENSOR_ONE_BONES_H

template <typename _T>
class tensor_one : public variable_base<tensor_one<_T>>
{
    friend class variable_base<tensor_one<_T>>;
public:
    using data_type = typename detail::get_identity_tensor<_T>::data_type;

    static_assert (detail::is_tensor<data_type>::value, "tensor_zero:: data type is not a tensor");

    tensor_one();

    template<typename _Data>
    constexpr inline auto operator()(_Data const& /*__data*/);

    constexpr inline std::ostream& print(std::ostream & __os)const;

    template<typename _Data>
    constexpr auto value(_Data) = delete;

    template<typename _Data>
    constexpr auto update(_Data) = delete;

    constexpr auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& /*__data*/)const;
};
#endif // TENSOR_ONE_BONES_H
