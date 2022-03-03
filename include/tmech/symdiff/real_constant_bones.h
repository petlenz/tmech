#ifndef REAL_CONSTANT_BONES_H
#define REAL_CONSTANT_BONES_H

template<typename _T, long long int _L, long long int _R, long long int _Exp>
class real : public variable_base<real<_T, _L, _R, _Exp>>
{
    friend class variable_base<real<_T, _L, _R, _Exp>>;
public:
    using data_type = _T;

    real(){
        std::stringstream strout;
        strout << "0." << _L << _R << "e" << _Exp;
        strout >> _value;
    }

    real(real const& data):
        _value(data._value)
    {}

    template<typename _Data>
    constexpr inline auto& operator()(_Data const& __data) const{
        return get_value(__data);
    }

    inline std::string get_string()const{
        return std::to_string(_value);
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<_value;
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& /*__data*/)const{
        return _value;
    }

    data_type _value;
};

#endif // REAL_CONSTANT_BONES_H
