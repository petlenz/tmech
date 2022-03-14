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
#ifndef ISOTROPIC_TENSOR_FUNCTIONS_BONES_H
#define ISOTROPIC_TENSOR_FUNCTIONS_BONES_H


namespace detail {

struct sqrt_{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return std::sqrt(value);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return T(1)/(T(2)*std::sqrt(value));
    }
};

struct log{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return std::log(value);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return T(1)/value;
    }
};

struct exp{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return std::exp(value);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return std::exp(value);
    }
};

struct positive_part{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return value >= T(0) ? value : T(0);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return value >= T(0) ? T(1) : T(0);
    }
};

struct negative_part{
    template<typename  T>
    static constexpr inline auto apply(T const& value){
        return value <= T(0) ? value : T(0);
    }

    template<typename  T>
    static constexpr inline auto derivative(T const& value){
        return value <= T(0) ? T(1) : T(0);
    }
};


template <typename Tensor, typename Func>
class isotropic_tensor_function : public tensor_base<isotropic_tensor_function<Tensor, Func>>
{
    using data_type_tensor = typename std::remove_const<typename std::remove_reference<Tensor>::type>::type;

    static constexpr auto Dim{data_type_tensor::dimension()};
    static constexpr auto Rank{data_type_tensor::rank()};
public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;

    static_assert(Dim == 2 || Dim == 3, "isotropic_tensor_function: only for Dim = 2, 3");
    static_assert(Rank == 2,            "isotropic_tensor_function: only only for Rank = 2");

    constexpr isotropic_tensor_function(Tensor data);

    constexpr isotropic_tensor_function(isotropic_tensor_function const& data);

    constexpr inline auto operator()(size_type const i, size_type const j)const;

    static constexpr inline auto rank();

    static constexpr inline auto dimension();

    constexpr inline auto derivative();

    constexpr inline auto evaluate();

    constexpr inline auto get_derivative()const;

    constexpr inline auto raw_data()const{
        return _data.raw_data();
    }

private:
    constexpr inline auto evaluate_Gij();

    tensor<value_type, Dim, 2> _data;
    tensor<value_type, Dim, 4> _deriv;
    std::array<std::array<value_type, Dim>, Dim> _Gij;
    eigen_decomposition_wrapper<Tensor> _eigen_decomposition;
};

} // NAMESPACE DETAIL

#endif // ISOTROPIC_TENSOR_FUNCTIONS_BONES_H
