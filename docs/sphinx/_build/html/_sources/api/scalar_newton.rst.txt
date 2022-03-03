Nonlinear functions
===================


How to use newton-raphson
-------------------------

.. code::

    #include <iostream>
    #include <tmech.h>

    int main()
    {
    	//Start point
        //auto data{std::make_tuple(1.0, 1.0, 1.0)};
        std::array<double, 3> data{1,2,3};

        //constants
        symdiff::real<double, 1,   0, 1> _1;
        symdiff::real<double, 1,   5, 1> _1_5;
        symdiff::real<double, 2,   0, 1> _2;
        symdiff::real<double, 3,   0, 1> _3;
        symdiff::real<double, 4,   0, 1> _4;
        symdiff::real<double, 625, 0, 3> _625;
        symdiff::real<double, 20,  0, 2> _20;
        symdiff::real<double, 9,   0, 1> _9;

        //functions
        auto f1 = x*x - _2*x + y*y - z + _1;
        auto f2 = x*y*y - x - _3*y + y*z + _2;
        auto f3 = x*z*z - _3*z + y*z*z + x*y;

        //vector
        auto f = symdiff::make_vector(f1, f2, f3);

        //jacobian
        auto J = symdiff::make_jacobi_matrix(f, x, y, z);

        //newton solver
        auto solver = symdiff::newton_solver(J, f);

        //solve for given start point
        solver.solve(data);

        //root
        std::cout<<"Solution should be"<<std::endl;
        std::cout<<1<<std::endl;
        std::cout<<1<<std::endl;
        std::cout<<1<<std::endl;

        std::cout<<"Solution given by Newton-Raphson method"<<std::endl;
        //for std array
        std::cout<<data[0]<<std::endl;
        std::cout<<data[1]<<std::endl;
        std::cout<<data[2]<<std::endl;
        std::cout<<std::endl;

        //change start point
        data[0] = data[1] = data[2] = 0;

        //solve for given start point
        solver.solve(data);

        //root
        std::cout<<"Solution should be"<<std::endl;
        std::cout<<1.09894<<std::endl;
        std::cout<<0.367617<<std::endl;
        std::cout<<0.144932<<std::endl;

        std::cout<<"Solution given by Newton-Raphson method"<<std::endl;
        //for std array
        std::cout<<data[0]<<std::endl;
        std::cout<<data[1]<<std::endl;
        std::cout<<data[2]<<std::endl;
        std::cout<<std::endl;
        
        return 0;
    }

Outputs:

.. code::

    9.0

