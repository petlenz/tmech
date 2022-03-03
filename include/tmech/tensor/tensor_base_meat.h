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
#ifndef TENSOR_BASE_H
#define TENSOR_BASE_H

/**
    * @class tensor_base
    * @brief Base class of a tensor and
    * tensor objects.
    *
    * @tparam Derived Derived object
    */
template<typename Derived>
class tensor_base
{
public:
    constexpr tensor_base():
        _is_init(false)
    {}

    constexpr tensor_base(tensor_base const& __data):
        _is_init(false)
    {}

    constexpr inline auto& convert() const {
        return *static_cast<const Derived*>(this);
    }

    constexpr inline auto& convert(){
        return *static_cast<Derived*>(this);
    }

    constexpr inline auto operator-()const{
        if constexpr (std::is_lvalue_reference_v<decltype (this)>){
            return detail::negative_tensor_wrapper<Derived const&>(this->convert());
        }else{
            return detail::negative_tensor_wrapper<Derived>(this->convert());
        }
    }

    template<typename DerivedOut>
    friend std::ostream & operator<<(std::ostream &os, const tensor_base<DerivedOut>& tensor);

    static constexpr inline auto size(){
        return detail::get_tensor_size<Derived::dimension(), Derived::rank()>::size;
    }

protected:
    bool _is_init;
};

template<typename Derived>
std::ostream & operator<<(std::ostream &os, const tensor_base<Derived>& __tensor)
{
    tensor<typename Derived::value_type, Derived::dimension(), Derived::rank()> temp{__tensor};
    temp.print(os);
    return os;
}



#endif // TENSOR_BASE_H
