/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_U_WRAPPER_BONES_H
#define POLAR_DECOMPOSITION_U_WRAPPER_BONES_H

namespace detail {

template <typename _Base, typename _Tensor>
class polar_decomposition_U_wrapper : public tensor_base<polar_decomposition_U_wrapper<_Base, _Tensor>>
{
    using basetype = tensor_base<polar_decomposition_U_wrapper<_Base, _Tensor>>;
public:
    using size_type  = std::size_t;
    using value_type = typename _Tensor::value_type;

    polar_decomposition_U_wrapper(_Base const& __base, _Tensor const& __U)noexcept;

    polar_decomposition_U_wrapper(polar_decomposition_U_wrapper const & __data)noexcept;

    constexpr inline auto const& operator()(size_type const i, size_type const j)const noexcept;

    static constexpr inline auto dimension()noexcept;

    static constexpr inline auto rank()noexcept;

    constexpr inline auto raw_data()const noexcept;

    constexpr inline auto evaluate()noexcept;

    constexpr inline auto derivative()const noexcept;

private:
    _Tensor const& _U;
    _Base const& _base;
};

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_U_WRAPPER_BONES_H
