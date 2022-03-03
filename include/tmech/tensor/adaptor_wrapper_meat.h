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
#ifndef ADAPTOR_WRAPPER_MEAT_H
#define ADAPTOR_WRAPPER_MEAT_H


/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr adaptor<_T, _Dim, _Rank, _Type>::adaptor(_T * __data):
    _data(__data)
{}

/**
 * Copy constructor
 */
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr adaptor<_T, _Dim, _Rank, _Type>::adaptor(adaptor const& __data):
    _data(__data._data)
{}
//@}

/**
* @name Extended copy semantic
*/
//@{
/**
* The extended assignment operator.
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
template<typename _Derived>
constexpr inline auto const& adaptor<_T, _Dim, _Rank, _Type>::operator=(tensor_base<_Derived> const& tensor_base){
    static_assert(_Derived::rank() == _Rank,     "No matching rank in assignment operator");
    static_assert(_Derived::dimension() == _Dim, "No matching dimensions in assignment operator.");

    auto& tensor{tensor_base.convert()};
    detail::evaluate::apply(tensor);
    _Type::assign_tensor(_data, tensor);
    return *this;
}
//@}


/**
* @name Data
*/
/**
* @brief Returns the element at the specified position in the expression.
* @param indicies A list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
template<typename ...Indices>
constexpr inline auto adaptor<_T, _Dim, _Rank, _Type>::operator()(Indices ...indices)const{
    return _Type::apply(_data, indices...);
}
//@}


/**
* @name Dimension and rank
*/
/**
* Returns the dimension of the expression.
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr inline auto adaptor<_T, _Dim, _Rank, _Type>::dimension(){
    return _Dim;
}

/**
 * Returns the rank of the expression.
 */
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr inline auto adaptor<_T, _Dim, _Rank, _Type>::rank(){
    return _Rank;
}
//@}


#endif // ADAPTOR_WRAPPER_MEAT_H
