#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H

namespace detail {

template <typename _Expr, typename _Func>
class function_wrapper : public variable_base<function_wrapper<_Expr, _Func>>
{
    friend class variable_base<function_wrapper<_Expr, _Func>>;
public:
    using data_type = decltype (_Func::apply(typename _Expr::data_type()));

    explicit function_wrapper():
        _expr(_Expr())
    {}

    template<typename Data>
    constexpr inline auto const& operator()(Data const& x){
        reset_imp();
        update_imp(x);
        return _value;
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<_Func::get_string()<<"("<<_expr<<")";
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto update_imp(_Data const& x){
        static_cast<variable_base<_Expr>&>(_expr).update(x);
        if(!this->_update){
            _value = _Func::apply(_expr(x));
            this->_update = true;
        }
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_Expr>&>(_expr).reset();
        this->_update = false;
    }

    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const{
        return _value;
    }

    static data_type _value;
    _Expr _expr;
};

template <typename _Expr, typename _Func>
typename function_wrapper<_Expr, _Func>::data_type function_wrapper<_Expr, _Func>::_value;



template <typename _Expr, typename _Func>
class tensor_function_wrapper : public variable_base<tensor_function_wrapper<_Expr, _Func>>
{
    friend class variable_base<tensor_function_wrapper<_Expr, _Func>>;
public:
    using data_type = typename _Expr::data_type;

    explicit tensor_function_wrapper():
        _expr(_Expr())
    {}

    template<typename _Data>
    constexpr inline auto const& operator()(_Data const& __data){
        reset_imp();
        update_imp(__data);
        return get_value(__data);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<_Func::get_string()<<"("<<_expr<<")";
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;


private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const{
        return _value;
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data){
        static_cast<variable_base<_Expr>&>(_expr).update(__data);
        if(!this->_update){
            _value = _Func::apply(static_cast<variable_base<_Expr>&>(_expr).value(__data));
            this->_update = true;
        }
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_Expr>&>(_expr).reset();
        this->_update = false;
    }

    static data_type _value;
    _Expr _expr;
};

template <typename _Expr, typename _Func>
typename tensor_function_wrapper<_Expr, _Func>::data_type tensor_function_wrapper<_Expr, _Func>::_value;

}
#endif // FUNCTION_WRAPPER_H
