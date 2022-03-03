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
