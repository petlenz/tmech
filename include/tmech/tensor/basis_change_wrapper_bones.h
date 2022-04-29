/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef BASIS_CHANGE_WRAPPER_BONES_H
#define BASIS_CHANGE_WRAPPER_BONES_H

namespace detail {

template <typename _Tensor, typename _Sequence>
class basis_change_wrapper
        : public tensor_base<basis_change_wrapper<_Tensor, _Sequence>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
    using sequence         = min_value_squence_t<_Sequence, 1>;
public:
    using size_type  = std::size_t;
    using value_type = typename data_type_tensor::value_type;

    static_assert (_Sequence::size()== data_type_tensor::rank(), "basis_change_wrapper: no matching number of indices and rank");
    static_assert (_Sequence::size() != 0, "basis_change_wrapper: lhs sequence is empty");
    static_assert (!tmech::detail::check_duplicated_in_sequence_v<_Sequence>, "basis_change_wrapper: duplicated values in sequence");

    basis_change_wrapper(_Tensor __data)noexcept;

    basis_change_wrapper(basis_change_wrapper const& __data)noexcept;

    template<typename _Derived/*, typename = std::enable_if<!std::is_const_v<_Tensor> && std::is_reference_v<_Tensor>, bool>::type*/>
    constexpr inline auto operator=(tensor_base<_Derived> const& __data_base)noexcept;

    template<typename ...Indicies>
    constexpr inline auto operator()(Indicies... indices)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    _Tensor _data;
};

} // NAMESPACE DETAIL

#endif // BASIS_CHANGE_WRAPPER_BONES_H
