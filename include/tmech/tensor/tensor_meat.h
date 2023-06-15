/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_MEAT_H
#define TENSOR_MEAT_H

/**
 * @name In-place arithmetic operators
*/
//@{
/**
 * In-place tensor subtraction.
 * @param rhs Right hand side tensor expression of subtraction.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
template <typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value>*>
constexpr inline auto const& tensor<T, Dim, Rank>::operator+=(_Tensor && __rhs) noexcept{
    *this = *this + std::forward<_Tensor>(__rhs);
    return *this;
}

/**
 * In-place tensor addition.
 * @param rhs Right hand side tensor expression of addtion.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
template <typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value>*>
constexpr inline auto const& tensor<T, Dim, Rank>::operator-=(_Tensor && __rhs) noexcept{
    *this = *this - std::forward<_Tensor>(__rhs);
    return *this;
}

/**
 * In-place tensor single contraction.
 * @param rhs Right hand side tensor expression of single contraction.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
template <typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value>*>
constexpr inline auto const& tensor<T, Dim, Rank>::operator*=(_Tensor && __rhs) noexcept{
    *this = *this * std::forward<_Tensor>(__rhs);
    return *this;
}

/**
 * In-place tensor element-wise scalar multiplication.
 * @param value Value of element-wise scalar multiplication.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto const& tensor<T, Dim, Rank>::operator*=(value_type const value) noexcept{
    *this = *this * value;
    return *this;
}

/**
 * In-place tensor element-wise scalar division.
 * @param value Value of element-wise scalar division.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto const& tensor<T, Dim, Rank>::operator/=(value_type const value) noexcept{
    *this = *this / value;
    return *this;
}
//@}

template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto const& tensor<T, Dim, Rank>::operator=(tensor const& tensor) noexcept {
    if(&tensor == this)
        return *this;
    _data = tensor._data;
    return *this;
}

template<typename T, std::size_t Dim, std::size_t Rank>
template<typename Derived>
constexpr inline auto const& tensor<T, Dim, Rank>::operator=(tensor_base<Derived> const& tensor_base) noexcept{
    static_assert(Derived::rank() == Rank,     "tensor::operator=(): non matching rank");
    static_assert(Derived::dimension() == Dim, "tensor::operator=(): no matching dimensions");

    //const auto& tensor{tensor_base.convert()};

    check_size();
    if constexpr(std::experimental::is_detected<detail::has_evaluate, Derived, decltype (*this)>::value){
        tensor_base.convert().evaluate(*this);
    }else{
        if constexpr(std::experimental::is_detected<detail::has_evaluate, Derived>::value){
            tensor_base.convert().evaluate();
        }
        using meta_loop = detail::for_loop_t<rank()-1, Dim>;
        auto func{[&](auto ...numbers){(*this)(numbers...) = tensor_base.convert()(numbers...);}};
        meta_loop::for_loop(func);
    }
    return *this;
}



template<typename T, std::size_t Dim, std::size_t Rank>
template<typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value>*>
constexpr inline auto const&
tensor<T, Dim, Rank>::operator=(_Tensor && __tensor) noexcept{
    using TensorType = typename std::decay<_Tensor>::type;
    static_assert(TensorType::rank() == Rank,     "tensor::operator=(): non matching rank");
    static_assert(TensorType::dimension() == Dim, "tensor::operator=(): no matching dimensions");

    TensorType _tensor{std::forward<_Tensor>(__tensor)};

    check_size();
    if constexpr(std::experimental::is_detected<detail::has_evaluate, TensorType, decltype (*this)>::value){
        _tensor.evaluate(*this);
    }else{
        if constexpr(std::experimental::is_detected<detail::has_evaluate, TensorType>::value){
            _tensor.evaluate();
        }
        using meta_loop = detail::for_loop_t<rank()-1, Dim>;
        auto func{[&](auto ...numbers){(*this)(numbers...) = _tensor(numbers...);}};
        meta_loop::for_loop(func);
    }
    return *this;
}

/**
* @name Data
*/
//@{
/**
* Returns a constant reference of the element at the specified position in the tensor.
* @param indicies a list of indices specifying the position in the tensor. Indices
* must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indicies>
inline constexpr auto const& tensor<T, Dim, Rank>::operator()(Indicies const ... indices)const noexcept{
    static_assert(sizeof...(indices) == Rank, "Non matching indexing in tensor");
    const auto index{detail::get_tensor_array_index<Dim, Rank, 0>::index(std::tuple<Indicies...>(indices...))};
    return _data[index];
}

/**
 * Returns a reference of the element at the specified position in the tensor.
 * @param indicies a list of indices specifying the position in the tensor. Indices
 * must be unsigned integers, the number of indices must be equal to the rank of the expression.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
template<typename ...Indicies>
inline constexpr auto& tensor<T, Dim, Rank>::operator ()(Indicies const ... indices) noexcept{
    static_assert(sizeof...(indices) == Rank, "Non matching indexing in tensor");
    const auto index{detail::get_tensor_array_index<Dim, Rank, 0>::index(std::tuple<Indicies...>(indices...))};
    return const_cast<T&>(_data[index]);
}

/**
 * Returns an iterator to the first element of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::begin()noexcept{
    return const_cast<T*>(&_data[0]);
}

/**
 * Returns an iterator to the element following the last element
 * of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::end()noexcept{
    return const_cast<T*>(&_data[Size]);
}

/**
 * Returns a constant iterator to the first element of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::begin()const noexcept{
    return &_data[0];
}

/**
 * Returns a constant iterator to the element following the last element
 * of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::end()const noexcept{
    return &_data[Size];
}

/**
 * Returns a pointer to the memory of the array.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline T* tensor<T, Dim, Rank>::raw_data()  noexcept{
    return const_cast<T*>(&_data[0]);
}

/**
 * Returns a constant pointer to the memory of the array.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline T const* tensor<T, Dim, Rank>::raw_data()const noexcept{
    return &_data[0];
}
//@}

/**
* @name Dimension and rank
*/
//@{
/**
* Returns the dimension of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::dimension() noexcept{
    return Dim;
}

/**
* Returns the rank of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::rank() noexcept{
    return Rank;
}

/**
* Returns the size of the underlying array.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::size() noexcept{
    return Size;
}
//@}

/**
 * @name Deconstructor
*/
//@{
/**
 * Deallocates a Dim-dimensional Rank-tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
tensor<T, Dim, Rank>::~tensor(){
    this->delete_data();
}
//@}

/**
 * @name Constructors
*/
//@{
/**
 * Allocates a Dim-dimensional Rank-tensor.
 * All elements are set to zero.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
tensor<T, Dim, Rank>::tensor():_data() {
    //this->check_size_fill_zeros();
    check_size();
}

/**
 * Allocates a Dim-dimensional Rank-tensor.
 * Elements are copyed from _tensor
 *
 * @param _tensor Tensor to be copyed.
 */
template<typename T, std::size_t Dim, std::size_t Rank>
tensor<T, Dim, Rank>::tensor(tensor<T, Dim, Rank> const& _tensor):
    basetype(_tensor),
    _data()
{
    if(&_tensor != this){
        check_size();
        _data = _tensor._data;
        //detail::assign_tensors::apply(*this, _tensor);
    }
    //std::copy(_tensor.raw_data(), _tensor.raw_data()+Size, raw_data());
}
//@}

/**
 * @name Constructors from initializer list
 */
//@{
/**
 * Allocates a Dim-dimensional Rank-tensor.
 *  @param l The elements of the tensor.
 */
template<typename T, std::size_t Dim, std::size_t Rank>
tensor<T, Dim, Rank>::tensor(std::initializer_list<value_type> const& l):_data() {
    std::copy(l.begin(), l.end(), raw_data());
}
//@}

/**
 * @name Extended copy semantic
 */
//@{
/**
     * The extended copy constructor.
     */
template<typename T, std::size_t Dim, std::size_t Rank>
template<typename _Tensor,  std::enable_if_t<is_tensor_type<_Tensor>::value> *>
tensor<T, Dim, Rank>::tensor(_Tensor const& __tensor):
    _data()
{
    static_assert(_Tensor::rank() == Rank,     "tensor::operator=(): non matching rank");
    static_assert(_Tensor::dimension() == Dim, "tensor::operator=(): no matching dimensions");

    check_size();

    if constexpr (std::is_same_v<_Tensor, tensor<T, Dim, Rank>>){
        _data = std::forward<_Tensor>(__tensor)._data;
    }else{
        if constexpr(std::experimental::is_detected<detail::has_evaluate, _Tensor, decltype (*this)>::value){
            const_cast<_Tensor&>(__tensor).evaluate(*this);
        }else{
            if constexpr(std::experimental::is_detected<detail::has_evaluate, _Tensor>::value){
                const_cast<_Tensor&>(__tensor).evaluate();
            }
            using meta_loop = detail::for_loop_t<rank()-1, Dim>;
            auto func{[&](auto ...numbers){(*this)(numbers...) = __tensor(numbers...);}};
            meta_loop::for_loop(func);
        }
    }
}

template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline std::ostream& tensor<T, Dim, Rank>::print(std::ostream & os)const{
    detail::print_tensor::apply(*this, os);
    return os;
}

template<typename T, std::size_t Dim, std::size_t Rank>
constexpr inline auto tensor<T, Dim, Rank>::fill(value_type const value) noexcept{
    std::fill(_data.begin(),_data.end(),value);
}

template<typename T, std::size_t Dim, std::size_t Rank>
inline auto const& tensor<T, Dim, Rank>::randu()noexcept{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<entry_type> dist(0,1);
    std::generate(begin(),end(),[&](){ return dist(rng);});
    return *this;
}

template<typename T, std::size_t Dim, std::size_t Rank>
inline auto const& tensor<T, Dim, Rank>::randn()noexcept{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<entry_type> dist(0,1);
    std::generate(begin(),end(),[&](){ return dist(rng);});
    return *this;
}

//template<typename T, std::size_t Dim, std::size_t Rank>
//constexpr inline auto const& tensor<T, Dim, Rank>::data()const noexcept{
//    return _data;
//}

//template<typename T, std::size_t Dim, std::size_t Rank>
//constexpr inline auto& tensor<T, Dim, Rank>::data()noexcept{
//    return _data;
//}

template<typename T, std::size_t Dim, std::size_t Rank>
template<typename _Sequence>
constexpr inline auto tensor<T, Dim, Rank>::change_basis_view()noexcept{
    return detail::basis_change_wrapper<tensor&, _Sequence>(*this);
}

/**
 * @ingroup comparison_operators
 * @brief Lesser than
 *
 * Compares element-wise and returns true if
 * all elements in \a lhs are lesser than the
 * elements in \a rhs.
 * @param lhs a \ref tensor_base
 * @param rhs a \ref tensor_baser
 * @return a bool value
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator < (_TensorLHS && __lhs, _TensorRHS && __rhs)noexcept{
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator <(): no matching rank in tensor.");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator <(): no matching dimensions");

    //complex numbers not supported
    static_assert(!detail::is_complex_t<typename TensorTypeLHS::value_type>(), "operator <(): does not support complex numbers");
    static_assert(!detail::is_complex_t<typename TensorTypeRHS::value_type>(), "operator <(): does not support complex numbers");

    //compare function
    auto lambda{[](auto __x, auto __y){return (__x > __y);}};
    return detail::compare_lambda::check(std::forward<_TensorLHS>(__lhs),
                                         std::forward<_TensorRHS>(__rhs), lambda);
}


/**
 * @ingroup comparison_operators
 * @brief Lesser or equal
 *
 * Compares element-wise and returns true if
 * all elements in \a lhs are lesser or equal
 * the elements in \a rhs.
 * @param lhs a \ref tensor_base
 * @param rhs a \ref tensor_baser
 * @return a bool value
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator <= (_TensorLHS && __lhs, _TensorRHS && __rhs)noexcept{
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator <(): no matching rank in tensor.");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator <(): no matching dimensions");

    //complex numbers not supported
    static_assert(!detail::is_complex_t<typename TensorTypeLHS::value_type>(), "operator <(): does not support complex numbers");
    static_assert(!detail::is_complex_t<typename TensorTypeRHS::value_type>(), "operator <(): does not support complex numbers");

    //compare function
    auto lambda{[](auto __x, auto __y){return ((__x > __y) && (__x != __y));}};
    return detail::compare_lambda::check(std::forward<_TensorLHS>(__lhs),
                                         std::forward<_TensorRHS>(__rhs), lambda);
}


/**
 * @ingroup comparison_operators
 * @brief Greater than
 *
 * Compares element-wise and returns true if
 * all elements in \a lhs are greater than the
 * elements in \a rhs.
 * @param lhs a \ref tensor_base
 * @param rhs a \ref tensor_baser
 * @return a bool value
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator > (_TensorLHS && __lhs, _TensorRHS && __rhs)noexcept{
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator >(): no matching rank in tensor.");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator >(): no matching dimensions");

    //complex numbers not supported
    static_assert(!detail::is_complex_t<typename TensorTypeLHS::value_type>(), "operator >(): does not support complex numbers");
    static_assert(!detail::is_complex_t<typename TensorTypeRHS::value_type>(), "operator >(): does not support complex numbers");

    //compare function
    auto lambda{[](auto __x, auto __y){return (__x < __y);}};
    return detail::compare_lambda::check(std::forward<_TensorLHS>(__lhs),
                                         std::forward<_TensorRHS>(__rhs), lambda);
}



/**
 * @ingroup comparison_operators
 * @brief Greater equal
 *
 * Compares element-wise and returns true if
 * all elements in \a lhs are greater or equal
 * the elements in \a rhs.
 * @param lhs a \ref tensor_base
 * @param rhs a \ref tensor_baser
 * @return a bool value
 */
template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator >= (_TensorLHS && __lhs, _TensorRHS && __rhs)noexcept{
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator >=(): no matching rank in tensor.");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator >=(): no matching dimensions");

    //complex numbers not supported
    static_assert(!detail::is_complex_t<typename TensorTypeLHS::value_type>(), "operator >=(): does not support complex numbers");
    static_assert(!detail::is_complex_t<typename TensorTypeRHS::value_type>(), "operator >=(): does not support complex numbers");

    //compare function
    auto lambda{[](auto __x, auto __y){return ((__x < __y )&& (__x != __y));}};
    return detail::compare_lambda::check(std::forward<_TensorLHS>(__lhs),
                                         std::forward<_TensorRHS>(__rhs), lambda);
}


template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator == (_TensorLHS && __lhs, _TensorRHS && __rhs)noexcept{
    using TensorTypeLHS = typename std::decay<_TensorLHS>::type;
    using TensorTypeRHS = typename std::decay<_TensorRHS>::type;

    //check rank of both
    static_assert(TensorTypeLHS::rank() == TensorTypeRHS::rank(), "operator == (): no matching rank");
    //check size of indices
    static_assert(TensorTypeLHS::dimension() == TensorTypeRHS::dimension(), "operator == (): no matching dimensions");
    //do the check
    auto lambda{[](auto __x, auto __y){return (__x != __y);}};
    return detail::compare_lambda::check(std::forward<_TensorLHS>(__lhs),
                                         std::forward<_TensorRHS>(__rhs), lambda);
}


template<typename _TensorLHS, typename _TensorRHS,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorLHS>::type>::value> * = nullptr,
         typename std::enable_if_t<is_tensor_type<typename std::decay<_TensorRHS>::type>::value> * = nullptr>
constexpr inline auto operator != (_TensorLHS && __lhs, _TensorRHS && __rhs)noexcept{
    return !(std::forward<_TensorLHS>(__lhs) == std::forward<_TensorRHS>(__rhs));
}




#endif // TENSOR_MEAT_H
