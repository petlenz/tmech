/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef POLAR_DECOMPOSITION_R_WRAPPER_BONES_H
#define POLAR_DECOMPOSITION_R_WRAPPER_BONES_H

namespace detail {

template <typename _Base, typename _Tensor>
class polar_decomposition_R_wrapper : public tensor_base<polar_decomposition_R_wrapper<_Base, _Tensor>>
{
public:
    using size_type  = std::size_t;
    using value_type = typename _Tensor::value_type;

    polar_decomposition_R_wrapper(_Base const & __base, _Tensor const& __R);

    polar_decomposition_R_wrapper(polar_decomposition_R_wrapper const & __data);

    constexpr inline auto const& operator()(size_type const i, size_type const j)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto raw_data()const;

    constexpr inline auto evaluate();

    constexpr inline auto derivative()const;

private:
    _Tensor const& _R;
    _Base const& _base;
};

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_R_WRAPPER_BONES_H
