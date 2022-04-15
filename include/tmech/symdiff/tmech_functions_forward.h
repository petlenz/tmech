/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TMECH_FUNCTIONS_FORWARD_H
#define TMECH_FUNCTIONS_FORWARD_H

template<typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto otimes(_VariableLHS const&, _VariableRHS const&);

template<typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto otimesl(_VariableLHS, _VariableRHS);

template<typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto otimesl(_VariableLHS, _VariableRHS);

template<typename _SequenceLHS, typename _SequenceRHS, typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto outer_product(_VariableLHS, _VariableRHS);

template<typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto dcontract(_VariableLHS, _VariableRHS);

template<typename _SequenceLHS, typename _SequenceRHS, typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto inner_product(_VariableLHS, _VariableRHS);

template<typename _Sequence, typename _Expression, typename >
constexpr inline auto basis_change(_Expression);

template<typename _Expression, typename >
constexpr inline auto trans(_Expression);

template<typename _Expression, typename >
constexpr inline auto trace(_Expression);

template<typename _Expression, typename >
constexpr inline auto det(_Expression);

template<typename _Expression, typename >
constexpr inline auto dev(_Expression);

template<typename _Expression, typename >
constexpr inline auto vol(_Expression);

template<typename _Expression, typename >
constexpr inline auto sym(_Expression);

template<typename _Expression, typename >
constexpr inline auto skew(_Expression);

template<typename _Expression, typename >
constexpr inline auto adj(_Expression);

template<typename _Expression, typename >
constexpr inline auto cof(_Expression);

template<typename _Expression, typename >
constexpr inline auto inv(_Expression);

template<typename _Expression, typename >
constexpr inline auto sqrt(_Expression);

template<typename _Expression, typename >
constexpr inline auto exp_sym(_Expression);

template<typename _Expression, typename >
constexpr inline auto log(_Expression);

template<typename _Expression, typename >
constexpr inline auto positive(_Expression);

template<typename _Expression, typename >
constexpr inline auto negative(_Expression);

template<typename _Expression, long long int _L, long long int _R, long long int _E, typename>
constexpr inline auto pow(_Expression, symdiff::real<int, _L, _R, _E>);

template<typename _DataType, std::size_t _ID>
constexpr inline auto as_sym(symdiff::variable<_DataType, _ID>);

#endif // TMECH_FUNCTIONS_FORWARD_H
