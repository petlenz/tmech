#ifndef NEGATIVE_MEAT_H
#define NEGATIVE_MEAT_H

namespace detail {

template<typename _Expr>
negative<_Expr>::negative(){}

template<typename _Expr>
template<typename _Data>
constexpr inline auto negative<_Expr>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template<typename _Expr>
constexpr inline std::ostream& negative<_Expr>::print(std::ostream & __os)const{
    __os<<"-"<<_Expr();
    return __os;
}

template<typename _Expr>
template<typename _Data>
constexpr inline auto negative<_Expr>::get_value(_Data const& __data){
    return -static_cast<variable_base<_Expr>&>(_expr).value(__data);
}

template<typename _Expr>
template<typename _Data>
constexpr inline auto negative<_Expr>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
}

template<typename _Expr>
constexpr inline auto negative<_Expr>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
    this->_update = false;
}

}

#endif // NEGATIVE_MEAT_H
