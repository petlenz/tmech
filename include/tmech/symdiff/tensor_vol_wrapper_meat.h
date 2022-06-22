#ifndef TENSOR_VOL_WRAPPER_MEAT_H
#define TENSOR_VOL_WRAPPER_MEAT_H


namespace detail {

template <typename _Expr>
tensor_vol_wrapper<_Expr>::tensor_vol_wrapper():
    _expr(_Expr())
{}

template <typename _Expr>
template<typename _Data>
constexpr inline auto const& tensor_vol_wrapper<_Expr>::operator()(_Data const& __data){
    reset_imp();
    update_imp(__data);
    return get_value(__data);
}

template <typename _Expr>
constexpr inline std::ostream& tensor_vol_wrapper<_Expr>::print(std::ostream & __os)const{
    __os<<"vol("<<_expr<<")";
    return __os;
}

template <typename _Expr>
template<typename _Data>
constexpr inline auto const& tensor_vol_wrapper<_Expr>::get_value(_Data const& /*__data*/)const{
    return _data;
}

template <typename _Expr>
template<typename _Data>
constexpr inline auto tensor_vol_wrapper<_Expr>::update_imp(_Data const& __data){
    static_cast<variable_base<_Expr>&>(_expr).update(__data);
    if(!this->_update){
        _data = tmech::vol(static_cast<const variable_base<_Expr>&>(_expr).value(__data));
        this->_update = true;
    }
}

template <typename _Expr>
constexpr inline auto tensor_vol_wrapper<_Expr>::reset_imp(){
    static_cast<variable_base<_Expr>&>(_expr).reset();
    this->_update = false;
}

}


#endif // TENSOR_VOL_WRAPPER_MEAT_H
