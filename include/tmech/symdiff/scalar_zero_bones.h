#ifndef SCALAR_ZERO_BONES_H
#define SCALAR_ZERO_BONES_H

template<typename T>
class scalar_zero : public variable_base<scalar_zero<T>>
{
public:
    using data_type = T;

    template<typename Data>
    constexpr inline data_type operator()(Data const& __data) const{
        return get_value(__data);
    }

    inline std::string get_string()const{
        return std::string("0");
    }

    constexpr inline std::ostream& print(std::ostream & os)const{
        os<<0;
        return os;
    }

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline data_type get_value(_Data const& __data)const{
        return 0;
    }
};

#endif // SCALAR_ZERO_BONES_H
