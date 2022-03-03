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
#ifndef IDENTITY_TENSOR_MEAT_H
#define IDENTITY_TENSOR_MEAT_H


/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr eye<T, Dim, Rank>::eye()
{}
//@}

/**
* @name Data
*/
/**
* Returns the element at the specified position in the expression.
* @param indicies a list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indices>
constexpr inline auto eye<T, Dim, Rank>::operator ()(const Indices... indices)const{
    return static_cast<value_type>(value(indices...));
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto eye<T, Dim, Rank>::dimension(){
    return Dim;
}

/**
* @brief Returns the rank.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto eye<T, Dim, Rank>::rank(){
    return Rank;
}
//@}

/**
* @name Private functions
*/
/**
* @brief Recursive function to determine the element at the specified position.
* @param indicies a list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indicies>
constexpr inline auto eye<T, Dim, Rank>::value(size_type const i, size_type const j, Indicies const ... indices){
    return (i == j ? 1 : 0)*value(indices...);
}

/**
* @brief Determines the element at the specified position.
* @param i First index specifying the position in the tensor expression.
* @param j Second index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto eye<T, Dim, Rank>::value(size_type const i, size_type const j){
    return (i == j ? 1 : 0);
}
//@}


#endif // IDENTITY_TENSOR_MEAT_H
