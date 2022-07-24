/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef EIGEN_DECOMPOSITION_MEAT_H
#define EIGEN_DECOMPOSITION_MEAT_H

namespace detail {

/**
* @name Constructors
*/
//@{
/**
* Default constructor
*/
template <typename Tensor>
eigen_decomposition_wrapper<Tensor>::eigen_decomposition_wrapper(Tensor const& data)noexcept:
    _data(),
    _eigval(),
    _eigvectors(),
    _eig_basis(),
    _non_repeated_eigenvalues({0}),
    _all_repeated_eigenvalues(false),
    _pair_repeated_eigenvalues(false),
    _data_base(data)
{}

/**
 * Copy constructor
 */
template <typename Tensor>
eigen_decomposition_wrapper<Tensor>::eigen_decomposition_wrapper(eigen_decomposition_wrapper const& data)noexcept:
    _data(),
    _eigval(),
    _eigvectors(),
    _eig_basis(),
    _non_repeated_eigenvalues({0}),
    _all_repeated_eigenvalues(false),
    _pair_repeated_eigenvalues(false),
    _data_base(data._data_base)
{}
//@}


template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::decompose_eigenbasis()noexcept{
    _all_repeated_eigenvalues = false;
    _pair_repeated_eigenvalues = false;
    _data = _data_base;
    std::iota(_non_repeated_eigenvalues.begin(), _non_repeated_eigenvalues.end(), 0);
    if constexpr (data_type_tensor::dimension() == 2){
        evaluate_detail_2D(false, _data(0,0), _data(0,1), _data(1,0), _data(1,1));
    }else if constexpr (data_type_tensor::dimension() == 3){
        evaluate_detail_3D(false, _data(0,0), _data(0,1), _data(0,2), _data(1,1), _data(1,2), _data(2,2));
    }
    get_repeated_eigenvalues();
    evaluate_eigenbasis();

    return std::make_tuple(eigenvalues(), eigenbasis());
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::eigenbasis()const noexcept{
    return eigen_decomposition_basis_wrapper<eigen_decomposition_wrapper<Tensor>>(*this);
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::all_eigenvalues_repeated()const noexcept{
    return _all_repeated_eigenvalues;
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::pair_eigenvalues_repeated()const noexcept{
    return _pair_repeated_eigenvalues;
}

template <typename Tensor>
constexpr inline typename eigen_decomposition_wrapper<Tensor>::size_type eigen_decomposition_wrapper<Tensor>::number_non_repeated_eigenvalues()const noexcept{
    if constexpr (data_type_tensor::dimension() == 2){
        return (all_eigenvalues_repeated() ? 1 : 2);
    }else if constexpr (data_type_tensor::dimension() == 3){
        return (all_eigenvalues_repeated() ? 1 : pair_eigenvalues_repeated() ? 2 : 3);
    }
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::permutation()const noexcept{
    return _permut;
}


template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::non_repeated_eigenvalues_index()const noexcept{
    switch (number_non_repeated_eigenvalues()) {
    case 1:
        return std::vector<size_type>{_non_repeated_eigenvalues[0]};
        break;
    case 2:
        return std::vector<size_type>{_non_repeated_eigenvalues[0],
                                      _non_repeated_eigenvalues[1]};
        break;
    case 3:
        return std::vector<size_type>{_non_repeated_eigenvalues[0],
                                      _non_repeated_eigenvalues[1],
                                      _non_repeated_eigenvalues[2]};
        break;
    default:
        return std::vector<size_type>(0);
        break;
    }
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::get_repeated_eigenvalues()noexcept{
    if constexpr (data_type_tensor::dimension() == 2){
        const auto x1{_eigval[0]}, x2{_eigval[1]};
        const auto max_val{std::max(static_cast<value_type>(1.0),std::max(x1,x2))};
        const auto eps{std::numeric_limits<value_type>::epsilon()*max_val};
        const auto equal_12{std::fabs(x1-x2) <= eps};
        if(equal_12){
            _all_repeated_eigenvalues = true;
            return ;
        }
    }else if constexpr (data_type_tensor::dimension() == 3){
        const auto x1{_eigval[0]}, x2{_eigval[1]}, x3{_eigval[2]};
        const auto max_val{std::max(static_cast<value_type>(1.0), std::max(x1,std::max(x2,x3)))};
        const auto eps{std::numeric_limits<value_type>::epsilon()*max_val};
        const auto equal_12{std::fabs(x1-x2) <= eps};
        const auto equal_13{std::fabs(x1-x3) <= eps};
        const auto equal_23{std::fabs(x2-x3) <= eps};
        _all_repeated_eigenvalues = false;
        _pair_repeated_eigenvalues = false;
        if(equal_12 && equal_13 && equal_23){
            _all_repeated_eigenvalues = true;
            return ;
        }

        //non repeated always the first entry
        if(equal_12){
            //first and thrid
            _non_repeated_eigenvalues[0] = 2;
            _non_repeated_eigenvalues[1] = 0;
            _pair_repeated_eigenvalues = true;
        }else if (equal_13) {
            //first and second
            _non_repeated_eigenvalues[0] = 1;
            _non_repeated_eigenvalues[1] = 0;
            _pair_repeated_eigenvalues = true;
        }else if (equal_23) {
            //first and second
            _non_repeated_eigenvalues[0] = 0;
            _non_repeated_eigenvalues[1] = 1;
            _pair_repeated_eigenvalues = true;
        }
    }
}


template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::evaluate_eigenbasis()noexcept{
    const tmech::eye<value_type, data_type_tensor::dimension(), 2> I;
    if constexpr (data_type_tensor::dimension() == 2){
        if(_all_repeated_eigenvalues){
            _eig_basis[0] = I;
        }else{
            _eig_basis[0] = (_data - _eigval[1]*I)/(_eigval[0] -  _eigval[1]);
            _eig_basis[1] = (_data - _eigval[0]*I)/(_eigval[1] -  _eigval[0]);
        }
    }else if constexpr (data_type_tensor::dimension() == 3){
        if(_all_repeated_eigenvalues){
            _eig_basis[0] = I;
        }else if (_pair_repeated_eigenvalues) {
            _eig_basis[_non_repeated_eigenvalues[0]] = (_data - _eigval[_non_repeated_eigenvalues[1]]*I)/(_eigval[_non_repeated_eigenvalues[0]] - _eigval[_non_repeated_eigenvalues[1]]);
            _eig_basis[_non_repeated_eigenvalues[1]] = I - _eig_basis[_non_repeated_eigenvalues[0]];
        }else{
            _eig_basis[0] = (_data - _eigval[1]*I) * (_data - _eigval[2]*I) / ((_eigval[0]-_eigval[1])*(_eigval[0]-_eigval[2]));
            _eig_basis[1] = (_data - _eigval[0]*I) * (_data - _eigval[2]*I) / ((_eigval[1]-_eigval[0])*(_eigval[1]-_eigval[2]));
            _eig_basis[2] = (_data - _eigval[0]*I) * (_data - _eigval[1]*I) / ((_eigval[2]-_eigval[0])*(_eigval[2]-_eigval[1]));
        }
    }
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::decompose()noexcept{
    std::iota(_permut.begin(),_permut.end(),0);
    _data = _data_base;
    if constexpr (data_type_tensor::dimension() == 2){
        evaluate_detail_2D(true, _data(0,0), _data(0,1), _data(1,0), _data(1,1));
    }else if constexpr (data_type_tensor::dimension() == 3){
        evaluate_detail_3D(true, _data(0,0), _data(0,1), _data(0,2), _data(1,1), _data(1,2), _data(2,2));
    }
    return std::make_tuple(eigenvalues(), eigenvectors());
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::eigenvalues()const noexcept{
    return eigen_decomposition_values_wrapper<eigen_decomposition_wrapper<Tensor>>(*this);
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::eigenvectors()const noexcept{
    return eigen_decomposition_vectors_wrapper<eigen_decomposition_wrapper<Tensor>>(*this);
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::evaluate_detail_2D(bool eigenvectors, value_type const _a00, value_type const _a01, value_type const _a10, value_type const _a11)noexcept{
    const eye<value_type, 2, 2> I;
    const value_type max_element{std::max(_a00, std::max(_a01, std::max(_a10, _a11)))};
    const value_type inv_max_element{static_cast<value_type>(1)/max_element};
    const value_type a00{_a00*inv_max_element};
    const value_type a10{_a10*inv_max_element};
    const value_type a01{_a01*inv_max_element};
    const value_type a11{_a11*inv_max_element};
    const value_type eps_zero{std::numeric_limits<value_type>::epsilon()*std::max(static_cast<value_type>(1), max_element)};

    if(std::abs(max_element) <= eps_zero){
        // tensor is zero.
        _eigval[0] = 0;
        _eigval[1] = 0;
        if(eigenvectors){
            _eigvectors[0](0) = 1;
            _eigvectors[1](1) = 1;
        }
        return ;
    }else if((a10*a01) <= eps_zero){
        // tensor is diagonal.
        _eigval[0] = a00;
        _eigval[1] = a11;
        if(eigenvectors){
            _eigvectors[0](0) = 1;
            _eigvectors[0](1) = 0;
            _eigvectors[1](0) = 0;
            _eigvectors[1](1) = 1;
        }
        return ;
    }

    const auto I1{a00+a11};
    const auto I2{a00*a11 - a01*a10};
    //determine discriminant
    const auto dis{I1*I1 - static_cast<value_type>(4)*I2};
    if(dis <= eps_zero){
        //almost zero
        //both eigenvalues are almmost equal
        _eigval[0] = I1*static_cast<value_type>(0.5);
        _eigval[1] = I1*static_cast<value_type>(0.5);
        if(eigenvectors){
            _eigvectors[0](0) = 1;
            _eigvectors[0](1) = 0;
            _eigvectors[1](0) = 0;
            _eigvectors[1](1) = 1;
        }
    }else{
        if(dis > 0){
            //two distinct real eigenvalues
            const auto sqrt_dis{std::sqrt(dis)};
            _eigval[0] = (I1 + sqrt_dis)*static_cast<value_type>(0.5);
            _eigval[1] = (I1 - sqrt_dis)*static_cast<value_type>(0.5);
        }/*else{
            //at least one eigenvalue is complex
            throw std::runtime_error("at least one eigenvalue is complex");
        }*/
    }
    _eigval[0] *= max_element;
    _eigval[1] *= max_element;

    if(_eigval[0] > _eigval[1]){
        std::swap(_eigval[0], _eigval[1]);
        std::swap(_permut[0], _permut[1]);
    }

    if(eigenvectors){
        for(size_type i{0}; i<2; ++i){
            const auto temp{std::sqrt(1.+std::pow((_eigval[i]-_a00)/_a01,2))};
            _eigvectors[i](0) = static_cast<value_type>(1)/temp;
            _eigvectors[i](1) = (_eigval[i]-_a00)/(temp*_a01);
        }
    }
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::evaluate_detail_3D(bool eigenvectors, value_type const _a00, value_type const _a01, value_type const _a02, value_type const _a11, value_type const _a12, value_type const _a22)noexcept{
    // Precondition the matrix by factoring out the maximum absolute
    // value of the components.  This guards against floating-point
    // overflow when computing the eigenvalues.
    const value_type max0{std::max(std::fabs(_a00), std::fabs(_a01))};
    const value_type max1{std::max(std::fabs(_a02), std::fabs(_a11))};
    const value_type max2{std::max(std::fabs(_a12), std::fabs(_a22))};
    const value_type maxAbsElement{std::max(std::max(max0, max1), max2)};


    if (maxAbsElement == 0.){
        // A is the zero matrix.
        _eigval[0] = 0;
        _eigval[1] = 0;
        _eigval[2] = 0;
        if(eigenvectors){
            _eigvectors[0](0) = 1;
            _eigvectors[1](1) = 1;
            _eigvectors[2](2) = 1;
        }
        return;
    }

    const value_type invMaxAbsElement{static_cast<value_type>(1)/maxAbsElement};
    const value_type a00 = _a00*invMaxAbsElement;
    const value_type a01 = _a01*invMaxAbsElement;
    const value_type a02 = _a02*invMaxAbsElement;
    const value_type a11 = _a11*invMaxAbsElement;
    const value_type a12 = _a12*invMaxAbsElement;
    const value_type a22 = _a22*invMaxAbsElement;

    const value_type norm{a01 * a01 + a02 * a02 + a12 * a12};
    if (norm > static_cast<value_type>(0)){
        // Compute the eigenvalues of A.
        const value_type q{(a00 + a11 + a22) / static_cast<value_type>(3)};

        const value_type b00{a00 - q};
        const value_type b11{a11 - q};
        const value_type b22{a22 - q};

        const value_type p{std::sqrt((b00 * b00 + b11 * b11 + b22 * b22 + norm * static_cast<value_type>(2)) / static_cast<value_type>(6))};

        const value_type c00{b11 * b22 - a12 * a12};
        const value_type c01{a01 * b22 - a12 * a02};
        const value_type c02{a01 * a12 - b11 * a02};
        const value_type det{(b00 * c00 - a01 * c01 + a02 * c02) / (p * p * p)};

        const value_type halfDet{std::min(std::max(det * static_cast<value_type>(0.5), -static_cast<value_type>(1)), static_cast<value_type>(1))};

        constexpr value_type twoThirdsPi{static_cast<value_type>(2)*M_PIf64/static_cast<value_type>(3)};
        const value_type angle{std::acos(halfDet) / static_cast<value_type>(3)};
        const value_type beta2{std::cos(angle) * static_cast<value_type>(2)};
        const value_type beta0{std::cos(angle + twoThirdsPi) * static_cast<value_type>(2)};
        const value_type beta1{-(beta0 + beta2)};

        // The eigenvalues of A are ordered as
        // alpha0 <= alpha1 <= alpha2.
        _eigval[0] = q + p * beta0;
        _eigval[1] = q + p * beta1;
        _eigval[2] = q + p * beta2;

        if(eigenvectors){
            // Compute the eigenvectors so that the set
            // {evec[0], evec[1], evec[2]} is right handed andorthonormal.
            if (halfDet >= 0.){
                compute_eigenvector0(a00, a01, a02, a11, a12, a22, _eigval[2], _eigvectors[2]);
                compute_eigenvector1(a00, a01, a02, a11, a12, a22, _eigvectors[2], _eigval[1], _eigvectors[1]);
                cross(_eigvectors[0].raw_data(), _eigvectors[1].raw_data(), _eigvectors[2].raw_data());
            }else{
                compute_eigenvector0(a00, a01, a02, a11, a12, a22, _eigval[0], _eigvectors[0]);
                compute_eigenvector1(a00, a01, a02, a11, a12, a22, _eigvectors[0], _eigval[1], _eigvectors[1]);
                cross(_eigvectors[2].raw_data(), _eigvectors[0].raw_data(), _eigvectors[1].raw_data());
            }
        }


    }else{
        // The matrix is diagonal.

        _eigval[0] = a00;
        _eigval[1] = a11;
        _eigval[2] = a22;
        if(eigenvectors){
            _eigvectors[0](0) = 1;
            _eigvectors[1](1) = 1;
            _eigvectors[2](2) = 1;
        }
    }

    //check for repeated eigenvalues


    // The preconditioning scaled the matrix A, which scales the
    // eigenvalues. Revert the scaling.
    for(size_type i{0}; i<3; ++i){
        _eigval[i] *= maxAbsElement;
    }

    sort_eigenvalues(1, true, eigenvectors);
}



template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::mul(value_type *data, value_type const*const data_lhs, value_type const*const data_rhs)noexcept{
    const value_type rhs0{data_rhs[0]}, rhs1{data_rhs[1]}, rhs2{data_rhs[2]};
    data[0] = data_lhs[0] * rhs0 + data_lhs[1] * rhs1 + data_lhs[2] * rhs2;
    data[1] = data_lhs[3] * rhs0 + data_lhs[4] * rhs1 + data_lhs[5] * rhs2;
    data[2] = data_lhs[6] * rhs0 + data_lhs[7] * rhs1 + data_lhs[8] * rhs2;
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::dot(value_type const*const data_lhs, value_type const*const data_rhs)noexcept{
    return data_lhs[0] * data_rhs[0] + data_lhs[1] * data_rhs[1] + data_lhs[2] * data_rhs[2];
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::cross(value_type *data, value_type const*const data_lhs, value_type const*const data_rhs)noexcept{
    data[0] = data_lhs[1] * data_rhs[2] - data_lhs[2] * data_rhs[1];
    data[1] = data_lhs[2] * data_rhs[0] - data_lhs[0] * data_rhs[2];
    data[2] = data_lhs[0] * data_rhs[1] - data_lhs[1] * data_rhs[0];
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::compute_orthogonal_complement(tensor1 const& W, tensor1& U, tensor1& V) const noexcept{
    // Robustly compute a right-handed orthonormal set { U, V, W }.
    // The vector W is guaranteed to be unit-length, in which case
    // there is no need to worry about a division by zero when
    // computing invLength.
    if (std::fabs(W(0)) > std::fabs(W(1))){
        // The component of maximum absolute value is either W[0] or W[2].
        const auto invLength = static_cast<value_type>(1) / std::sqrt(W(0) * W(0) + W(2) * W(2));
        U = tensor1({-W(2) * invLength, static_cast<value_type>(0), +W(0) * invLength});
    }else{
        // The component of maximum absolute value is either W[1] or W[2].
        const auto invLength = static_cast<value_type>(1) / std::sqrt(W(1) * W(1) + W(2) * W(2));
        U = tensor1({0, +W(2) * invLength, -W(1) * invLength});
    }

    cross(V.raw_data(), W.raw_data(), U.raw_data());
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::compute_eigenvector0(value_type const a00, value_type const a01, value_type const a02, value_type const a11, value_type const a12, value_type const a22, value_type const eval0,
                                                                                   tensor1& evec0)const noexcept{
    // Compute a unit-length eigenvector for eigenvalue[i0].  The
    // matrix is rank 2, so two of the rows are linearly independent.
    // For a robust computation of the eigenvector, select the two
    // rows whose cross product has largest length of all pairs of
    // rows.
    const tensor1 row0{a00 - eval0, a01, a02 };
    const tensor1 row1{a01, a11 - eval0, a12 };
    const tensor1 row2{a02, a12, a22 - eval0 };
    tensor1 r0xr1, r0xr2, r1xr2;

    cross(r0xr1.raw_data(), row0.raw_data(), row1.raw_data());
    cross(r0xr2.raw_data(), row0.raw_data(), row2.raw_data());
    cross(r1xr2.raw_data(), row1.raw_data(), row2.raw_data());

    const value_type d0{dot(r0xr1.raw_data(), r0xr1.raw_data())};
    const value_type d1{dot(r0xr2.raw_data(), r0xr2.raw_data())};
    const value_type d2{dot(r1xr2.raw_data(), r1xr2.raw_data())};

    value_type dmax = d0;
    int imax = 0;
    if(d1 > dmax){
        dmax = d1;
        imax = 1;
    }
    if(d2 > dmax){
        imax = 2;
    }

    if(imax == 0){
        const auto temp{static_cast<value_type>(1)/std::sqrt(d0)};
        evec0 = r0xr1 * temp;
    }else if(imax == 1){
        const auto temp{static_cast<value_type>(1)/std::sqrt(d1)};
        evec0 = r0xr2 * temp;
    }else{
        const auto temp{static_cast<value_type>(1)/std::sqrt(d2)};
        evec0 = r1xr2 * temp;
    }
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::compute_eigenvector1(value_type const a00, value_type const a01, value_type const a02, value_type const a11, value_type const a12, value_type const a22,
                                                                                   tensor1 const& evec0, value_type const eval1, tensor1& evec1) const noexcept{
    // Robustly compute a right-handed orthonormal set
    // { U, V, evec0 }.
    tensor1 U, V;
    compute_orthogonal_complement(evec0, U, V);

    // Let e be eval1 and let E be a corresponding eigenvector which
    // is a solution to the linear system (A - e*I)*E = 0.  The matrix
    // (A - e*I) is 3x3, not invertible (so infinitely many
    // solutions), and has rank 2 when eval1 and eval are different.
    // It has rank 1 when eval1 and eval2 are equal.  Numerically, it
    // is difficult to compute robustly the rank of a matrix.  Instead,
    // the 3x3 linear system is reduced to a 2x2 system as follows.
    // Define the 3x2 matrix J = [U V] whose columns are the U and V
    // computed previously.  Define the 2x1 vector X = J*E.  The 2x2
    // system is 0 = M * X = (J^T * (A - e*I) * J) * X where J^T is
    // the transpose of J and M = J^T * (A - e*I) * J is a 2x2 matrix.
    // The system may be written as
    //     +-                        -++-  -+       +-  -+
    //     | U^T*A*U - e  U^T*A*V     || x0 | = e * | x0 |
    //     | V^T*A*U      V^T*A*V - e || x1 |       | x1 |
    //     +-                        -++   -+       +-  -+
    // where X has row entries x0 and x1.

    tensor1 AU
    {
        a00 * U(0) + a01 * U(1) + a02 * U(2),
                a01 * U(0) + a11 * U(1) + a12 * U(2),
                a02 * U(0) + a12 * U(1) + a22 * U(2)
    };
    tensor1 AV
    {
        a00 * V(0) + a01 * V(1) + a02 * V(2),
                a01 * V(0) + a11 * V(1) + a12 * V(2),
                a02 * V(0) + a12 * V(1) + a22 * V(2)
    };
    //mul(AU.raw_data(),_data.raw_data(),U.raw_data());
    //mul(AU.raw_data(),_data.raw_data(),V.raw_data());

    value_type m00{dot(U.raw_data(),AU.raw_data()) - eval1};
    value_type m01{dot(U.raw_data(),AV.raw_data())};
    value_type m11{dot(V.raw_data(),AV.raw_data()) - eval1};


    // For robustness, choose the largest-length row of M to compute
    // the eigenvector.  The 2-tuple of coefficients of U and V in the
    // assignments to eigenvector[1] lies on a circle, and U and V are
    // unit length and perpendicular, so eigenvector[1] is unit length
    // (within numerical tolerance).
    const value_type absM00{std::fabs(m00)};
    const value_type absM01{std::fabs(m01)};
    const value_type absM11{std::fabs(m11)};
    if (absM00 >= absM11){
        const value_type maxAbsComp{std::max(absM00, absM01)};
        if (maxAbsComp > static_cast<value_type>(0)){
            if (absM00 >= absM01){
                m01 /= m00;
                m00 = static_cast<value_type>(1) / std::sqrt(static_cast<value_type>(1) + m01 * m01);
                m01 *= m00;
            }else{
                m00 /= m01;
                m01 = static_cast<value_type>(1) / std::sqrt(static_cast<value_type>(1) + m00 * m00);
                m00 *= m01;
            }
            evec1 = U*m01 - V*m00;
        }else{
            evec1 = U;
        }
    }
    else{
        const value_type maxAbsComp{std::max(absM11, absM01)};
        if (maxAbsComp > static_cast<value_type>(0)){
            if (absM11 >= absM01){
                m01 /= m11;
                m11 = static_cast<value_type>(1) / std::sqrt(static_cast<value_type>(1) + m01 * m01);
                m01 *= m11;
            }else{
                m11 /= m01;
                m01 = static_cast<value_type>(1) / std::sqrt(static_cast<value_type>(1) + m11 * m11);
                m11 *= m01;
            }
            evec1 = U*m11 - V*m01;
        }else{
            evec1 = U;
        }
    }
}

template <typename Tensor>
constexpr inline auto eigen_decomposition_wrapper<Tensor>::sort_eigenvalues(int sortType, bool isRotation, bool eigenvectors)noexcept{
    if (sortType != 0){
        // Sort the eigenvalues to eval[0] <= eval[1] <= eval[2].
        std::array<size_type, 3> index{0,1,2};
        if (_eigval[index[0]] > _eigval[index[1]])
            std::swap(index[0], index[1]);
        if (_eigval[index[0]] > _eigval[index[2]])
            std::swap(index[0], index[2]);
        if (_eigval[index[1]] > _eigval[index[2]])
            std::swap(index[1], index[2]);

//        if (_eigval[0] < _eigval[1]){
//            if (_eigval[2] < _eigval[1]){
//                // even permutation
//                index[0] = 2;
//                index[1] = 0;
//                index[2] = 1;
//            }else if (_eigval[2] < _eigval[1]){
//                // odd permutation
//                index[0] = 0;
//                index[1] = 2;
//                index[2] = 1;
//                isRotation = !isRotation;
//            }else{
//                // even permutation
//                index[0] = 0;
//                index[1] = 1;
//                index[2] = 2;
//            }
//        }else{
//            if (_eigval[2] < _eigval[1]){
//                // odd permutation
//                index[0] = 2;
//                index[1] = 1;
//                index[2] = 0;
//                isRotation = !isRotation;
//            }else if (_eigval[2] < _eigval[0]){
//                // even permutation
//                index[0] = 1;
//                index[1] = 2;
//                index[2] = 0;
//            }else{
//                // odd permutation
//                index[0] = 1;
//                index[1] = 0;
//                index[2] = 2;
//                isRotation = !isRotation;
//            }
//        }

        //odd permutation
        if((index[0] == 0 && index[1] == 2 && index[2] == 1)
                || (index[0] == 1 && index[1] == 0 && index[2] == 2)
                || (index[0] == 2 && index[1] == 1 && index[2] == 0))
            isRotation = !isRotation;

        if (sortType == -1){
            // The request is for eval[0] >= eval[1] >= eval[2]. This
            // requires an odd permutation, (i0,i1,i2) -> (i2,i1,i0).
            std::swap(index[0], index[2]);
            isRotation = !isRotation;
        }

        const std::array<value_type, 3> unorderedEVal{_eigval};
        for(size_type j = 0; j < 3; ++j){
            _eigval[j] = unorderedEVal[index[j]];
            _permut[j] = index[j];
        }

        if(eigenvectors){
            const std::array<tensor1, 3> unorderedEVec{_eigvectors};
            for(size_type j = 0; j < 3; ++j){
                _eigvectors[j] = unorderedEVec[index[j]];
            }
        }
    }
    if(eigenvectors){
        // Ensure the ordered eigenvectors form a right-handed basis.
        if (!isRotation){
            for (size_type j = 0; j < 3; ++j){
                _eigvectors[2](j) = -_eigvectors[2](j);
            }
        }
    }
}

} // NAMESPACE DETAIL
#endif // EIGEN_DECOMPOSITION_MEAT_H
