#ifndef NEGATIVE_BONES_H
#define NEGATIVE_BONES_H

namespace detail {

template<typename _Expr>
class negative : public variable_base<negative<_Expr>>
{
public:
    using data_type = typename _Expr::data_type;

    negative(){}

    template<typename _Data>
    constexpr inline auto operator()(_Data const& x){
        reset_imp();
        update_imp(x);
        return get_value(x);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<"-"<<_Expr();
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:

    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data){
        return -static_cast<variable_base<_Expr>&>(_expr).value(__data);
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data){
        static_cast<variable_base<_Expr>&>(_expr).update(__data);
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_Expr>&>(_expr).reset();
        this->_update = false;
    }

    _Expr _expr;
};

}
#endif // NEGATIVE_BONES_H
