/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EXP_WRAPPER_BONES_H
#define EXP_WRAPPER_BONES_H

namespace detail {

template <typename _Father, typename _Tensor>
class exp_derivative_tensor_wrapper;

template <typename _Tensor>
class exp_tensor_wrapper : public tensor_base<exp_tensor_wrapper<_Tensor>>
{
    using data_type_tensor     = typename std::remove_const<typename std::remove_reference<_Tensor>::type>::type;
    static constexpr auto Dim  = data_type_tensor::dimension();
    static constexpr auto Rank = data_type_tensor::rank();
    friend class exp_derivative_tensor_wrapper<exp_tensor_wrapper<_Tensor>, tensor<typename data_type_tensor::value_type, Dim, 4>>;

public:
    using value_type = typename data_type_tensor::value_type;
    using size_type  = std::size_t;
    using data_type  = tensor<value_type, Dim, 2>;

    static_assert(Dim == 2 || Dim == 3, "exp_tensor_wrapper only for Dim == 2, 3");
    static_assert(Rank == 2,            "exp_tensor_wrapper only only for Rank == 2");

    exp_tensor_wrapper(data_type_tensor const& data)noexcept;

    exp_tensor_wrapper(exp_tensor_wrapper const& data)noexcept;

    template<typename ...Indices>
    constexpr inline auto operator()(Indices const... indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto derivative()const noexcept;

    constexpr inline auto evaluate()noexcept;

private:
    constexpr inline auto evaluate_derivative()noexcept;

    tensor<value_type, Dim, 2> _data;
    tensor<value_type, Dim, 4> _derivative;
    size_type _num_of_iter;
    _Tensor _data_base;
};


template <typename _Father, typename _Tensor>
class exp_derivative_tensor_wrapper : public tensor_base<exp_derivative_tensor_wrapper<_Father, _Tensor>>
{
public:
    using value_type = typename _Tensor::value_type;
    using size_type  = std::size_t;

    exp_derivative_tensor_wrapper(_Father const& __father, _Tensor const& __tensor)noexcept:
        _father(__father),
        _dexp(__tensor)
    {}

    exp_derivative_tensor_wrapper(exp_derivative_tensor_wrapper const& __data)noexcept:
        _father(__data._father),
        _dexp(__data._dexp)
    {}

    template<typename ...Indices>
    constexpr inline auto operator()(Indices const... indices)const noexcept{
        return _dexp(indices...);
    }

    static constexpr inline auto rank()noexcept{
        return _Tensor::rank();
    }

    static constexpr inline auto dimension()noexcept{
        return _Tensor::dimension();
    }

    constexpr inline auto evaluate()noexcept{
        const_cast<_Father&>(_father).evaluate_derivative();
    }

private:
    _Father const& _father;
    _Tensor const& _dexp;
};

} // NAMESPACE DETAIL

#endif // EXP_WRAPPER_BONES_H
