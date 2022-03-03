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
#ifndef POSITVE_NEGATIVE_DECOMPOSITION_WRAPPER_BONES_H
#define POSITVE_NEGATIVE_DECOMPOSITION_WRAPPER_BONES_H

namespace detail {

template <typename _Tensor>
class positive_negative_decomposition_wrapper
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;
    using this_type = positive_negative_decomposition_wrapper<_Tensor>;

    positive_negative_decomposition_wrapper(data_type_tensor const& data);

    positive_negative_decomposition_wrapper(positive_negative_decomposition_wrapper const& data);

    constexpr inline auto positive()const{
        return positive_negative_decomposition_pos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>(*this, data_pos);
    }

    constexpr inline auto negative()const{
        return positive_negative_decomposition_neg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>(*this, data_neg);
    }

    constexpr inline auto dpositive()const{
        return positive_negative_decomposition_dpos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>(*this, deriv_pos);
    }

    constexpr inline auto dnegative()const{
        return positive_negative_decomposition_dneg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>(*this, deriv_neg);
    }

private:
    friend class positive_negative_decomposition_pos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>;
    friend class positive_negative_decomposition_neg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>;
    friend class positive_negative_decomposition_dpos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>;
    friend class positive_negative_decomposition_dneg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>;

    constexpr inline auto evaluate_positive(){
        if(!is_pos){
            decompose_eigen();
            decompose_details<positive_part>(data_pos);
            is_pos = true;
        }
    }

    constexpr inline auto evaluate_negative(){
        if(!is_neg){
            decompose_eigen();
            decompose_details<negative_part>(data_neg);
            is_neg = true;
        }
    }

    constexpr inline auto evaluate_dpositive(){
        if(!is_dpos){
            decompose_eigen();
            evaluate_Gij<positive_part>();
            derivative_details(deriv_pos);
            is_dpos = true;
        }
    }

    constexpr inline auto evaluate_dnegative(){
        if(!is_dneg){
            decompose_eigen();
            evaluate_Gij<negative_part>();
            derivative_details(deriv_neg);
            is_dneg = true;
        }
    }

    constexpr inline auto decompose_eigen(){
        if(!is_pos || !is_neg || !is_dpos || !is_dneg){
            eigen_decomposition.decompose_eigenbasis();
        }
    }

    template <typename Func>
    constexpr inline auto evaluate_Gij();

    template <typename Data>
    constexpr inline auto derivative_details(Data & data);

    template <typename Func, typename Data>
    constexpr inline auto decompose_details(Data & data);

    bool is_pos;
    bool is_neg;
    bool is_dpos;
    bool is_dneg;
    tensor<value_type, data_type_tensor::dimension(), 2> data_pos;
    tensor<value_type, data_type_tensor::dimension(), 2> data_neg;
    tensor<value_type, data_type_tensor::dimension(), 4> deriv_pos;
    tensor<value_type, data_type_tensor::dimension(), 4> deriv_neg;
    std::array<std::array<value_type, data_type_tensor::dimension()>, data_type_tensor::dimension()> Gij;
    eigen_decomposition_wrapper<_Tensor> eigen_decomposition;
};

} // NAMESPACE DETAIL

#endif // POSITVE_NEGATIVE_DECOMPOSITION_WRAPPER_BONES_H
