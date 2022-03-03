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
#ifndef SCALAR_MEAT_H
#define SCALAR_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template<typename _T>
constexpr scalar<_T>::scalar(_T const scalar):
    scalar_(scalar)
{}

/**
 * Copy constructor
 */
template<typename _T>
constexpr scalar<_T>::scalar(scalar const& scalar):
    scalar_(scalar.scalar_)
{}
//@}


template<typename _T>
constexpr inline _T scalar<_T>::operator()()const{return scalar_;}

template<typename _T>
template<typename ...Numbers>
constexpr inline _T scalar<_T>::operator()(Numbers...)const{return scalar_;}

} // NAMESPACE DETAIL
#endif // SCALAR_MEAT_H
