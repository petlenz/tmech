#ifndef POW_TENSOR_DERIVATIVE_WRAPPER_BONES_H
#define POW_TENSOR_DERIVATIVE_WRAPPER_BONES_H

namespace detail {

template <typename _Base, typename _Tensor>
class pow_tensor_derivative_wrapper : public tensor_base<pow_tensor_derivative_wrapper<_Base, _Tensor>>
{
public:
    using size_type  = std::size_t;
    using value_type = typename _Tensor::value_type;

    pow_tensor_derivative_wrapper(_Base const & __base, _Tensor const& __derivative);

    pow_tensor_derivative_wrapper(pow_tensor_derivative_wrapper const & __data);

    constexpr inline auto const& operator()(size_type const i, size_type const j, size_type const k, size_type const l)const;

    static constexpr inline auto dimension();

    static constexpr inline auto rank();

    constexpr inline auto raw_data()const;

    constexpr inline auto evaluate();

private:
    _Tensor const& _derivative;
    _Base const& _base;
};

} // NAMESPACE DETAIL

#endif // POW_TENSOR_DERIVATIVE_WRAPPER_BONES_H
