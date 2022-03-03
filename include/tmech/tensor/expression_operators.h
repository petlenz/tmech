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
#ifndef EXPRESSION_OPERATORS_H
#define EXPRESSION_OPERATORS_H

namespace detail {

struct operator_add{
//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 const& rhs){
//        return lhs + rhs;
//    }
//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 && lhs, T2 const& rhs){
//        return std::forward<T1>(lhs) + rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 && rhs){
//        return lhs + std::forward<T2>(rhs);
//    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 && lhs, T2 && rhs){
        return std::forward<T1>(lhs) + std::forward<T2>(rhs);
    }
};

struct operator_sub{
//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 const& rhs){
//        return lhs - rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 && lhs, T2 const& rhs){
//        return std::forward<T1>(lhs) - rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 && rhs){
//        return lhs - std::forward<T2>(rhs);
//    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 && lhs, T2 && rhs){
        return std::forward<T1>(lhs) - std::forward<T2>(rhs);
    }
};

struct operator_mul{
//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 const& rhs){
//        return lhs * rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 && lhs, T2 const& rhs){
//        return std::forward<T1>(lhs) * rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 && rhs){
//        return lhs * std::forward<T2>(rhs);
//    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 && lhs, T2 && rhs){
        return std::forward<T1>(lhs) * std::forward<T2>(rhs);
    }
};

struct operator_div{
    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 const& lhs, T2 const& rhs){
        return lhs / rhs;
    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 && lhs, T2 const& rhs){
        return std::forward<T1>(lhs) / rhs;
    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 const& lhs, T2 && rhs){
        return lhs / std::forward<T2>(rhs);
    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 && lhs, T2 && rhs){
        return std::forward<T1>(lhs) / std::forward<T2>(rhs);
    }
};

} // NAMESPACE DETAIL

#endif // EXPRESSION_OPERATORS_H
