/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ADAPTOR_WRAPPER_MEAT_H
#define ADAPTOR_WRAPPER_MEAT_H


/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr adaptor<_T, _Dim, _Rank, _Type>::adaptor(_T * __data)noexcept:
    _data(__data)
{}

/**
 * Copy constructor
 */
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr adaptor<_T, _Dim, _Rank, _Type>::adaptor(adaptor const& __data)noexcept:
    basetype(__data),
    _data(__data._data)
{}
//@}

/**
* @name Extended copy semantic
*/
//@{
/**
* The extended assignment operator.
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
template<typename _Derived>
constexpr inline auto const& adaptor<_T, _Dim, _Rank, _Type>::operator=(tensor_base<_Derived> const& tensor_base)noexcept{
    static_assert(_Derived::rank() == _Rank,     "No matching rank in assignment operator");
    static_assert(_Derived::dimension() == _Dim, "No matching dimensions in assignment operator.");

    auto& tensor{tensor_base.convert()};
    constexpr std::size_t Size{[] { std::size_t n{1}; for (std::size_t r{0}; r < _Rank; ++r) n *= _Dim; return n; }()};
    if constexpr (_Type::use_raw_data()) { // nested: the checks below need raw_data()
        if constexpr (::tmech_detail::is_detected<detail::has_can_write_to, _Derived>::value
                      && ::tmech_detail::is_detected<detail::has_evaluate, _Derived, adaptor&>::value) {
            if (tensor.can_write_to(_data, _data + Size)) { // straight into the target, as tensor::operator=
                tensor.evaluate(*this);
                return *this;
            }
        }
    }
    detail::evaluate::apply(tensor); // into the expression's own storage: safe if the target is an operand
    if constexpr (_Type::use_raw_data() && detail::is_direct_assignable_v<_Derived>) {
#ifdef TMECH_HAS_XSIMD
        if constexpr (detail::is_simd_assignable_v<_Derived, value_type>) {
            using batch_type = xsimd::batch<value_type>;
            constexpr std::size_t W = batch_type::size;
            constexpr std::size_t vec_end = (Size / W) * W;
            for (std::size_t i = 0; i < vec_end; i += W)
                tensor.batch_access(i).store_unaligned(_data + i);
            for (std::size_t i = vec_end; i < Size; ++i)
                _data[i] = tensor.direct_access(i);
        } else
#endif
        {
            for (std::size_t i = 0; i < Size; ++i)
                _data[i] = tensor.direct_access(i);
        }
    } else {
        _Type::assign_tensor(_data, tensor);
    }
    return *this;
}

template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr inline auto const& adaptor<_T, _Dim, _Rank, _Type>::operator=(adaptor const& __other)noexcept{
    return this->operator=(static_cast<tensor_base<adaptor> const&>(__other));
}
//@}


/**
* @name Data
*/
/**
* @brief Returns the element at the specified position in the expression.
* @param indicies A list of indices specifying the position in the tensor expression. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
template<typename ...Indices>
constexpr inline auto adaptor<_T, _Dim, _Rank, _Type>::operator()(Indices ...indices)const noexcept{
    return _Type::apply(_data, indices...);
}
//@}


/**
* @name Dimension and rank
*/
/**
* Returns the dimension of the expression.
*/
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr inline auto adaptor<_T, _Dim, _Rank, _Type>::dimension()noexcept{
    return _Dim;
}

/**
 * Returns the rank of the expression.
 */
template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
constexpr inline auto adaptor<_T, _Dim, _Rank, _Type>::rank()noexcept{
    return _Rank;
}
//@}


#endif // ADAPTOR_WRAPPER_MEAT_H
