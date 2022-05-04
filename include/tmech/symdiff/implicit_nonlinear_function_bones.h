#ifndef IMPLICIT_NONLINEAR_FUNCTION_BONES_H
#define IMPLICIT_NONLINEAR_FUNCTION_BONES_H

namespace detail {

template <typename Function, typename Var1, typename Var2>
class implicit_nonlinear_function : public variable_base<implicit_nonlinear_function<Function, Var1, Var2>>
{
public:
    using data_type = typename Function::data_type;

    constexpr implicit_nonlinear_function():
        _data()
    {}

    template<typename Data>
    constexpr inline auto operator()(Data const& x) const{
        return _data(x);
    }

    constexpr inline std::ostream& print(std::ostream & __os) const{
        __os << _data;
        return __os;
    }

    template<typename _Data>
    constexpr inline  auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline  auto update(_Data) = delete;

    constexpr inline  auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto get_value(_Data const& __data)const{
        return static_cast<const variable_base<Function>&>(_data).value(__data);
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data){
        //static_cast<variable_base<Function>&>(_data).update(__data);
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<Function>&>(_data).reset();
    }

    Function _data;
};

}


#endif // IMPLICIT_NONLINEAR_FUNCTION_BONES_H
