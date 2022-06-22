#ifndef TENSOR_FOURTH_ORDER_IDENTITY_WRAPPER_MEAT_H
#define TENSOR_FOURTH_ORDER_IDENTITY_WRAPPER_MEAT_H

//#include "../tensor/tensor_operators.h"

namespace symdiff {
namespace detail {

template <typename _T>
tensor_sym_identity_wrapper<_T>::tensor_sym_identity_wrapper(){}

template <typename _T>
template<typename _Data>
constexpr inline auto tensor_sym_identity_wrapper<_T>::operator()(_Data const& __data){
    return get_value(__data);
}

template <typename _T>
constexpr inline std::ostream& tensor_sym_identity_wrapper<_T>::print(std::ostream & __os)const{
    __os<<"IIsym";
    return __os;
}

template <typename _T>
template<typename _Data>
constexpr inline auto tensor_sym_identity_wrapper<_T>::get_value(_Data const& /*__data*/)const{
    using I = tmech::eye<typename tensor_info::value_type, tensor_info::dimension(), 2>;
    using otimesu = decltype (tmech::otimesu(I(),I()));
    using otimesl = decltype (tmech::otimesl(I(),I()));
    using sym = decltype (::operator+<otimesu, otimesl>(tmech::otimesu(I(),I()), tmech::otimesl(I(),I())));
    return ::operator*<double, sym>(0.5, ::operator+<otimesu, otimesl>(otimesu(I(),I()), otimesl(I(),I())));
}



template <typename _T>
tensor_dev_identity_wrapper<_T>::tensor_dev_identity_wrapper(){}

template <typename _T>
template<typename _Data>
constexpr inline decltype(auto) tensor_dev_identity_wrapper<_T>::operator()(_Data const& __data){
    return get_value(__data);
}

template <typename _T>
constexpr inline std::ostream& tensor_dev_identity_wrapper<_T>::print(std::ostream & __os)const{
    __os<<"IIdev";
    return __os;
}

template <typename _T>
template<typename _Data>
constexpr inline decltype(auto) tensor_dev_identity_wrapper<_T>::get_value(_Data const& /*__data*/)const{
    using I = tmech::eye<typename tensor_info::value_type, tensor_info::dimension(), 2>;
    using otimesu = decltype (tmech::otimesu(I(),I()));
    using otimesl = decltype (tmech::otimesl(I(),I()));
    using otimes  = decltype (tmech::otimes(I(),I()));
    using sym = decltype (::operator+<otimesu, otimesl>(tmech::otimesu(I(),I()), tmech::otimesl(I(),I())));

    //template function inside a template function.... ugly af
    return ::operator-
            <
            decltype (::operator*<double, sym>(0.5, ::operator+<otimesu, otimesl>(otimesu(I(),I()), otimesl(I(),I())))),
            decltype (::operator*<double, otimes>((1.0/tensor_info::dimension()), otimes(I(),I())))
            >
            (::operator*<double, sym>(0.5, ::operator+<otimesu, otimesl>(otimesu(I(),I()), otimesl(I(),I()))),
             ::operator*<double, otimes>((1.0/tensor_info::dimension()), otimes(I(),I())));
}



template <typename _T>
tensor_vol_identity_wrapper<_T>::tensor_vol_identity_wrapper(){}

template <typename _T>
template<typename _Data>
constexpr inline auto tensor_vol_identity_wrapper<_T>::operator()(_Data const& __data){
    return get_value(__data);
}

template <typename _T>
constexpr inline std::ostream& tensor_vol_identity_wrapper<_T>::print(std::ostream & __os)const{
    __os<<"IIvol";
    return __os;
}

template <typename _T>
template<typename _Data>
constexpr inline auto tensor_vol_identity_wrapper<_T>::get_value(_Data const& /*__data*/)const{
    using I = tmech::eye<typename tensor_info::value_type, tensor_info::dimension(), 2>;
    using otimes  = decltype (tmech::otimes(I(),I()));
    return ::operator*<double, otimes>((1.0/tensor_info::dimension()), otimes(I(),I()));
}
}
}
#endif // TENSOR_FOURTH_ORDER_IDENTITY_WRAPPER_MEAT_H
