/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_AS_SYM_WRAPPER_BONES_H
#define TENSOR_AS_SYM_WRAPPER_BONES_H

namespace detail {

template <typename _Var>
class as_sym_wrapper: public variable_base<as_sym_wrapper<_Var>>
{
    friend class variable_base<as_sym_wrapper<_Var>>;
public:
    using data_type = typename _Var::data_type;

    as_sym_wrapper():
        _var(_Var())
    {}

    template<typename Data>
    constexpr inline auto const& operator()(Data const& __data){
        reset_imp();
        update_imp(__data);
        return get_value(__data);
    }

    constexpr inline std::ostream& print(std::ostream & __os)const{
        __os<<_var;
        return __os;
    }

    template<typename _Data>
    constexpr inline auto value(_Data) = delete;

    template<typename _Data>
    constexpr inline auto update(_Data) = delete;

    constexpr inline auto reset() = delete;

private:
    template<typename _Data>
    constexpr inline auto const& get_value(_Data const& __data)const{
        return static_cast<const variable_base<_Var>&>(_var).value(__data);
    }

    template<typename _Data>
    constexpr inline auto update_imp(_Data const& __data){
        static_cast<variable_base<_Var>&>(_var).update(__data);
    }

    constexpr inline auto reset_imp(){
        static_cast<variable_base<_Var>&>(_var).reset();
    }

    _Var _var;
};

}
#endif // TENSOR_AS_SYM_WRAPPER_BONES_H
