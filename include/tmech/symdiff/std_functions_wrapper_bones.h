#ifndef STD_FUNCTIONS_WRAPPER_BONES_H
#define STD_FUNCTIONS_WRAPPER_BONES_H

namespace detail {

struct exp_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::exp(value);
    }

    static inline std::string get_string(){
        return std::string("exp");
    }
};

struct log_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::log(value);
    }
    static inline std::string get_string(){
        return std::string("log");
    }
};

struct log2_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::log2(value);
    }
    static inline std::string get_string(){
        return std::string("log2");
    }
};

struct log10_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::log10(value);
    }
    static inline std::string get_string(){
        return std::string("log10");
    }
};

struct sqrt_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::sqrt(value);
    }
    static inline std::string get_string(){
        return std::string("sqrt");
    }
};

struct abs_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::abs(value);
    }
    static inline std::string get_string(){
        return std::string("abs");
    }
};

struct cos_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::cos(value);
    }
    static inline std::string get_string(){
        return std::string("cos");
    }
};

struct sin_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::sin(value);
    }
    static inline std::string get_string(){
        return std::string("sin");
    }
};

struct tan_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::tan(value);
    }
    static inline std::string get_string(){
        return std::string("tan");
    }
};


struct asin_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::asin(value);
    }
    static inline std::string get_string(){
        return std::string("asin");
    }
};

struct acos_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return std::acos(value);
    }
    static inline std::string get_string(){
        return std::string("acos");
    }
};

struct sec_wrapper{
    template<typename T>
    constexpr static inline auto apply(T const& value){
        return static_cast<T>(1.0)/std::cos(value);
    }
    static inline std::string get_string(){
        return std::string("sec");
    }
};

struct sign_wrapper{
    template<typename T>
    constexpr static inline T apply(T const& value){
        return (value < 0 ? -1 : 1);
    }
    static inline std::string get_string(){
        return std::string("sign");
    }
};

template <typename Base, typename Exp, bool IsFundamental = true>
class diff_pow : public variable_base<diff_pow<Base, Exp, IsFundamental>>
{
    friend class variable_base<diff_pow<Base, Exp, IsFundamental>>;
public:
    using data_type = typename Base::data_type;

    explicit diff_pow():
        _base(Base()),
        _exp(Exp())
    {}

    template<typename Data>
    constexpr inline auto operator()(Data const& __data){
        reset_imp();
        update_imp(__data);
        return _value;
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<"pow(";
        os<<_base;
        os<<", ";
        os<<_exp;
        os<<")";
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
        static_cast<variable_base<Base>&>(_base).update(x);
        static_cast<variable_base<Exp>&>(_exp).update(x);
        if(!this->_update){
            _value = std::pow(_base(x),_exp(x));
            this->_update = true;
        }
    }

    constexpr inline auto reset_imp(){
        this->_update = false;
        static_cast<variable_base<Base>&>(_base).reset();
        static_cast<variable_base<Exp>&>(_exp).reset();
    }

    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const{
        return _value;
    }

    static data_type _value;
    Base _base;
    Exp _exp;
};

template <typename Base, typename Exp, bool IsFundamental>
typename diff_pow<Base, Exp, IsFundamental>::data_type diff_pow<Base, Exp, IsFundamental>::_value;



}
#endif // STD_FUNCTIONS_WRAPPER_BONES_H
