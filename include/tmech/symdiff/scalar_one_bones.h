#ifndef SCALAR_ONE_BONES_H
#define SCALAR_ONE_BONES_H


template<typename T>
class scalar_one : public variable_base<scalar_one<T>>
{
    friend class variable_base<scalar_one<T>>;
public:
    using data_type = T;

    static_assert (detail::is_scalar<T>::value, "scalar_one: no matching data type");

    scalar_one(){}

    template<typename _Data>
    constexpr inline data_type operator()(_Data const& __data) const{
        return get_value(__data);
    }

    inline std::string get_string()const{
        return std::string("1");
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<1;
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;


private:
    template<typename _Data>
    constexpr inline data_type get_value(_Data const& /*__data*/)const{
        return 1;
    }
};

#endif // SCALAR_ONE_BONES_H
