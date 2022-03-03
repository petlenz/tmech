#ifndef TST_SYMDIF_TEST_H
#define TST_SYMDIF_TEST_H

#include <gtest/gtest.h>
#include <tmech/tmech.h>

using namespace testing;
using namespace symdiff;

using value_type = double;

TEST(adCase, productRuleScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{x*x};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), 10);
    //EXPECT_EQ(df.get_string(), "(2.000000*var0)");
}

TEST(adCase, sumRuleScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{x+x};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), 2);
    //EXPECT_EQ(df.get_string(), "2.000000");
}

TEST(adCase, minRuleScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{x-x};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), 0);
    //EXPECT_EQ(df.get_string(), "0.000000");
}

TEST(adCase, expVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::exp(x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), std::exp(std::get<0>(f_var)));
    //EXPECT_EQ(df.get_string(), "exp(var0)");
}

TEST(adCase, expFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::exp(x*x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), 2*std::get<0>(f_var)*std::exp(std::get<0>(f_var)*std::get<0>(f_var)));
    //EXPECT_EQ(df.get_string(), "(exp((var0*var0))*(2.000000*var0))");
}

TEST(adCase, logVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::log(x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), 1.0/std::get<0>(f_var));
    //EXPECT_EQ(df.get_string(), "(1.000000/var0)");
}

TEST(adCase, logFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::log(x*x+x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (((2.0*std::get<0>(f_var))+1.0)/((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))));
    //EXPECT_EQ(df.get_string(), "(((2.000000*var0)+1.000000)/((var0*var0)+var0))");
}

TEST(adCase, sqrtVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::sqrt(x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (1.0/(2.0*sqrt(std::get<0>(f_var)))));
    //EXPECT_EQ(df.get_string(), "(1.000000/(2.000000*sqrt(var0)))");
}

TEST(adCase, sqrtFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::sqrt(x*x+x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (((2.000000*std::get<0>(f_var))+1.000000)/(2.000000*sqrt(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))));
    //EXPECT_EQ(df.get_string(), "(((2.000000*var0)+1.000000)/(2.000000*sqrt(((var0*var0)+var0))))");
}

TEST(adCase, absVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::abs(x)};
    auto f_var{std::make_tuple(-5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), std::get<0>(f_var)/std::abs(std::get<0>(f_var)));
    //EXPECT_EQ(df.get_string(), "sign(var0)");
}

TEST(adCase, absFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::abs(x*x+x)};
    auto f_var{std::make_tuple(-5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), ((((2.*std::get<0>(f_var))+1.)*((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))/abs(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))));
    //EXPECT_EQ(df.get_string(), "(((2.000000*var0)+1.000000)*sign(((var0*var0)+var0)))");
}

TEST(adCase, cosVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::cos(x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), -std::sin(std::get<0>(f_var)));
    //EXPECT_EQ(df.get_string(), "-(sin(var0))");
}

TEST(adCase, cosFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::cos(x*x+x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), -((((2.*std::get<0>(f_var))+1.)*sin(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))));
    //EXPECT_EQ(df.get_string(), "-((((2.000000*var0)+1.000000)*sin(((var0*var0)+var0))))");
}

TEST(adCase, sinVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::sin(x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), std::cos(std::get<0>(f_var)));
    //EXPECT_EQ(df.get_string(), "cos(var0)");
}

TEST(adCase, sinFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::sin(x*x+x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), ((((2.*std::get<0>(f_var))+1.)*cos(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))));
    //EXPECT_EQ(df.get_string(), "(((2.000000*var0)+1.000000)*cos(((var0*var0)+var0)))");
}

TEST(adCase, tanVarScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::tan(x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), std::pow(1./std::cos(std::get<0>(f_var)), 2.));
    //EXPECT_EQ(df.get_string(), "pow(sec(var0), 2.000000)");
}

TEST(adCase, tanFuncScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::tan(x*x+x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (((2.*std::get<0>(f_var))+1.)*std::pow(1.0/std::cos(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))), 2.)));
    //EXPECT_EQ(df.get_string(), "(((2.000000*var0)+1.000000)*pow(sec(((var0*var0)+var0)), 2.000000))");
}


TEST(adCase, powVaraScalar)
{
    symdiff::variable<value_type, 0> x;
    symdiff::constant<double, 1> c(2,"c");
    auto f{std::pow(x, c)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), 2.0*std::get<0>(f_var));
    //EXPECT_EQ(df.get_string(), "(pow(var0, (C1-1.000000))*C1)");
}

TEST(adCase, powFuncaScalar)
{
    symdiff::variable<value_type, 0> x;
    symdiff::constant<value_type, 1> c;
    auto f{std::pow(x*x+x, c)};
    auto f_var{std::make_tuple(-.5, 2)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (2*((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))*((2.*std::get<0>(f_var))+1.));
    //EXPECT_EQ(df.get_string(), "(pow(((var0*var0)+var0), (C1-1.000000))*(C1*((2.000000*var0)+1.000000)))");
}

TEST(adCase, powVarbScalar)
{
    symdiff::variable<value_type, 0> x;
    symdiff::constant<value_type, 1> c(2,"c");
    auto f{std::pow(c, x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), std::pow(2.,std::get<0>(f_var))*std::log(2.0));
    //EXPECT_EQ(df.get_string(), "(pow(C1, (var0-1.000000))*(log(C1)*C1))");
}

TEST(adCase, powFuncbScalar)
{
    symdiff::variable<value_type, 0> x;
    symdiff::constant<value_type, 1> c(2,"c");
    auto f{std::pow(c, x*x+x)};
    auto f_var{std::make_tuple(-.5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (std::pow(2.0, std::get<0>(f_var)*std::get<0>(f_var)+std::get<0>(f_var))*((((2.*std::get<0>(f_var))+1.)*log(2)))));
    //EXPECT_EQ(df.get_string(), "(pow(C1, (((var0*var0)+var0)-1.000000))*((((2.000000*var0)+1.000000)*log(C1))*C1))");
}

TEST(adCase, powVarcScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::pow(x, x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), std::pow(std::get<0>(f_var), (std::get<0>(f_var)-1))*((std::log(std::get<0>(f_var))*std::get<0>(f_var))+std::get<0>(f_var)));
    //EXPECT_EQ(df.get_string(), "(pow(var0, (var0-1.000000))*((log(var0)*var0)+var0))");
}

TEST(adCase, powFunccScalar)
{
    symdiff::variable<value_type, 0> x;
    auto f{std::pow(x*x+x, x*x+x)};
    auto f_var{std::make_tuple(5)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (std::pow(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)), (((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))-1))*(((((2.*std::get<0>(f_var))+1.)*std::log(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))))*((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))+(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var))*((2.*std::get<0>(f_var))+1.)))));
    //EXPECT_EQ(df.get_string(), "(pow(((var0*var0)+var0), (((var0*var0)+var0)-1.000000))*(((((2.000000*var0)+1.000000)*log(((var0*var0)+var0)))*((var0*var0)+var0))+(((var0*var0)+var0)*((2.000000*var0)+1.000000))))");
}

using tensor2 = tmech::tensor<value_type, 3, 2>;
using tensor4 = tmech::tensor<value_type, 3, 4>;
TEST(adCase, productRuleTensor)
{
    tmech::tensor<value_type, 3, 2> a;
    symdiff::variable<tensor2, 0> x;
    a.randn();
    tmech::eye<double,3,2> I;
    auto II = tmech::otimesu(I,I);
    auto f{x*x};
    auto f_var{std::make_tuple(a)};
    auto df = derivative<1>(f, x);
    EXPECT_EQ(df(f_var), (tmech::inner_product<tmech::sequence<2>,tmech::sequence<1>>(tmech::otimesu(I, I), x(f_var))
                          +tmech::inner_product<tmech::sequence<2>,tmech::sequence<1>>(x(f_var), tmech::otimesu(I, I))));
    //EXPECT_EQ(df.get_string(), "((II*var0)+(var0*II))");
}

//TEST(adCase, sumRuleTensor)
//{
//    tensor2 a;
//    a.randn();
//    IIt II;
//    variable<tensor2, 0> x;
//    auto f{x+x};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), 2*II);
//    EXPECT_EQ(df.get_string(), "(2.000000*II)");
//}

//TEST(adCase, minRuleTensor)
//{
//    tensor2 a;
//    a.randn();
//    IIt II;
//    variable<tensor2, 0> x;
//    auto f{x-x};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), (II - II));
//    EXPECT_EQ(df.get_string(), std::string("0"));
//}

//TEST(adCase, traceTensorVar)
//{
//    tensor2 a;
//    a.randn();
//    It I;
//    variable<tensor2, 0> x;
//    auto f{math::trace(x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    const bool val{(df(f_var) == I)};
//    EXPECT_EQ(true, val);
//    EXPECT_EQ(df.get_string(), "I");
//}

//TEST(adCase, traceTensorFunc)
//{
//    tensor2 a;
//    a.randn();
//    It I;
//    IIt II;
//    variable<tensor2, 0> x;
//    auto f{math::trace(x*x+x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), tmech::dcontractoi((((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II),I));
//    EXPECT_EQ(df.print(), "dcontractio(I,(((II*var0)+(var0*II))+II))");
//}

//TEST(adCase, invTensorVar)
//{
//    tensor2 a;
//    a.randn();
//    IIt II;
//    variable<tensor2, 0> x;
//    auto f{math::inv(x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), -(tmech::otimes(tmech::inv(std::get<0>(f_var)),tmech::inv(std::get<0>(f_var)))));
//    EXPECT_EQ(df.print(), "-(otimes(inv(var0),inv(var0)))");
//}

//TEST(adCase, invTensorFunc)
//{
//    tensor2 a;
//    a.randn();
//    IIt II;
//    It I;
//    variable<tensor2, 0> x;
//    auto f{math::inv(x*x+x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), -((tmech::inv(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))*((((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II)*tmech::inv(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))))));
//    EXPECT_EQ(df.print(), "dcontractio(-(otimes(inv(((var0*var0)+var0)),inv(((var0*var0)+var0)))),(((II*var0)+(var0*II))+II))");
//}

//TEST(adCase, detTensorVar)
//{
//    tensor2 a;
//    a.randn();
//    variable<tensor2, 0> x;
//    auto f{math::det(x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), tmech::det(std::get<0>(f_var))*tmech::trans(tmech::inv(std::get<0>(f_var))));
//    EXPECT_EQ(df.get_string(), "(det(var0)*trans(inv(var0)))");
//}



//TEST(adCase, powTensorVar)
//{
//    tensor2 a;
//    tensor4 a_dif;
//    a.randn();
//    variable<tensor2, 0> x;
//    constant<int,1> c;
//    IIt II;
//    auto f{math::pow(x,c)};
//    auto f_var{std::make_tuple(std::ref(a),2)};
//    f.update(f_var);
//    auto df = derivative<1>(f, x);

//    const int n{c(f_var)};
//    for(int i{0}; i<n; ++i){
//        const auto r{(n-1)-i};
//        if(i == 0){
//            a_dif += tmech::otimes(tmech::eye_tensor<value_type, tensor2::dimension(), 2>(),tmech::pow(a,r));
//        }else if (r == 0) {
//            a_dif += tmech::otimes(tmech::pow(a,i),tmech::eye_tensor<value_type, tensor2::dimension(), 2>());
//        }else{
//            a_dif += tmech::otimes(tmech::pow(a,i),tmech::pow(a,r));
//        }
//    }
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), a_dif);
//    EXPECT_EQ(df.print(), "pow(var0,C1)'");
//}

//TEST(adCase, powTensorFunc)
//{
//    tensor2 a;
//    tensor4 a_dif;
//    a.randn();
//    variable<tensor2, 0> x;
//    constant<int,1> c;
//    IIt II;
//    auto f{math::pow(x*x+x,c)};
//    auto f_var{std::make_tuple(std::ref(a),2)};
//    f.update(f_var);
//    auto df = derivative<1>(f, x);

//    const int n{c(f_var)};
//    for(int i{0}; i<n; ++i){
//        const auto r{(n-1)-i};
//        if(i == 0){
//            a_dif += tmech::otimes(tmech::eye_tensor<value_type, tensor2::dimension(), 2>(),tmech::pow((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var),r));
//        }else if (r == 0) {
//            a_dif += tmech::otimes(tmech::pow((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var),i),tmech::eye_tensor<value_type, tensor2::dimension(), 2>());
//        }else{
//            a_dif += tmech::otimes(tmech::pow((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var),i),tmech::pow((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var),r));
//        }
//    }
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), tmech::dcontractio(a_dif,(((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II)));
//    EXPECT_EQ(df.print(), "dcontractio(pow(((var0*var0)+var0),C1)',(((II*var0)+(var0*II))+II))");
//}

//TEST(adCase, devTensorVar)
//{
//    tensor2 a;
//    tensor4 a_dif;
//    a.randn();
//    variable<tensor2, 0> x;
//    It I;
//    IIt II;
//    auto f{math::dev(x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), (II-((1./tensor2::dimension())*tmech::times(I,I))));
//    EXPECT_EQ(df.get_string(), "(otimes(I,I)-(0.333333*times(I,I)))");
//}

//TEST(adCase, devTensorFunc)
//{
//    tensor2 a;
//    tensor4 a_dif;
//    a.randn();
//    variable<tensor2, 0> x;
//    It I;
//    IIt II;
//    auto f{math::dev(x*x+x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    df.update(f_var);
//    EXPECT_EQ(df(f_var), tmech::dcontractio((tmech::otimes(I,I)-((1./tensor2::dimension())*tmech::times(I,I))),(((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II)));
//    EXPECT_EQ(df.print(), "dcontractio((otimes(I,I)-(0.333333*times(I,I))),(((II*var0)+(var0*II))+II))");
//}

//TEST(adCase, volTensorVar)
//{
//    tensor2 a;
//    tensor4 a_dif;
//    a.randn();
//    variable<tensor2, 0> x;
//    It I;
//    auto f{math::vol(x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), ((1./tensor2::dimension())*tmech::times(I,I)));
//    EXPECT_EQ(df.get_string(), "(0.333333*times(I,I))");
//}

//TEST(adCase, volTensorFunc)
//{
//    tensor2 a;
//    tensor4 a_dif;
//    a.randn();
//    variable<tensor2, 0> x;
//    It I;
//    IIt II;
//    auto f{math::vol(x*x+x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), tmech::dcontractio(((1./tensor2::dimension())*tmech::times(I,I)),(((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II)));
//    EXPECT_EQ(df.print(), "dcontractio((0.333333*times(I,I)),(((II*var0)+(var0*II))+II))");
//}

//TEST(adCase, sqrt_det_tensor)
//{
//    tensor2 a;
//    a.randn();
//    variable<tensor2, 0> x;
//    constant<double, 1> c;
//    auto f{pow(math::det(x),c)};
//    auto f_var{std::make_tuple(std::ref(a),2)};
//    auto df = derivative<1>(f, x);
//    df.update(f_var);
//    //EXPECT_EQ(df(f_var), tmech::det(std::get<0>(f_var))*tmech::trans(tmech::inv(std::get<0>(f_var))));
//    //EXPECT_EQ(df.get_string(), "(det(var0)*trans(inv(var0)))");
//}

//TEST(adCase, test)
//{
//    tensor2 a{2.71217, 0.00853142, 0.702217,
//              0.00853142, -0.653445, 0.358802,
//              0.702217, 0.358802, 0.329014};
//    tmech::tensor<double, 3, 1> a_eigval;

//    //a.randn();
//    variable<tensor2, 0> x;
//    constant<double, 1> c;
//    auto f{(x + math::inv(x))*c};
//    auto f_var{std::make_tuple(std::ref(a),0.5)};
//    auto df = derivative<1>(f, x);
//    //a = tmech::eval(tmech::sym(a));
//    std::cout<<a<<std::endl;
//    tmech::eig(a, a_eigval);
//    std::cout<<a_eigval<<std::endl;
//    std::cout<<a<<std::endl;
//    std::cout<<tmech::sqrt(a)<<std::endl;

//    //std::cout<<tmech::exp(a)<<std::endl;
//    //EXPECT_EQ(df(f_var), tmech::det(std::get<0>(f_var))*tmech::trans(tmech::inv(std::get<0>(f_var))));
//    //EXPECT_EQ(df.get_string(), "(det(var0)*trans(inv(var0)))");
//}
















//TEST(adCase, absTensorVar)
//{
//    tensor2 a;
//    a.randn();
//    variable<tensor2, 0> x;
//    IIt II;
//    auto f{math::abs(x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), II*tmech::sign(std::get<0>(f_var)));
//    EXPECT_EQ(df.get_string(), "(II*sign(var0))");
//}

//TEST(adCase, absTensorFunc)
//{
//    tensor2 a;
//    a.randn();
//    variable<tensor2, 0> x;
//    IIt II;
//    auto f{math::abs(x*x+x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    EXPECT_EQ(df(f_var), ((((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II)*tmech::sign(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))));
//    EXPECT_EQ(df.get_string(), "dcontractio((II*sign(((var0*var0)+var0))),(((II*var0)+(var0*II))+II))");
//}

//TEST(adCase, sqrt_det_tensor2)
//{
//    tensor2 a;
//    a.randn();
//    variable<tensor2, 0> x;
//    auto f{sqrt(math::det(x))*x};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    //EXPECT_EQ(df(f_var), tmech::det(std::get<0>(f_var))*tmech::trans(tmech::inv(std::get<0>(f_var))));
//    EXPECT_EQ(df.get_string(), "(det(var0)*trans(inv(var0)))");
//}

//TEST(adCase, invTensorFunc2)
//{
//    tmech::tensor<value_type,3,3,3,3> a;
//    a.randn();
//    const auto II{tmech::eye_tensor<value_type,3,3,3,3,3,3,3,3>()};
//    variable<tmech::tensor<value_type,3,3,3,3>, 0> x;
//    auto f{inv(x*x+x)};
//    auto f_var{std::make_tuple(std::ref(a))};
//    auto df = derivative<1>(f, x);
//    const bool val{(df(f_var) == -((inv(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))*((((II*std::get<0>(f_var))+(std::get<0>(f_var)*II))+II)*inv(((std::get<0>(f_var)*std::get<0>(f_var))+std::get<0>(f_var)))))))};
//    EXPECT_EQ(true, val);
//    const std::string temp{df.get_string()};
//    EXPECT_EQ(temp, "-((inv(((var0*var0)+var0))*((((I8*var0)+(var0*I8))+I8)*inv(((var0*var0)+var0)))))");
//}


#endif // TST_SYMDIF_TEST_H
