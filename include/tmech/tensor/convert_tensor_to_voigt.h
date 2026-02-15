/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef CONVERT_TENSOR_TO_VOIGT_H
#define CONVERT_TENSOR_TO_VOIGT_H

namespace detail {


template <typename _Tuple, typename _Tensor>
struct convert_tensor_to_voigt_wrapper
{
    using size_type = std::size_t;
    using value_type = typename _Tensor::value_type;
    static constexpr size_type size_tuple{std::tuple_size_v<_Tuple>};
    static_assert (size_tuple*2 == _Tensor::rank(), "convert_tensor_to_voigt:: tuple::size()*2 != tensor::rank()");

private:
    template <size_type _Iter = size_tuple-1, typename _Result = sequence<>>
    struct get_index_sequence
    {
      static constexpr auto tuple{min_value_squence_t<
          std::tuple_element_t<size_tuple - 1 - _Iter, _Tuple>, 1>()};
      using sequence = append_sequence_end_t<
          typename get_index_sequence<_Iter - 1, _Result>::sequence,
          typename std::remove_const<decltype(tuple)>::type>;
    };

    template <typename _Result>
    struct get_index_sequence<0, _Result>
    {
      static constexpr auto tuple{
          min_value_squence_t<std::tuple_element_t<size_tuple - 1, _Tuple>,
                              1>()};
      using sequence = append_sequence_end_t<
          _Result, typename std::remove_const<decltype(tuple)>::type>;
    };

public:
    static inline auto convert(_Tensor const& __tensor, value_type * __ptr)noexcept{
        using index_tuple = typename get_index_sequence<>::sequence;
        //meta for loop
        using meta_loop = typename meta_for_loop_deep_symmetric<_Tensor::dimension(), _Tensor::rank()/2 - 1>::type;

        evaluate::apply(__tensor);

        const auto lambda{[&](auto&&... numbers){
                __ptr[single_index(numbers...)] = tuple_call(__tensor, std::make_tuple(numbers...), index_tuple());
        }};

        meta_loop::loop(lambda);
    }

private:
    template<typename ..._Numbers>
    static constexpr inline size_type single_index(size_type const __first, size_type const __second, _Numbers... __numbers)noexcept{
        constexpr size_type Size{(_Tensor::dimension() == 2 ? 3 : 6)};
        return (__first == __second ? __first : Size - (__first + __second))*Size + single_index(__numbers...);
    }

    static constexpr inline size_type single_index(size_type const __first, size_type const __second)noexcept{
        constexpr size_type Size{(_Tensor::dimension() == 2 ? 3 : 6)};
        return (__first == __second ? __first : Size - (__first + __second));
    }
};

}

#endif // CONVERT_TENSOR_TO_VOIGT_H
