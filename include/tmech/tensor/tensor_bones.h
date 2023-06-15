/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef TENSOR_BONES_H
#define TENSOR_BONES_H

/**
 * @class tensor
 * @brief Dense multidimensional tensor.
 *
 * @tparam T The type of the tensor holding the elements.
 * @tparam Dim Dimension of the underlying physical problem.
 * @tparam Rank Rank of the tensor.
*/
template<typename T, std::size_t Dim, std::size_t Rank>
class tensor : public tensor_base<tensor<T, Dim, Rank>>
{
    using basetype = tensor_base<tensor<T, Dim, Rank>>;
public:
    static constexpr std::size_t Size{detail::get_tensor_size<Dim, Rank>::size};
    static constexpr bool dynamic_memory{((Size <= TMECH_MAX_STATIC_SIZE) ? false : true)};
    using value_data_type = typename detail::get_tensor_array_type<T, Size, dynamic_memory>::data_type;
    using data_type  = tensor<T, Dim, Rank>;
    using entry_type = typename detail::get_complex_entry_type<T>::type;
    using value_type = T;
    using size_type  = std::size_t;

    static_assert(Dim != 0,
    "Tensors must have a dimension greater than or equal to one.");

    static_assert(Rank != 0,
    "Tensors must have a rank greater than or equal to one.");

    tensor();

    tensor(tensor<T, Dim, Rank> const& _tensor);

    template<typename _Tensor,  std::enable_if_t<is_tensor_type<_Tensor>::value> * = nullptr>
    tensor(_Tensor const& tensor_base);

    tensor(std::initializer_list<value_type> const& data);

    ~tensor();

    /**
      * \brief Assigns tensors to each other.
      *
      * \note This is a special case of the templated operator=. Its purpose is
      * to prevent a default operator= from hiding the templated operator=.
      *
      */
    constexpr inline auto const& operator=(tensor const& tensor) noexcept;

    template<typename Derived>
    constexpr inline auto const& operator=(tensor_base<Derived> const& tensor_base) noexcept;

    template<typename _Tensor,  std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
    constexpr inline auto const& operator=(_Tensor && __tensor) noexcept;

    template <typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
    constexpr inline auto const& operator+=(_Tensor && __rhs) noexcept;

    template <typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
    constexpr inline auto const& operator-=(_Tensor && __rhs) noexcept;

    template <typename _Tensor, std::enable_if_t<is_tensor_type<typename std::decay<_Tensor>::type>::value> * = nullptr>
    constexpr inline auto const& operator*=(_Tensor && __rhs) noexcept;

    constexpr inline auto const& operator*=(value_type const value) noexcept;

    constexpr inline auto const& operator/=(value_type const value) noexcept;

    template<typename ...Indicies>
    inline constexpr auto const& operator()(Indicies const ... indices)const noexcept;

    template<typename ...Indicies>
    inline constexpr auto& operator()(Indicies const ... indices) noexcept;

    template<typename _TensorLHS, typename _TensorRHS, typename, typename >
    friend constexpr inline auto operator == (_TensorLHS && __lhs, _TensorRHS && __rhs) noexcept;

    template<typename _TensorLHS, typename _TensorRHS, typename, typename >
    friend constexpr inline auto operator != (_TensorLHS && __lhs, _TensorRHS && __rhs) noexcept;

    static constexpr inline auto rank() noexcept;

    static constexpr inline auto dimension() noexcept;

    static constexpr inline auto size() noexcept;

    constexpr inline std::ostream& print(std::ostream & os) const;

    constexpr inline auto fill(value_type const value) noexcept;

    inline auto const& randu() noexcept;

    inline auto const& randn() noexcept;

    constexpr inline auto begin() noexcept;

    constexpr inline auto end() noexcept;

    constexpr inline auto begin()const noexcept;

    constexpr inline auto end()const noexcept;

    constexpr inline T* raw_data() noexcept;

    constexpr inline T const* raw_data()const noexcept;

    static constexpr inline auto simple_evaluation(){return true;}

    constexpr inline auto direct_access(std::size_t __idx)const noexcept{
        return _data[__idx];
    }

//    constexpr inline auto const& data()const noexcept;

//    constexpr inline auto& data()noexcept;

    template<typename _Sequence>
    constexpr inline auto change_basis_view()noexcept;

private:
    // Allow an arbitrary Tensor to access the underlying values.
    template <typename, std::size_t, std::size_t>
    friend class tensor;

    template<bool DynamicMemory = dynamic_memory, typename std::enable_if_t<!DynamicMemory>* = nullptr >
    constexpr inline auto check_size(){}

    template<bool DynamicMemory = dynamic_memory, typename std::enable_if_t<DynamicMemory>* = nullptr >
    constexpr inline auto check_size(){
        if(_data.size() == 0){
            _data.resize(Size);
        }
    }

    template<bool DynamicMemory = dynamic_memory, typename std::enable_if_t<!DynamicMemory>* = nullptr >
    constexpr inline auto check_size_fill_zeros(){}

    template<bool DynamicMemory = dynamic_memory, typename std::enable_if_t<DynamicMemory>* = nullptr >
    constexpr inline auto check_size_fill_zeros(){
        if(_data.size() == 0){
            _data.resize(Size);
        }
        _data.fill(0);
    }

    template<bool DynamicMemory = dynamic_memory, typename std::enable_if_t<!DynamicMemory>* = nullptr >
    constexpr inline auto delete_data(){}

    template<bool DynamicMemory = dynamic_memory, typename std::enable_if_t<DynamicMemory>* = nullptr >
    constexpr inline auto delete_data(){}

    alignas(32) value_data_type _data;
};

#endif // TENSOR_BONES_H
