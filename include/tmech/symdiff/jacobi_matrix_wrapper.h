/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef JACOBI_MATRIX_WRAPPER_H
#define JACOBI_MATRIX_WRAPPER_H

#include <cstdlib>

namespace detail {


template <typename Function, typename Variable>
struct get_derivative_data_type
{
    using type = typename decltype (derivative(Function(), typename std::decay<Variable>::type()))::data_type;
};

template <typename Result, std::size_t Size, std::size_t I, std::size_t J, typename Vector, typename Variables>
struct get_matrix_type_row_h;

template <std::size_t Size, std::size_t I, std::size_t J, typename Vector, typename Variables>
struct get_matrix_type_row_h<std::tuple<>, Size, I, J, Vector, Variables>
        : public get_matrix_type_row_h<std::tuple<typename get_derivative_data_type<decltype (Vector().template get_function<I>()), decltype (std::get<J>(Variables()))>::type>, Size, I, J+1, Vector, Variables>
{};

template <typename ...Result, std::size_t Size, std::size_t I, std::size_t J, typename Vector, typename Variables>
struct get_matrix_type_row_h<std::tuple<Result...>, Size, I, J, Vector, Variables>
        : public get_matrix_type_row_h<std::tuple<Result..., typename get_derivative_data_type<decltype (Vector().template get_function<I>()), decltype (std::get<J>(Variables()))>::type>, Size, I, J+1, Vector, Variables>
{};

template <typename ...Result, std::size_t Size, std::size_t I, typename Vector, typename Variables>
struct get_matrix_type_row_h<std::tuple<Result...>, Size, I, Size, Vector, Variables>
{
    using type = std::tuple<Result...>;
};

template <std::size_t I, typename Vector, typename Variables>
struct get_matrix_type_row
        : public get_matrix_type_row_h<std::tuple<>, Vector::size, I, 0, Vector, Variables>
{};

template <std::size_t I, typename Vector, typename Variables>
using get_matrix_type_row_t = typename get_matrix_type_row<I, Vector, Variables>::type;




template <typename Result, std::size_t Size, std::size_t I, typename Vector, typename Variables>
struct get_matrix_type_h;

template <typename ...Result, std::size_t Size, std::size_t I, typename Vector, typename Variables>
struct get_matrix_type_h<std::tuple<Result...>, Size, I, Vector, Variables>
        : public get_matrix_type_h<std::tuple<Result..., get_matrix_type_row_t<I, Vector, Variables>>, Size, I+1, Vector, Variables>
{};

template <typename ...Result, std::size_t Size, typename Vector, typename Variables>
struct get_matrix_type_h<std::tuple<Result...>, Size, Size, Vector, Variables>
{
    using type = std::tuple<Result...>;
};

template <typename Vector, typename Variables>
struct get_matrix_type
        : public get_matrix_type_h<std::tuple<>, Vector::size, 0, Vector, Variables>
{};

template <typename Vector, typename ...Variables>
using get_matrix_type_t = typename get_matrix_type<Vector, decltype (std::make_tuple(Variables()...))>::type;




//template <typename Vector, typename ...Variables>
//class jacobi_matrix_wrapper_fundamental_type
//{
//public:
//    using size_type = std::size_t;
//    using value_type = typename Vector::value_type;
//    using Vars = std::tuple<Variables...>;

//    jacobi_matrix_wrapper_fundamental_type():
//        _matrix()
//    {}


//    template<typename Data>
//    constexpr inline auto const& operator()(Data const& data){
//        update_details(data);
//        return _matrix;
//    }

//    template<size_type Row, size_type Col>
//    static constexpr inline auto get_derivative(){
//        auto f{Vector().template get_function<Row>()};
//        return derivative(f, std::get<Col>(Vars()));
//    }

//    static constexpr inline size_type rows = Vector::size;
//    static constexpr inline size_type cols = Vector::size;

//    constexpr inline auto set_up_matrix(){

//    }

//    template<typename Data>
//    constexpr inline auto update(Data const& data){
//        update_details(data);
//    }

//    template<size_type I>
//    static constexpr inline auto get_var_ID(){
//        return decltype (forward(std::get<I>(Vars())))::get_ID();
//    }

//private:
//    template<typename T>
//    static constexpr inline auto forward(T && data){
//        return std::forward<T>(data);
//    }

//    template<size_type Row = 0, size_type Col = 0, typename Data>
//    constexpr inline auto update_details(Data const& data){
//        if constexpr (Row < rows && Col < cols){
//            _matrix[Row][Col] = get_derivative<Row, Col>()(data);
//            update_details<Row, Col+1>(data);
//        }else if constexpr (Row < rows && Col == cols) {
//            update_details<Row+1, 0>(data);
//        }
//    }

//    Vars vars;

//    std::array<std::array<value_type, cols>, rows> _matrix;
//};




template <typename Vector, typename ...Variables>
class jacobi_matrix_wrapper_mixed_type
{
public:
    using size_type = std::size_t;
    using value_type = typename Vector::value_type;
    using Vars = std::tuple<Variables...>;

    jacobi_matrix_wrapper_mixed_type():
        _matrix()
    {}

    template<typename Data>
    constexpr inline auto const& operator()(Data const& data){
        update_details(data);
        return _matrix;
    }

    template<size_type Row, size_type Col>
    static constexpr inline auto get_derivative(){
        auto f{Vector().template get_function<Row>()};
        return derivative(f, std::get<Col>(Vars()));
    }

    static constexpr inline size_type rows = Vector::size;
    static constexpr inline size_type cols = Vector::size;

    constexpr inline auto set_up_matrix(){

    }

    template<typename Data>
    constexpr inline auto update(Data const& data){
        update_details(data);
    }

    template<size_type I>
    static constexpr inline auto get_var_ID(){
        return decltype (forward(std::get<I>(Vars())))::get_ID();
    }

    constexpr inline auto& data()const{
        return _matrix;
    }

private:
    template<typename T>
    static constexpr inline auto forward(T && data){
        return std::forward<T>(data);
    }

    template<size_type Row = 0, size_type Col = 0, typename Data>
    constexpr inline auto update_details(Data const& data){
        if constexpr (Row < rows && Col < cols){
            std::get<Col>(std::get<Row>(_matrix)) = get_derivative<Row, Col>()(data);
            update_details<Row, Col+1>(data);
        }else if constexpr (Row < rows && Col == cols) {
            update_details<Row+1, 0>(data);
        }
    }

    Vars vars;
    get_matrix_type_t<Vector, Variables...> _matrix;
};

}
#endif // JACOBI_MATRIX_WRAPPER_H
