#ifndef TENSOR_OUTER_PRODUCT_WRAPPER_BONES_H
#define TENSOR_OUTER_PRODUCT_WRAPPER_BONES_H

namespace detail {

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class tensor_outer_product_wrapper :
        public variable_base<tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>
{
    friend class variable_base<tensor_outer_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>;
public:
    using tensor_info_lhs = get_tensor_info<typename _ExprLHS::data_type>;
    using tensor_info_rhs = get_tensor_info<typename _ExprRHS::data_type>;
    using value_type = typename tmech::detail::result_type<typename tensor_info_lhs::value_type, typename tensor_info_lhs::value_type>::value_type;
    using data_type = typename get_tensor<value_type, tensor_info_lhs::dimension(), tensor_info_lhs::rank() + tensor_info_rhs::rank()>::type;

    static_assert (detail::is_tensor<typename _ExprLHS::data_type>::value,
    "tensor_outer_product_wrapper: lhs expression data type is not a tensor");

    static_assert (detail::is_tensor<typename _ExprRHS::data_type>::value,
    "tensor_outer_product_wrapper: rhs expression data type is not a tensor");

    tensor_outer_product_wrapper() {}

    template<typename Data>
    constexpr inline auto operator()(Data const& x){
        reset_imp();
        update_imp(x);
        return get_value(x);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        if constexpr (std::is_same_v<_SeqLHS, tmech::sequence<1,2>> && std::is_same_v<_SeqRHS, tmech::sequence<3,4>>){
            print_otimes(os);
        }else if constexpr (std::is_same_v<_SeqLHS, tmech::sequence<1,3>> && std::is_same_v<_SeqRHS, tmech::sequence<2,4>>){
            print_otimesu(os);
        }else if constexpr (std::is_same_v<_SeqLHS, tmech::sequence<1,4>> && std::is_same_v<_SeqRHS, tmech::sequence<2,3>>){
            print_otimesl(os);
        }else{
            print_general(os);
        }
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const{
        return tmech::outer_product<_SeqLHS, _SeqRHS>(static_cast<const variable_base<_ExprLHS>&>(_lhs).value(__data),
                                                      static_cast<const variable_base<_ExprRHS>&>(_rhs).value(__data));
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data){
        static_cast<variable_base<_ExprLHS>&>(_lhs).update(__data);
        static_cast<variable_base<_ExprRHS>&>(_rhs).update(__data);
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_ExprLHS>&>(_lhs).reset();
        static_cast<variable_base<_ExprRHS>&>(_rhs).reset();
    }

    constexpr inline auto print_general(std::ostream & os)const{
        os<<"outer<<";
        print_sequence(os, _SeqLHS());
        os<<">,<";
        print_sequence(os, _SeqRHS());
        os<<">>(";
        _lhs.print(os);
        os<<", ";
        _rhs.print(os);
        os<<")";
    }

    constexpr inline auto print_otimes(std::ostream & os)const{
        os<<"otimes(";
        _lhs.print(os);
        os<<", ";
        _rhs.print(os);
        os<<")";
    }

    constexpr inline auto print_otimesu(std::ostream & os)const{
        os<<"otimesu(";
        _lhs.print(os);
        os<<", ";
        _rhs.print(os);
        os<<")";
    }

    constexpr inline auto print_otimesl(std::ostream & os)const{
        os<<"otimesl(";
        _lhs.print(os);
        os<<", ";
        _rhs.print(os);
        os<<")";
    }

    template<std::size_t First, std::size_t ...Numbers>
    static constexpr auto print_sequence(std::ostream & os, tmech::sequence<First, Numbers...>){
        os<<First<<",";
        print_sequence(os, tmech::sequence<Numbers...>());
    }

    template<std::size_t Last>
    static constexpr auto print_sequence(std::ostream & os, tmech::sequence<Last>){
        os<<Last;
    }

    _ExprLHS _lhs;
    _ExprRHS _rhs;
};

}
#endif // TENSOR_OUTER_PRODUCT_WRAPPER_BONES_H
