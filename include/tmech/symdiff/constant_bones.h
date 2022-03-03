#ifndef CONSTANT_BONES_H
#define CONSTANT_BONES_H

template <typename _T, std::size_t _ID>
class constant : public variable_base<constant<_T, _ID>>
{
    friend class variable_base<constant<_T, _ID>>;
public:
    using data_type = _T;

    constant();

    constant(data_type const& __data);

    constant(std::string const& __name);

    constant(data_type const& __data, std::string const& __name);

    ~constant();

    template<typename _Type>
    constexpr inline auto operator=(_Type const& __data);


    template<typename Data>
    constexpr inline auto const& operator()(Data const& __data) const;

    constexpr inline std::ostream& print(std::ostream & os) const;

    template<typename _Data>
    auto value(_Data) = delete;

    template<typename _Data>
    auto update(_Data) = delete;

    auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& x)const;

    static std::string _name;
    static data_type _data;
};

template <typename _T, std::size_t _ID>
std::string constant<_T, _ID>::_name{};

template <typename _T, std::size_t _ID>
_T constant<_T, _ID>::_data;


#endif // CONSTANT_BONES_H
