#ifndef COMPARE_LAMBDA_H
#define COMPARE_LAMBDA_H

namespace detail {

struct compare_lambda
{
    template <typename _LHS, typename _RHS, typename _Func>
    static inline bool check(_LHS const& __lhs, _RHS const& __rhs, _Func _func){
        constexpr bool LHS_raw_data{std::experimental::is_detected<has_raw_data, _LHS>::value};
        constexpr bool RHS_raw_data{std::experimental::is_detected<has_raw_data, _RHS>::value};
        constexpr bool LHS_evaluate{std::experimental::is_detected<detail::has_evaluate, _LHS>::value};
        constexpr bool RHS_evaluate{std::experimental::is_detected<detail::has_evaluate, _RHS>::value};

        if constexpr (LHS_raw_data && RHS_raw_data){
            if constexpr(LHS_evaluate){const_cast<_LHS&>(__lhs).evaluate();}
            if constexpr (RHS_evaluate){const_cast<_RHS&>(__rhs).evaluate();}
            return check_detail<_LHS::size()>(__lhs.raw_data(), __rhs.raw_data(), _func);
        }else if constexpr (!LHS_raw_data && RHS_raw_data){
            if constexpr (RHS_evaluate){const_cast<_RHS&>(__rhs).evaluate();}
            tensor<typename _LHS::value_type, _LHS::dimension(), _LHS::rank()> lhs{__lhs};
            return check_detail<decltype (lhs)::size()>(lhs.raw_data(), __rhs.raw_data(), _func);
        }else if constexpr (LHS_raw_data && !RHS_raw_data){
            if constexpr(LHS_evaluate){const_cast<_LHS&>(__lhs).evaluate();}
            tensor<typename _RHS::value_type, _RHS::dimension(), _RHS::rank()> rhs{__rhs};
            return check_detail<decltype (rhs)::size()>(__lhs.raw_data(), rhs.raw_data(), _func);
        }else{
            tensor<typename _RHS::value_type, _RHS::dimension(), _RHS::rank()> rhs{__rhs};
            tensor<typename _LHS::value_type, _LHS::dimension(), _LHS::rank()> lhs{__lhs};
            return check_detail<decltype (lhs)::size()>(lhs.raw_data(), rhs.raw_data(), _func);
        }
    }

private:
    template <std::size_t _Size, typename _LHS, typename _RHS, typename _Func>
    static constexpr inline bool check_detail(_LHS * __lhs, _RHS * __rhs, _Func _func){
        for(std::size_t i{0}; i<_Size; ++i){
            if(_func(__lhs[i],__rhs[i])){
                return false;
            }
        }
        return true;
    }
};


}

#endif // COMPARE_LAMBDA_H
