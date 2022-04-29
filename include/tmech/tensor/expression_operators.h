/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
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
    static constexpr inline auto apply(T1 && lhs, T2 && rhs)noexcept{
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
    static constexpr inline auto apply(T1 && lhs, T2 && rhs)noexcept{
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
    static constexpr inline auto apply(T1 && lhs, T2 && rhs)noexcept{
        return std::forward<T1>(lhs) * std::forward<T2>(rhs);
    }
};

struct operator_div{
//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 const& rhs)noexcept{
//        return lhs / rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 && lhs, T2 const& rhs)noexcept{
//        return std::forward<T1>(lhs) / rhs;
//    }

//    template<typename T1, typename T2>
//    static constexpr inline auto apply(T1 const& lhs, T2 && rhs)noexcept{
//        return lhs / std::forward<T2>(rhs);
//    }

    template<typename T1, typename T2>
    static constexpr inline auto apply(T1 && lhs, T2 && rhs)noexcept{
        return std::forward<T1>(lhs) / std::forward<T2>(rhs);
    }
};

} // NAMESPACE DETAIL

#endif // EXPRESSION_OPERATORS_H
