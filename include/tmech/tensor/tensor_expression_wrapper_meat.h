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
#ifndef TENSOR_BINARY_EXPRESSION_WRAPPER_MEAT_H
#define TENSOR_BINARY_EXPRESSION_WRAPPER_MEAT_H


namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template<typename _LHS, typename _RHS, typename _Operator>
tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>::tensor_binary_expression_wrapper(data_type_LHS const& __lhs, data_type_RHS const& __rhs):
    _lhs(__lhs),
    _rhs(__rhs)
{}

/**
 * Copy constructor
 */
template<typename _LHS, typename _RHS, typename _Operator>
tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>::tensor_binary_expression_wrapper(tensor_binary_expression_wrapper const& __data):
    _lhs(__data._lhs),
    _rhs(__data._rhs)
{}
//@}


template<typename _LHS, typename _RHS, typename _Operator>
template<typename ..._Indices>
constexpr inline auto tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>::operator()(_Indices ... __indices)const{
    return _Operator::apply(_lhs(__indices...), _rhs(__indices...));
}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension of the expression.
*/
template<typename _LHS, typename _RHS, typename _Operator>
constexpr inline auto tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>::dimension(){
    return data_type_LHS::dimension();
}

/**
 * Returns the rank of the expression.
 */
template<typename _LHS, typename _RHS, typename _Operator>
constexpr inline auto tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>::rank(){
    return data_type_LHS::rank();
}
//@}


template<typename _LHS, typename _RHS, typename _Operator>
constexpr inline auto tensor_binary_expression_wrapper<_LHS, _RHS, _Operator>::evaluate(){
    evaluate::apply(_lhs);
    evaluate::apply(_rhs);
}

} // NAMESPACE DETAIL

#endif // TENSOR_BINARY_EXPRESSION_WRAPPER_MEAT_H
