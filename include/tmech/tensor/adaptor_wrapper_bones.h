/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef ADAPTOR_WRAPPER_BONES_H
#define ADAPTOR_WRAPPER_BONES_H

template<std::size_t _Dim, bool _ShearStrain>
struct abq_std;

template<std::size_t _Dim, bool _ShearStrain>
struct abq_exp;

template<std::size_t _Dim, bool _ShearStrain>
struct voigt;

template<std::size_t _Dim>
struct full;


template <typename _T, std::size_t _Dim, std::size_t _Rank, typename _Type>
class adaptor : public tensor_base<adaptor<_T, _Dim, _Rank, _Type>>
{
public:
    using value_type = typename std::remove_const<_T>::type;
    using size_type  = std::size_t;

    constexpr adaptor(_T *__data)noexcept;

    constexpr adaptor(adaptor const& __data)noexcept;

    template<typename _Derived>
    constexpr inline auto const& operator=(tensor_base<_Derived> const& tensor_base)noexcept;

    template<typename ...Indices>
    constexpr inline auto operator()(Indices ...indices)const noexcept;

    static constexpr inline auto rank()noexcept;

    static constexpr inline auto dimension()noexcept;

private:
    _T * _data;
};

template<std::size_t _Dim, bool _ShearStrain = false>
struct abq_std
{
    using size_type = std::size_t;

    //abq std 3D {11,22,33,12,13,23}
    //abq std 2D {11,22,12}

    //if _ShearStrain == true shear strains are multiplied with 0.5

    static_assert (_Dim == 3, "abq_std: currently is only 3D supported");

    template<typename T>
    static constexpr inline auto apply(T const*const __ptr, size_type const __i, size_type const __j)noexcept{
        constexpr auto _v2t{vt2()};
        if constexpr (_ShearStrain){
            return __ptr[_v2t[__i][__j]] * (__i == __j ? 1 : 0.5);
        }else{
            return __ptr[_v2t[__i][__j]];
        }
    }

    template<typename T>
    static constexpr inline auto apply(T const*const __ptr, size_type const __i, size_type const __j, size_type const __k, size_type const __l)noexcept{
        constexpr auto _v2t{vt2()};
        constexpr auto Rows{(_Dim == 2 ? 3 : 6)};
        return __ptr[_v2t[__i][__j]*Rows+_v2t[__k][__l]];
    }

    template<typename __T, typename __Tensor>
    static constexpr inline auto assign_tensor(__T* __ptr, __Tensor const& __input)noexcept{
        static_assert (__Tensor::rank() == 2 || __Tensor::rank() == 4, "voigt::assign_tensor() no matching rank");
        constexpr size_type Rows{(_Dim == 2 ? 3 : 6)};
        constexpr auto _t2v{t2v()};

        if constexpr (__Tensor::rank() == 2){
            for(size_type i{0}; i<Rows; ++i){
                if constexpr (_ShearStrain){
                    __ptr[i] = __input(_t2v[i][0], _t2v[i][1]) * (_Dim == 2 ? (i == 2 ? 2 : 1) : (i >= 3 ? 2 : 1));
                }else{
                    __ptr[i] = __input(_t2v[i][0], _t2v[i][1]);
                }
            }
        }else if constexpr (__Tensor::rank() == 4){
            for(size_type i{0}; i<Rows; ++i){
                for(size_type j{0}; j<Rows; ++j){
                    __ptr[i*Rows+j] = __input(_t2v[i][0], _t2v[i][1], _t2v[j][0], _t2v[j][1]);
                }
            }
        }
    }

private:
    static constexpr inline auto vt2()noexcept{
        if constexpr (_Dim == 2){
            return std::array<std::array<size_type, 2>, 2>{0,2, 2,1};
        }else{
            //{11,22,33,12,13,23}
            // 0  1  2  3  4  5
            return std::array<std::array<size_type, 3>, 3>{
                0,3,4,
                3,1,5,
                4,5,2};
        }
    }

    static constexpr inline auto t2v()noexcept{
        if constexpr (_Dim == 2){
            return std::array<std::array<size_type, 2>, 3>{0,0, 1,1, 0,1};
        }else{
            return std::array<std::array<size_type, 2>, 6>{0,0, 1,1, 2,2, 0,1, 0,2, 1,2};
        }
    }
};


template<std::size_t _Dim, bool _ShearStrain = false>
struct abq_exp
{
    using size_type = std::size_t;

    //abq exp 3D {11,22,33,12,23,13}
    //???????
    //abq exp 2D {11,22,12}

    //if _ShearStrain == true shear strains are multiplied with 0.5

    static_assert (_Dim == 3, "abq_exp: currently is only 3D supported");

    template<typename T>
    static constexpr inline auto apply(T const*const __ptr, size_type const __i, size_type const __j)noexcept{
        constexpr auto _v2t{vt2()};
        if constexpr (_ShearStrain){
            return __ptr[_v2t[__i][__j]] * (__i == __j ? 1 : 0.5);
        }else{
            return __ptr[_v2t[__i][__j]];
        }
    }

    template<typename T>
    static constexpr inline auto apply(T const*const __ptr, size_type const __i, size_type const __j, size_type const __k, size_type const __l)noexcept{
        constexpr auto _v2t{vt2()};
        constexpr auto Rows{(_Dim == 2 ? 3 : 6)};
        return __ptr[_v2t[__i][__j]*Rows+_v2t[__k][__l]];
    }

    template<typename __T, typename __Tensor>
    static constexpr inline auto assign_tensor(__T* __ptr, __Tensor const& __input)noexcept{
        static_assert (__Tensor::rank() == 2 || __Tensor::rank() == 4, "voigt::assign_tensor() no matching rank");
        constexpr size_type Rows{(_Dim == 2 ? 3 : 6)};
        constexpr auto _t2v{t2v()};

        if constexpr (__Tensor::rank() == 2){
            for(size_type i{0}; i<Rows; ++i){
                if constexpr (_ShearStrain){
                    __ptr[i] = __input(_t2v[i][0], _t2v[i][1]) * (_Dim == 2 ? (i == 2 ? 2 : 1) : (i >= 3 ? 2 : 1));
                }else{
                    __ptr[i] = __input(_t2v[i][0], _t2v[i][1]);
                }
            }
        }else if constexpr (__Tensor::rank() == 4){
            for(size_type i{0}; i<Rows; ++i){
                for(size_type j{0}; j<Rows; ++j){
                    __ptr[i*Rows+j] = __input(_t2v[i][0], _t2v[i][1], _t2v[j][0], _t2v[j][1]);
                }
            }
        }
    }

private:
    static constexpr inline auto vt2()noexcept{
        if constexpr (_Dim == 2){
            return std::array<std::array<size_type, 2>, 2>{0,2, 2,1};
        }else{
            //{11,22,33,12,23,13}
            // 0  1  2  3  4  5
            return std::array<std::array<size_type, 3>, 3>{0,3,5, 3,1,4, 5,4,2};
        }
    }

    static constexpr inline auto t2v()noexcept{
        if constexpr (_Dim == 2){
            return std::array<std::array<size_type, 2>, 3>{0,0, 1,1, 0,1};
        }else{
            return std::array<std::array<size_type, 2>, 6>{0,0, 1,1, 2,2, 0,1, 1,2, 0,2};
        }
    }
};



template<std::size_t _Dim, bool _ShearStrain = false>
struct voigt
{
    using size_type = std::size_t;

    //Voigt 3D {11,22,33,23,13,12}
    //Voigt 2D {11,22,12}

    //if _ShearStrain == true shear strains are multiplied with 0.5

    template<typename T>
    static constexpr inline auto apply(T const*const __ptr, size_type const __i, size_type const __j)noexcept{
        constexpr auto _v2t{vt2()};
        if constexpr (_ShearStrain){
            return __ptr[_v2t[__i][__j]] * (__i == __j ? 1 : 0.5);
        }else{
            return __ptr[_v2t[__i][__j]];
        }
    }

    template<typename T>
    static constexpr inline auto apply(T const*const __ptr, size_type const __i, size_type const __j, size_type const __k, size_type const __l)noexcept{
        constexpr auto _v2t{vt2()};
        constexpr auto Rows{(_Dim == 2 ? 3 : 6)};
        return __ptr[_v2t[__i][__j]*Rows+_v2t[__k][__l]];
    }

    template<typename __T, typename __Tensor>
    static constexpr inline auto assign_tensor(__T* __ptr, __Tensor const& __input)noexcept{
        static_assert (__Tensor::rank() == 2 || __Tensor::rank() == 4, "voigt::assign_tensor() no matching rank");
        constexpr size_type Rows{(_Dim == 2 ? 3 : 6)};
        constexpr auto _t2v{t2v()};

        if constexpr (__Tensor::rank() == 2){
            for(size_type i{0}; i<Rows; ++i){
                if constexpr (_ShearStrain){
                    __ptr[i] = __input(_t2v[i][0], _t2v[i][1]) * (_Dim == 2 ? (i == 2 ? 2 : 1) : (i >= 3 ? 2 : 1));
                }else{
                    __ptr[i] = __input(_t2v[i][0], _t2v[i][1]);
                }
            }
        }else if constexpr (__Tensor::rank() == 4){
            for(size_type i{0}; i<Rows; ++i){
                for(size_type j{0}; j<Rows; ++j){
                    __ptr[i*Rows+j] = __input(_t2v[i][0], _t2v[i][1], _t2v[j][0], _t2v[j][1]);
                }
            }
        }
    }

private:
    static constexpr inline auto vt2()noexcept{
        if constexpr (_Dim == 2){
            return std::array<std::array<size_type, 2>, 2>{0,2, 2,1};
        }else{
            return std::array<std::array<size_type, 3>, 3>{0,5,4, 5,1,3, 4,3,2};
        }
    }

    static constexpr inline auto t2v()noexcept{
        if constexpr (_Dim == 2){
            return std::array<std::array<size_type, 2>, 3>{0,0, 1,1, 0,1};
        }else{
            return std::array<std::array<size_type, 2>, 6>{0,0, 1,1, 2,2, 1,2, 0,2, 0,1};
        }
    }
};


template<std::size_t _Dim>
struct full
{
    //Full 3D {11 12 13 21 22 23 31 32 33}
    //Full 2D {11 12 21 22}
    using size_type = std::size_t;

    template<typename T>
    static constexpr inline auto apply(T const* const __ptr, size_type const __i)noexcept{
        return __ptr[__i];
    }

    template<typename T>
    static constexpr inline auto apply(T const* const __ptr, size_type const __i, size_type const __j)noexcept{
        return __ptr[__i*_Dim+__j];
    }

    template<typename T>
    static constexpr inline auto apply(T const* const __ptr, size_type const __i, size_type const __j, size_type const __k, size_type const __l)noexcept{
        constexpr auto Dim1{_Dim*_Dim*_Dim};
        constexpr auto Dim2{_Dim*_Dim};
        return __ptr[__i*Dim1+__j*Dim2+__k*_Dim+__l];
    }

    template<typename __T, typename __Tensor>
    static constexpr inline auto assign_tensor(__T* __ptr, __Tensor const& __input)noexcept{
        static_assert (__Tensor::rank() != 1 || __Tensor::rank() == 2 || __Tensor::rank() == 4, "full::assign_tensor() no matching rank");
        if constexpr (__Tensor::rank() == 1){
            for(size_type i{0}; i<_Dim; ++i){
                __ptr[i] = __input(i);
            }
        }else if constexpr(__Tensor::rank() == 2){
            for(size_type i{0}; i<_Dim; ++i){
                for(size_type j{0}; j<_Dim; ++j){
                    __ptr[i*_Dim+j] = __input(i,j);
                }
            }
        }else if constexpr(__Tensor::rank() == 4){
            constexpr auto Dim1{_Dim*_Dim*_Dim};
            constexpr auto Dim2{_Dim*_Dim};
            for(size_type i{0}; i<_Dim; ++i){
                for(size_type j{0}; j<_Dim; ++j){
                    for(size_type k{0}; k<_Dim; ++k){
                        for(size_type l{0}; l<_Dim; ++l){
                            __ptr[i*Dim1+j*Dim2+k*_Dim+l] = __input(i,j,k,l);
                        }
                    }
                }
            }
        }
    }
};


//static constexpr size_type voigt_to_tensor_dim_3[3][3]{{0, 5, 4},
//                                                       {5, 1, 3},
//                                                       {4, 3, 2}};

//static constexpr size_type voigt_to_tensor_dim_2[2][2]{{0, 2},
//                                                       {2, 1}};

//static constexpr size_type abqstd_to_tensor_dim_3[3][3]{{0, 3, 4},
//                                                        {3, 1, 5},
//                                                        {4, 5, 2}};

//static constexpr size_type abqexp_to_tensor_dim_3[3][3]{{0, 3, 5},
//                                                        {3, 1, 4},
//                                                        {5, 4, 2}};

//static constexpr size_type abq_to_tensor_dim_22[2][2]{{0, 3},
//                                                      {3, 1}};

//static constexpr size_type abq_to_tensor_dim_23[3][3]{{0, 3, 0},
//                                                      {3, 1, 0},
//                                                      {0, 0, 2}};

////Nonsymmetric2D {11 22 33 12 21}
//static constexpr size_type abqnonsymetric_to_tensor_dim_22[2][2]{{0, 3},
//                                                                 {4, 1}};
////Nonsymmetric2D {11 22 33 12 21}
//static constexpr size_type abqnonsymetric_to_tensor_dim_23[3][3]{{0, 3, 0},
//                                                                 {4, 1, 0},
//                                                                 {0, 0, 2}};

////Nonsymmetric3D {11 22 33 12 23 32 21 32 13}
//static constexpr size_type abqnonsymetric_to_tensor_dim_3[3][3]{{0, 3, 8},
//                                                                {6, 1, 4},
//                                                                {7, 5, 2}};




//static constexpr std::pair<size_type, size_type> tensor_to_voigt_dim_2[3]{{0,0},
//                                                                          {1,1},
//                                                                          {0,1}};

//static constexpr std::pair<size_type, size_type> tensor_to_voigt_dim_3[6]{{0,0},
//                                                                          {1,1},
//                                                                          {2,2},
//                                                                          {1,2},
//                                                                          {0,2},
//                                                                          {0,1}};

//static constexpr std::pair<size_type, size_type> tensor_to_abq_dim_23[4]{{0,0},
//                                                                         {1,1},
//                                                                         {2,2},
//                                                                         {0,1}};

//static constexpr std::pair<size_type, size_type> tensor_to_abq_dim_22[3]{{0,0},
//                                                                         {1,1},
//                                                                         {0,1}};


//static constexpr std::pair<size_type, size_type> tensor_to_abqstd_dim_3[6]{{0,0},
//                                                                           {1,1},
//                                                                           {2,2},
//                                                                           {0,1},
//                                                                           {0,2},
//                                                                           {1,2}};
#endif // ADAPTOR_WRAPPER_BONES_H
