#ifndef TENSOR_BASIS_CHANGE_WRAPPER_BONES_H
#define TENSOR_BASIS_CHANGE_WRAPPER_BONES_H

namespace detail {

template <typename _Expr, typename _Sequence>
class tensor_basis_change_wrapper :
        public variable_base<tensor_basis_change_wrapper<_Expr, _Sequence>>
{
    friend class variable_base<tensor_basis_change_wrapper<_Expr, _Sequence>>;
public:
    using data_type       = typename _Expr::data_type;
    using tensor_info_lhs = get_tensor_info<typename _Expr::data_type>;

    static_assert (detail::is_tensor<typename _Expr::data_type>::value,
    "tensor_basis_change_wrapper: expression data_type is not a tensor");

    constexpr tensor_basis_change_wrapper() {}

    template<typename Data>
    constexpr inline auto operator()(Data const& x){
        reset_imp();
        update_imp(x);
        return get_value(x);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        if constexpr (std::is_same_v<_Sequence, tmech::sequence<2,1>> && tensor_info_lhs::rank() == 2){
            print_trans(os);
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
    constexpr inline auto get_value(_Data const& x)const{
        return tmech::basis_change<_Sequence>(static_cast<const variable_base<_Expr>&>(_expr).value(x));
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& x){
        static_cast<variable_base<_Expr>&>(_expr).update(x);
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_Expr>&>(_expr).reset();
    }

    constexpr inline auto print_trans(std::ostream & os)const{
        os<<"trans(";
        _expr.print(os);
        os<<")";
    }

    constexpr inline auto print_general(std::ostream & os)const{
        os<<"basis_change<";
        print_sequence(os, _Sequence());
        os<<">(";
        _expr.print(os);
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

    _Expr _expr;
};

}

#endif // TENSOR_BASIS_CHANGE_WRAPPER_BONES_H
