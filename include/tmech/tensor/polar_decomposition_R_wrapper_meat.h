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
#ifndef POLAR_DECOMPOSITION_R_WRAPPER_MEAT_H
#define POLAR_DECOMPOSITION_R_WRAPPER_MEAT_H

namespace detail {

template <typename _Base, typename _Tensor>
polar_decomposition_R_wrapper<_Base, _Tensor>::polar_decomposition_R_wrapper(_Base const & __base, _Tensor const& __R):
    _base(__base),
    _R(__R)
{}

template <typename _Base, typename _Tensor>
polar_decomposition_R_wrapper<_Base, _Tensor>::polar_decomposition_R_wrapper(polar_decomposition_R_wrapper const & __data):
    _base(__data._base),
    _R(__data._R)
{}

template <typename _Base, typename _Tensor>
constexpr inline auto const& polar_decomposition_R_wrapper<_Base, _Tensor>::operator()(size_type const i, size_type const j)const{
    return _R(i,j);
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_R_wrapper<_Base, _Tensor>::dimension(){
    return _Tensor::dimension();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_R_wrapper<_Base, _Tensor>::rank(){
    return _Tensor::rank();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_R_wrapper<_Base, _Tensor>::raw_data()const{
    return _R.raw_data();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_R_wrapper<_Base, _Tensor>::evaluate(){
    const_cast<_Base&>(_base).evaluate();
}

template <typename _Base, typename _Tensor>
constexpr inline auto polar_decomposition_R_wrapper<_Base, _Tensor>::derivative()const{
    return polar_decomposition_dR_wrapper<_Base, typename _Base::tensor4>(_base, _base._dR);
}

} // NAMESPACE DETAIL
#endif // POLAR_DECOMPOSITION_R_WRAPPER_MEAT_H
