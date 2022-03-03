#ifndef TENSOR_INNER_PRODUCT_WRAPPER_BONES_H
#define TENSOR_INNER_PRODUCT_WRAPPER_BONES_H

namespace detail {

template <typename _T, std::size_t _Dim, std::size_t _Rank>
struct return_data_type_inner_product
{
    using type = typename get_tensor<_T, _Dim, _Rank>::type;
};

template <typename _T, std::size_t _Dim>
struct return_data_type_inner_product<_T, _Dim, 0>
{
    using type = double;
};

template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
class tensor_inner_product_wrapper
        : public variable_base<tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>
{
    friend class variable_base<tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>>;

public:
    using data_type_lhs = typename _ExprLHS::data_type;
    using data_type_rhs = typename _ExprRHS::data_type;

    using tensor_info_lhs = get_tensor_info<typename _ExprLHS::data_type>;
    using tensor_info_rhs = get_tensor_info<typename _ExprRHS::data_type>;
    using value_type = typename tmech::detail::result_type<typename tensor_info_lhs::value_type, typename tensor_info_lhs::value_type>::value_type;
    using data_type = typename return_data_type_inner_product<value_type, tensor_info_lhs::dimension(), tensor_info_lhs::rank() + tensor_info_rhs::rank() - _SeqLHS::size() - _SeqRHS::size()>::type;

    static_assert (detail::is_tensor<typename _ExprLHS::data_type>::value,
    "tensor_inner_product_wrapper: lhs expression data type is not a tensor");

    static_assert (detail::is_tensor<typename _ExprRHS::data_type>::value,
    "tensor_inner_product_wrapper: rhs expression data type is not a tensor");


    tensor_inner_product_wrapper() {}

    template<typename Data>
    constexpr inline auto const& operator()(Data const& x){
        reset_imp();
        update_imp(x);
        return get_value(x);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        print_general(os);
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& x)const{
        return _data;
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& x){
        static_cast<variable_base<_ExprLHS>&>(_lhs).update(x);
        static_cast<variable_base<_ExprRHS>&>(_rhs).update(x);
        if(!this->_update){
            _data = tmech::inner_product<_SeqLHS, _SeqRHS>(static_cast<const variable_base<_ExprLHS>&>(_lhs).value(x),
                                                           static_cast<const variable_base<_ExprRHS>&>(_rhs).value(x));
            this->_update = true;
        }
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_ExprLHS>&>(_lhs).reset();
        static_cast<variable_base<_ExprRHS>&>(_rhs).reset();
        this->_update = false;
    }

    constexpr inline auto print_general(std::ostream & os)const{
        os<<"inner_product<<";
        print_sequence(os, _SeqLHS());
        os<<">,<";
        print_sequence(os, _SeqRHS());
        os<<">>(";
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

    static data_type _data;
    _ExprLHS _lhs;
    _ExprRHS _rhs;
};


template <typename _ExprLHS, typename _ExprRHS, typename _SeqLHS, typename _SeqRHS>
typename tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::data_type tensor_inner_product_wrapper<_ExprLHS, _ExprRHS, _SeqLHS, _SeqRHS>::_data;
}

#endif // TENSOR_INNER_PRODUCT_WRAPPER_BONES_H
