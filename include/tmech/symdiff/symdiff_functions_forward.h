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
#ifndef SYMDIFF_FUNCTIONS_FORWARD_H
#define SYMDIFF_FUNCTIONS_FORWARD_H

template<std::size_t N = 1, typename _Expression, typename _Variable, typename, typename>
constexpr inline auto derivative(_Expression, _Variable);

#endif // SYMDIFF_FUNCTIONS_FORWARD_H
