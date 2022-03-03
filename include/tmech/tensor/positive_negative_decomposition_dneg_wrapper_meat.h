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
#ifndef POSITIVE_NEGATIVE_DECOMPOSITION_DNEG_WRAPPER_MEAT_H
#define POSITIVE_NEGATIVE_DECOMPOSITION_DNEG_WRAPPER_MEAT_H

namespace detail {

template <typename Base, typename Tensor>
positive_negative_decomposition_dneg_wrapper<Base, Tensor>::positive_negative_decomposition_dneg_wrapper(Base const& __base, Tensor const& __V):base(__base),V(__V) {}

template <typename Base, typename Tensor>
positive_negative_decomposition_dneg_wrapper<Base, Tensor>::positive_negative_decomposition_dneg_wrapper(positive_negative_decomposition_dneg_wrapper const & __data):base(__data.base),V(__data.V) {}

template <typename Base, typename Tensor>
constexpr inline auto const& positive_negative_decomposition_dneg_wrapper<Base, Tensor>::operator()(size_type const i, size_type const j, size_type const k, size_type const l)const{
    return V(i,j,k,l);
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_dneg_wrapper<Base, Tensor>::dimension(){
    return Tensor::dimension();
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_dneg_wrapper<Base, Tensor>::rank(){
    return Tensor::rank();
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_dneg_wrapper<Base, Tensor>::evaluate(){
    const_cast<Base&>(base).evaluate_dnegative();
}

template <typename Base, typename Tensor>
constexpr inline auto positive_negative_decomposition_dneg_wrapper<Base, Tensor>::raw_data()const{
    return V.raw_data();
}

} // NAMESPACE DETAIL

#endif // POSITIVE_NEGATIVE_DECOMPOSITION_DNEG_WRAPPER_MEAT_H
