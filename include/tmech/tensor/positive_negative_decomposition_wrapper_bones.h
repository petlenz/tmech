/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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

    positive_negative_decomposition_wrapper(data_type_tensor const& data)noexcept;

    positive_negative_decomposition_wrapper(positive_negative_decomposition_wrapper const& data)noexcept;

    constexpr inline auto positive()const noexcept{
        return positive_negative_decomposition_pos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>(*this, data_pos);
    }

    constexpr inline auto negative()const noexcept{
        return positive_negative_decomposition_neg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>(*this, data_neg);
    }

    constexpr inline auto dpositive()const noexcept{
        return positive_negative_decomposition_dpos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>(*this, deriv_pos);
    }

    constexpr inline auto dnegative()const noexcept{
        return positive_negative_decomposition_dneg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>(*this, deriv_neg);
    }

private:
    friend class positive_negative_decomposition_pos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>;
    friend class positive_negative_decomposition_neg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 2>>;
    friend class positive_negative_decomposition_dpos_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>;
    friend class positive_negative_decomposition_dneg_wrapper<this_type, tensor<value_type, data_type_tensor::dimension(), 4>>;

    constexpr inline auto evaluate_positive()noexcept{
        if(!is_pos){
            decompose_eigen();
            decompose_details<positive_part>(data_pos);
            is_pos = true;
        }
    }

    constexpr inline auto evaluate_negative()noexcept{
        if(!is_neg){
            decompose_eigen();
            decompose_details<negative_part>(data_neg);
            is_neg = true;
        }
    }

    constexpr inline auto evaluate_dpositive()noexcept{
        if(!is_dpos){
            decompose_eigen();
            evaluate_Gij<positive_part>();
            derivative_details(deriv_pos);
            is_dpos = true;
        }
    }

    constexpr inline auto evaluate_dnegative()noexcept{
        if(!is_dneg){
            decompose_eigen();
            evaluate_Gij<negative_part>();
            derivative_details(deriv_neg);
            is_dneg = true;
        }
    }

    constexpr inline auto decompose_eigen()noexcept{
        if(!is_pos || !is_neg || !is_dpos || !is_dneg){
            eigen_decomposition.decompose_eigenbasis();
        }
    }

    template <typename Func>
    constexpr inline auto evaluate_Gij()noexcept;

    template <typename Data>
    constexpr inline auto derivative_details(Data & data)noexcept;

    template <typename Func, typename Data>
    constexpr inline auto decompose_details(Data & data)noexcept;

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
