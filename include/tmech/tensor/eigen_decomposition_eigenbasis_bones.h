/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EIGEN_DECOMPOSITION_EIGENBASIS_BONES_H
#define EIGEN_DECOMPOSITION_EIGENBASIS_BONES_H

namespace detail {

template <typename Base>
class eigen_decomposition_basis_wrapper
{
public:
    using size_type  = std::size_t;
    using value_type = typename Base::value_type;

    constexpr eigen_decomposition_basis_wrapper(Base const& __basis):
        _basis(__basis)
    {}

    constexpr eigen_decomposition_basis_wrapper(eigen_decomposition_basis_wrapper const& __basis):
        _basis(__basis._basis)
    {}

    constexpr inline auto const& operator[](size_type const __idx)const{
        return _basis._eig_basis[__idx];
    }

private:
    Base const& _basis;
};

} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_EIGENBASIS_BONES_H
