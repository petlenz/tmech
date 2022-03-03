#ifndef TMECH_FUNCTIONS_FORWARD_H
#define TMECH_FUNCTIONS_FORWARD_H

template<typename _VariableLHS, typename _VariableRHS, typename, typename>
constexpr inline auto otimes(_VariableLHS, _VariableRHS);

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
