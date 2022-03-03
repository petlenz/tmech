// Copyright 2021 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------

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
