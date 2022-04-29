/***************************************************************************
* Copyright (c) Peter Lenz                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef MATH_detail_H
#define MATH_detail_H

namespace detail {

template<typename T>
static constexpr inline auto det_2_2(T const*const a)noexcept{
    //0 1
    //2 3
    return a[0]*a[3] - a[1]*a[2];
}

template<typename A>
constexpr inline auto det_3_3(A const*const a)noexcept{
    const auto A0{a[0]};
    const auto A1{a[1]};
    const auto A2{a[2]};
    const auto A3{a[3]};
    const auto A4{a[4]};
    const auto A5{a[5]};
    const auto A6{a[6]};
    const auto A7{a[7]};
    const auto A8{a[8]};

    return A0*(A4*A8-A5*A7)
           + A1*(A5*A6-A3*A8)
           + A2*(A3*A7-A4*A6);
}

template<typename T>
constexpr inline auto det_4_4(T const*const a)noexcept{
    const auto A0{a[0]};
    const auto A1{a[1]};
    const auto A2{a[2]};
    const auto A3{a[3]};

    const auto A4{a[4]};
    const auto A5{a[5]};
    const auto A6{a[6]};
    const auto A7{a[7]};

    const auto A8{a[8]};
    const auto A9{a[9]};
    const auto A10{a[10]};
    const auto A11{a[11]};

    const auto A12{a[12]};
    const auto A13{a[13]};
    const auto A14{a[14]};
    const auto A15{a[15]};

    const auto temp1{A10*A15-A11*A14};
    const auto temp2{A14*A9-A10*A13};
    const auto temp3{A15*A9-A11*A13};
    const auto temp4{A15*A8-A11*A12};
    const auto temp5{A13*A8-A12*A9};
    const auto temp6{A14*A8-A10*A12};

    return A0*((temp1)*A5  + A7*(temp2)-A6*(temp3))
           +A2*(-A5*(temp4) + A7*(temp5)+A4*(temp3))
           -A3*(-A5*(temp6) + A6*(temp5)+A4*(temp2))
           -A1*((temp1)*A4  + A7*(temp6)-A6*(temp4));
}

template<typename T>
constexpr inline auto det_5_5(T const*const a)noexcept{
    const auto A0{a[0]};
    const auto A1{a[1]};
    const auto A2{a[2]};
    const auto A3{a[3]};
    const auto A4{a[4]};

    const auto A5{a[5]};
    const auto A6{a[6]};
    const auto A7{a[7]};
    const auto A8{a[8]};
    const auto A9{a[9]};

    const auto A10{a[10]};
    const auto A11{a[11]};
    const auto A12{a[12]};
    const auto A13{a[13]};
    const auto A14{a[14]};

    const auto A15{a[15]};
    const auto A16{a[16]};
    const auto A17{a[17]};
    const auto A18{a[18]};
    const auto A19{a[19]};

    const auto A20{a[20]};
    const auto A21{a[21]};
    const auto A22{a[22]};
    const auto A23{a[23]};
    const auto A24{a[24]};

    const auto temp1{A18*A24-A19*A23};
    const auto temp2{A17*A24-A19*A22};
    const auto temp3{A16*A22-A17*A21};
    const auto temp4{A17*A23-A18*A22};
    const auto temp5{A16*A23-A18*A21};
    const auto temp6{A16*A24-A19*A21};
    const auto temp7{A15*A22-A17*A20};
    const auto temp8{A15*A21-A16*A20};
    const auto temp9{A15*A23-A18*A20};
    const auto temp10{A15*A24-A19*A20};
    const auto temp11{A12*(temp1)};
    const auto temp12{A14*(temp5)};
    const auto temp13{A11*(temp2)};
    const auto temp14{A13*(temp3)};
    const auto temp15{A14*(temp9)};
    const auto temp16{A10*(temp2)};
    const auto temp17{A13*(temp7)};
    const auto temp18{A11*(temp1)};
    const auto temp19{A10*(temp6)};
    const auto temp20{A13*(temp8)};
    const auto temp21{A14*(temp7)};
    const auto temp22{A12*(temp8)};
    const auto temp23{A11*(temp4)};
    const auto temp24{A10*(temp5)};
    const auto temp25{A14*(temp4)};
    const auto temp26{A14*(temp3)};
    const auto temp27{A10*(temp1)};
    const auto temp28{A10*(temp4)};
    const auto temp29{A14*(temp8)};
    const auto temp30{A10*(temp3)};

    return A0*((temp11-A13*(temp2)+temp25)*A6-(temp12-A13*(temp6)+temp18)*A7+(temp13-A12*(temp6)+temp26)*A8-(temp14-A12*(temp5)+temp23)*A9)
           -A1*((temp11-A13*(temp2)+temp25)*A5-(temp15-A13*(temp10)+temp27)*A7+(temp16-A12*(temp10)+temp21)*A8-(temp17-A12*(temp9)+temp28)*A9)
           +A2*((temp18-A13*(temp6)+temp12)*A5-(temp15-A13*(temp10)+temp27)*A6+(temp19-A11*(temp10)+temp29)*A8-(temp20-A11*(temp9)+temp24)*A9)
           -A3*((temp13-A12*(temp6)+temp26)*A5-(temp21-A12*(temp10)+temp16)*A6+(temp19-A11*(temp10)+temp29)*A7-(temp22-A11*(temp7)+temp30)*A9)
           +A4*((temp23-A12*(temp5)+temp14)*A5-(temp17-A12*(temp9)+temp28)*A6+(temp24-A11*(temp9)+temp20)*A7-(temp22-A11*(temp7)+temp30)*A8);
}


template<typename T>
constexpr inline auto det_6_6(T const*const a)noexcept{

    const auto A0{a[0]};
    const auto A1{a[1]};
    const auto A2{a[2]};
    const auto A3{a[3]};
    const auto A4{a[4]};
    const auto A5{a[5]};

    const auto A6{a[6]};
    const auto A7{a[7]};
    const auto A8{a[8]};
    const auto A9{a[9]};
    const auto A10{a[10]};
    const auto A11{a[11]};

    const auto A12{a[12]};
    const auto A13{a[13]};
    const auto A14{a[14]};
    const auto A15{a[15]};
    const auto A16{a[16]};
    const auto A17{a[17]};

    const auto A18{a[18]};
    const auto A19{a[19]};
    const auto A20{a[20]};
    const auto A21{a[21]};
    const auto A22{a[22]};
    const auto A23{a[23]};

    const auto A24{a[24]};
    const auto A25{a[25]};
    const auto A26{a[26]};
    const auto A27{a[27]};
    const auto A28{a[28]};
    const auto A29{a[29]};

    const auto A30{a[30]};
    const auto A31{a[31]};
    const auto A32{a[32]};
    const auto A33{a[33]};
    const auto A34{a[34]};
    const auto A35{a[35]};

    const auto temp1{A26*A33-A27*A32};
    const auto temp2{A26*A34-A28*A32};
    const auto temp3{A27*A34-A28*A33};
    const auto temp4{A25*A33-A27*A31};
    const auto temp5{A25*A34-A28*A31};
    const auto temp6{A25*A32-A26*A31};
    const auto temp7{A26*A35-A29*A32};
    const auto temp8{A25*A35-A29*A31};
    const auto temp9{A27*A35-A29*A33};
    const auto temp10{A28*A35-A29*A34};
    const auto temp11{A24*A33-A27*A30};
    const auto temp12{A24*A34-A28*A30};
    const auto temp13{A24*A32-A26*A30};
    const auto temp14{A24*A35-A29*A30};
    const auto temp15{A24*A31-A25*A30};
    const auto temp46{A22*(temp1)-A21*(temp2)+A20*(temp3)};
    const auto temp47{A22*(temp4)-A21*(temp5)+A19*(temp3)};
    const auto temp48{A22*(temp6)-A20*(temp5)+A19*(temp2)};
    const auto temp49{A21*(temp6)-A20*(temp4)+A19*(temp1)};
    const auto temp50{A19*(temp7)-A20*(temp8)+A23*(temp6)};
    const auto temp51{A23*(temp4)-A21*(temp8)+A19*(temp9)};
    const auto temp52{A20*(temp9)-A21*(temp7)+A23*(temp1)};
    const auto temp53{A23*(temp3)-A22*(temp9)+A21*(temp10)};
    const auto temp54{A23*(temp2)-A22*(temp7)+A20*(temp10)};
    const auto temp55{A23*(temp1)-A21*(temp7)+A20*(temp9)};
    const auto temp56{A19*(temp9)-A21*(temp8)+A23*(temp4)};
    const auto temp57{A23*(temp5)-A22*(temp8)+A19*(temp10)};
    const auto temp58{A21*(temp10)-A22*(temp9)+A23*(temp3)};
    const auto temp59{A23*(temp6)-A20*(temp8)+A19*(temp7)};
    const auto temp60{A22*(temp11)-A21*(temp12)+A18*(temp3)};
    const auto temp61{A22*(temp13)-A20*(temp12)+A18*(temp2)};
    const auto temp62{A18*(temp9)-A21*(temp14)+A23*(temp11)};
    const auto temp63{A21*(temp13)-A20*(temp11)+A18*(temp1)};
    const auto temp64{A18*(temp7)-A20*(temp14)+A23*(temp13)};
    const auto temp65{A23*(temp11)-A21*(temp14)+A18*(temp9)};
    const auto temp66{A23*(temp12)-A22*(temp14)+A18*(temp10)};
    const auto temp67{A23*(temp13)-A20*(temp14)+A18*(temp7)};
    const auto temp68{A22*(temp15)-A19*(temp12)+A18*(temp5)};
    const auto temp69{A21*(temp15)-A19*(temp11)+A18*(temp4)};
    const auto temp70{A18*(temp8)-A19*(temp14)+A23*(temp15)};
    const auto temp71{A23*(temp15)-A19*(temp14)+A18*(temp8)};
    const auto temp72{A20*(temp15)-A19*(temp13)+A18*(temp6)};
    const auto temp73{A18*(temp2)-A20*(temp12)+A22*(temp13)};
    const auto temp74{A20*(temp3)-A21*(temp2)+A22*(temp1)};
    const auto temp75{A18*(temp5)-A19*(temp12)+A22*(temp15)};
    const auto temp76{A19*(temp2)-A20*(temp5)+A22*(temp6)};
    const auto temp77{A20*(temp10)-A22*(temp7)+A23*(temp2)};


    return A0*(A11*(A13*(temp46)-A14*(temp47)+A15*(temp48)-A16*(temp49))-A10*(-A17*(temp49)+A15*(temp50)-A14*(temp51)+A13*(temp52))+(A14*(temp53)-A15*(temp54)+A16*
           (temp55)-A17*(temp46))*A7-(-A17*(temp47)+A16*(temp56)-A15*(temp57)+A13*(temp58))*A8+(A13*(temp54)-A14*(temp57)+A16*(temp59)-A17*(temp48))*A9)-A1*(A11*(A12*
           (temp46)-A14*(temp60)+A15*(temp61)-A16*(temp63))-A10*(-A17*(temp63)+A15*(temp64)-A14*(temp65)+A12*(temp52))+(A14*(temp53)-A15*(temp54)+A16*(temp55)-A17*(temp46))*A6-(-A17*
           (temp60)+A16*(temp62)-A15*(temp66)+A12*(temp58))*A8+(A12*(temp54)-A14*(temp66)+A16*(temp67)-A17*(temp61))*A9)+A2*(A11*(A12*(temp47)-A13*(temp60)+A15*(temp68)-A16*(temp69))-A10*(-A17*
           (temp69)+A15*(temp70)-A13*(temp65)+A12*(temp56))+(A13*(temp53)-A15*(temp57)+A16*(temp51)-A17*(temp47))*A6-(-A17*(temp60)+A16*(temp62)-A15*(temp66)+A12*(temp58))*A7+(A12*
           (temp57)-A13*(temp66)+A16*(temp71)-A17*(temp68))*A9)+A4*(A11*(A12*(temp49)-A13*(temp63)+A14*(temp69)-A15*(temp72))+(A13*(temp55)-A14*(temp51)+A15*(temp59)-A17*(temp49))*A6-(-A17*
           (temp63)+A15*(temp64)-A14*(temp65)+A12*(temp52))*A7+(A12*(temp51)-A13*(temp65)+A15*(temp71)-A17*(temp69))*A8-(-A17*(temp72)+A14*(temp70)-A13*
           (temp67)+A12*(temp50))*A9)-A5*(A10*(A12*(temp49)-A13*(temp63)+A14*(temp69)-A15*(temp72))+(A13*(temp46)-A14*(temp47)+A15*
           (temp48)-A16*(temp49))*A6-(-A16*(temp63)+A15*(temp73)-A14*(temp60)+A12*(temp74))*A7+(A12*(temp47)-A13*(temp60)+A15*(temp68)-A16*(temp69))*A8-(-A16*
           (temp72)+A14*(temp75)-A13*(temp61)+A12*(temp76))*A9)-A3*(A11*(A12*(temp48)-A13*(temp61)+A14*(temp68)-A16*(temp72))-A10*(-A17*(temp72)+A14*(temp70)-A13*(temp67)+A12*(temp50))+(A13*
           (temp54)-A14*(temp57)+A16*(temp59)-A17*(temp48))*A6-(-A17*(temp61)+A16*(temp64)-A14*(temp66)+A12*(temp77))*A7+(A12*(temp57)-A13*(temp66)+A16*(temp71)-A17*(temp68))*A8);

}


} // NAMESPACE DETAIL

#endif // MATH_detail_H
