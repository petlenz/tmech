/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_EXPRESSION_WRAPPER_BONES_H
#define TENSOR_EXPRESSION_WRAPPER_BONES_H

namespace detail {

/**
* @class Binary tensor expression wrapper
* @brief
*
* @tparam _LHS
* @tparam _RHS
* @tparam _Operator
*/
//Scalar needs to be RHS
template<typename _LHS, typename _RHS, typename _Operator>
class tensor_binary_expression_wrapper : public tensor_base<tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>>
{
    using basetype = tensor_base<tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>>;
    using data_type_RHS = typename std::remove_const<typename std::remove_reference<_RHS>::type>::type;
    using data_type_LHS = typename std::remove_const<typename std::remove_reference<_LHS>::type>::type;
public:
    using value_type = typename result_type<typename data_type_LHS::value_type, typename data_type_RHS::value_type>::value_type;
    using size_type = std::size_t;

    tensor_binary_expression_wrapper(data_type_LHS const& __lhs, data_type_RHS const& __rhs)noexcept;

    tensor_binary_expression_wrapper(tensor_binary_expression_wrapper const& __data)noexcept;

    template<typename ..._Indices>
    constexpr inline auto operator()(_Indices... __indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

    constexpr inline auto evaluate()noexcept;

    static constexpr inline auto simple_evaluation()noexcept{
        return data_type_RHS::simple_evaluation() && data_type_LHS::simple_evaluation();
    }

    constexpr inline auto direct_access(std::size_t __idx)const noexcept{
        return _Operator::apply(_lhs.direct_access(__idx), _rhs.direct_access(__idx));
    }

private:
    _LHS _lhs;
    _RHS _rhs;
};

} // NAMESPACE DETAIL

#endif // TENSOR_EXPRESSION_WRAPPER_BONES_H
