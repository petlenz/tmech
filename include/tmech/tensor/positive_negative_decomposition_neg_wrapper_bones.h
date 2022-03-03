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
#ifndef POSITIVE_NEGATIVE_DECOMPOSITION_NEG_WRAPPER_BONES_H
#define POSITIVE_NEGATIVE_DECOMPOSITION_NEG_WRAPPER_BONES_H

namespace detail {

template <typename Base, typename Tensor>
class positive_negative_decomposition_neg_wrapper : public tensor_base<positive_negative_decomposition_neg_wrapper<Base, Tensor>>
{
public:
    using size_type  = std::size_t;
    using value_type = typename Tensor::value_type;

    positive_negative_decomposition_neg_wrapper(Base const & __base, Tensor const& __R);

    positive_negative_decomposition_neg_wrapper(positive_negative_decomposition_neg_wrapper const & __data);

    constexpr inline auto const& operator()(size_type const i, size_type const j)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto raw_data()const;

    constexpr inline auto evaluate();

    constexpr inline auto const& get_base()const{
        return base;
    }

protected:
    constexpr inline auto evaluate_test(){
        std::cout<<"Test"<<std::endl;
    }

private:
    Tensor const& V;
    Base const& base;
};

} // NAMESPACE DETAIL
#endif // POSITIVE_NEGATIVE_DECOMPOSITION_NEG_WRAPPER_BONES_H
