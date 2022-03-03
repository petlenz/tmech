#ifndef TENSOR_INV_H
#define TENSOR_INV_H

namespace detail {

template <typename _Expr>
class inv_tensor : public variable_base<inv_tensor<_Expr>>
{
    friend class variable_base<inv_tensor<_Expr>>;
public:
    using data_type = typename _Expr::data_type;

    inv_tensor():
        _expr(_Expr())
    {}

    template<typename Data>
    constexpr inline auto const& operator()(Data const& x){
        reset_imp();
        update_imp(x);
        return get_value(x);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<"inv("<<_expr<<")";
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
        static_cast<variable_base<_Expr>&>(_expr).update(x);
        if(!this->_update){
            _data = tmech::inv(static_cast<const variable_base<_Expr>&>(_expr).value(x));
            this->_update = true;
        }
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_Expr>&>(_expr).reset();
        this->_update = false;
    }

    static data_type _data;
    _Expr _expr;
};

template <typename _Expr>
typename inv_tensor<_Expr>::data_type inv_tensor<_Expr>::_data;

}


#endif // TENSOR_INV_H
