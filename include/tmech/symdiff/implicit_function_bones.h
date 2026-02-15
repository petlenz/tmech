#ifndef IMPLICIT_FUNCTION_BONES_H
#define IMPLICIT_FUNCTION_BONES_H

namespace detail {

template <typename _Functions, typename _Variables>
class implicit_function : public variable_base<implicit_function<_Functions, _Variables>>
{
public:
    //using data_type = typename std::decay<decltype(std::get<0>(_Functions()))>::type::data_type;
    using function_type = typename std::decay<decltype(std::get<0>(_Functions()))>::type;
    using data_type = typename function_type::data_type;
    static constexpr auto number_of_functions = std::tuple_size_v<_Functions>;
    static constexpr auto number_of_variables = std::tuple_size_v<_Variables>;

    constexpr implicit_function():
        _data()
    {}

    template<typename Data>
    constexpr inline auto operator()(Data const& x){
        return _data(x);
    }

    constexpr inline std::ostream& print(std::ostream & __os) const{
        __os << std::get<0>(_data);
        return  __os;
    }

    template<typename _Data>
    constexpr inline  auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline  auto update(_Data) = delete;

    constexpr inline  auto reset() = delete;

private:
  template <typename _Data>
  constexpr inline auto get_value([[maybe_unused]] _Data const &__data) const {
    return 0; // static_cast<const
              // variable_base<Function>&>(_data).value(__data);
  }

  template <typename _Data>
  constexpr inline auto update_imp([[maybe_unused]] _Data const &__data) {
    if (!this->_update) {
      // static_cast<variable_base<Function>&>(_data).update(__data);
    }
  }

    constexpr inline auto reset_imp(){
        //static_cast<variable_base<Function>&>(_data).reset();
    }

    _Functions _data;
};

}

#endif // IMPLICIT_FUNCTION_BONES_H
