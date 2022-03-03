// Copyright 2022 Peter Lenz
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

#endif // TMECH_FUNCTIONS_FORWARD_H
