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

#ifndef ZERO_TENSOR_MEAT_H
#define ZERO_TENSOR_MEAT_H


/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr zeros<T, Dim, Rank>::zeros()
{}
//@}


/**
* @name Data
*/
/**
* @brief Returns a zeros.
* @param indicies A list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indices>
constexpr inline auto zeros<T, Dim, Rank>::operator ()(const Indices... /*indices*/)const{
    return static_cast<value_type>(0);
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
constexpr inline auto zeros<T, Dim, Rank>::dimension(){
    return Dim;
}

/**
 * Returns the rank.
 */
template <typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto zeros<T, Dim, Rank>::rank(){
    return Rank;
}
//@}

#endif // ZERO_TENSOR_MEAT_H
