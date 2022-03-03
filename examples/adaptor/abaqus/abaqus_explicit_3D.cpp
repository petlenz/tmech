#include <iostream>
#include <tmech/tmech.h>

int main() {
    //Abaqus Explicite 3D
    double ptrC[36];
    double ptrSig[6];
    double ptrEps[6]{1,1,1,1,1,0.5};

    const double K{12000}, G{200};
    tmech::eye<double, 3, 2> I;
    const auto IIsym{(tmech::otimesu(I,I) + tmech::otimesl(I,I))*0.5};
    const auto IIvol{tmech::otimes(I, I)/3.};
    const auto IIdev{IIsym - IIvol};

    tmech::adaptor<double, 3, 4, tmech::abq_exp<3>> C(ptrC);
    tmech::adaptor<double, 3, 2, tmech::abq_exp<3>> sig(ptrSig);
    tmech::adaptor<double, 3, 2, tmech::abq_exp<3,true>> eps(ptrEps);

    C = 3*K*IIvol + 2*G*IIdev;
    sig = tmech::dcontract(C, eps);

    tmech::tensor<double, 3, 4> C_;
    tmech::tensor<double, 3, 2> sig_, eps_{1,0.5,0.25,
                                           0.5,1,0.5,
                                           0.25,0.5,1};
    C_ = 3*K*IIvol + 2*G*IIdev;
    sig_ = tmech::dcontract(C_, eps_);

    std::cout<<sig-sig_<<std::endl;

    assert(sig == sig_);
    return 0;
}
