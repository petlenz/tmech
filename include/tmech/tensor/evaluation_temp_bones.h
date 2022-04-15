/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EVALUATION_TEMP_BONES_H
#define EVALUATION_TEMP_BONES_H

namespace detail {

/**
* @class abs_tensor_wrapper
* @brief Element-wise absolute value.
*
* @tparam Tensor Tensor expression from which the
* element-wise absolute value is to be taken.
*/
template <typename _Tensor>
class eval_tensor_wrapper : public tensor_base<eval_tensor_wrapper<_Tensor>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    constexpr eval_tensor_wrapper(_Tensor data);

    constexpr eval_tensor_wrapper(eval_tensor_wrapper const& data);

    template<typename ...Indices>
    constexpr inline auto operator()(Indices const... indices)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

    constexpr inline auto raw_data()const;

//    constexpr inline auto evaluate(){
//        if(!this->_is_init){
//            _data = _data_base;
//            this->_is_init = true;
//        }
//    }

private:
    tensor<value_type, dimension(), rank()> _data;
    _Tensor _data_base;
};

} // NAMESPACE DETAIL

#endif // EVALUATION_TEMP_BONES_H
