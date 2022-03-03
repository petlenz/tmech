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
#ifndef LEVI_CIVITA_SYMBOL_MEAT_H
#define LEVI_CIVITA_SYMBOL_MEAT_H



/**
* @name Constructors
*/
//@{
/**
* @brief Default constructor
*/
template <typename T, std::size_t Dim>
constexpr levi_civita<T, Dim>::levi_civita() {}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* @brief Returns the rank.
*/
template <typename T, std::size_t Dim>
constexpr inline auto levi_civita<T, Dim>::rank(){
    return Dim;
}
/**
* @brief Returns the dimension.
*/
template <typename T, std::size_t Dim>
constexpr inline auto levi_civita<T, Dim>::dimension(){
    return Dim;
}
//@}

/**
* @name Data
*/
/**
* @brief Returns the element at the specified position in the expression.
* @param i Index specifying the position in the tensor expression.
* @param j Index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim>
template<std::size_t DIM, typename >
constexpr inline auto levi_civita<T, Dim>::operator()(size_type const i, size_type const j)const{
    return static_cast<value_type>(j) - static_cast<value_type>(i);
}

/**
* @brief Returns the element at the specified position in the expression.
* @param i Index specifying the position in the tensor expression.
* @param j Index specifying the position in the tensor expression.
* @param k Index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim>
template<std::size_t DIM, typename >
constexpr inline auto levi_civita<T, Dim>::operator()(size_type const i, size_type const j, size_type const k)const{
    return (static_cast<value_type>(j) - static_cast<value_type>(i))
            *(static_cast<value_type>(k) - static_cast<value_type>(j))
            *(static_cast<value_type>(i) - static_cast<value_type>(k))*static_cast<value_type>(0.5);
}

/**
* @brief Returns the element at the specified position in the expression.
* @param i Index specifying the position in the tensor expression.
* @param j Index specifying the position in the tensor expression.
* @param k Index specifying the position in the tensor expression.
* @param l Index specifying the position in the tensor expression.
* Indices must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename T, std::size_t Dim>
template<std::size_t DIM, typename>
constexpr inline auto levi_civita<T, Dim>::operator()(size_type const i, size_type const j, size_type const k, size_type const l)const{
    constexpr value_type fac{1./12.};
    return (static_cast<value_type>(j) - static_cast<value_type>(i))
            *(static_cast<value_type>(k) - static_cast<value_type>(i))
            *(static_cast<value_type>(l) - static_cast<value_type>(i))
            *(static_cast<value_type>(k) - static_cast<value_type>(j))
            *(static_cast<value_type>(l) - static_cast<value_type>(j))
            *(static_cast<value_type>(l) - static_cast<value_type>(k))*fac;
}
//@}

#endif // LEVI_CIVITA_SYMBOL_MEAT_H
