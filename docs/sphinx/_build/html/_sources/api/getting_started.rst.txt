Getting started
===============

This guide explains how to get started with `tmech` after installing it 
using the methods described in the installation section.



First example
-------------
.. code::

    #include <iostream>
    #include <tmech/tmech.h>

    int main()
    {
        constexpr std::size_t Rank{2}, Dim{3};
        tmech::tensor<double, Dim, Rank> A{1, 2, 3,
					   4, 5, 6,
           				   7, 8, 9};

        tmech::tensor<double, Dim, Rank> B{9, 8, 7,
        				   6, 5, 4,
           			 	   3, 2, 1};

        tmech::tensor<double, Dim, Rank> res = A + B;

        std::cout << res << std::endl;

        return 0;
    }

This example simply adds two second order tensors.




Compiling the first example
---------------------------

`tmech` is a header-only library. You have to tell the compiler where to find `tmech`.
For example with g++, use the ``-I`` option to achieve this. Assuming the first example code is 
located in ``first_example.cpp``, the compilation command is:

.. code:: bash

    g++ -std=c++17 -I /path/to/tmech/ first_example.cpp -o first_example


Starting the program, produces the following output:

.. code::

   10 10 10
   10 10 10
   10 10 10

Building with cmake
-------------------

A better way for building programs using `tmech` is to use `cmake`. 
Assuming the following folder structure:

.. code:: bash

    first_example
       |- src
       |   |- first_example.cpp
       |- CMakeLists.txt

The following minimal ``CMakeLists.txt`` is enough to build the first example:

.. code:: cmake

    cmake_minimum_required(VERSION 3.1)
    set(CMAKE_CXX_STANDARD 17)
    project(first_example)

    find_package(tmech REQUIRED)

    add_executable(first_example src/first_example.cpp)

    if(MSVC)
        set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
    endif()

    target_link_libraries(first_example tmech)


`cmake` has to know where to find the headers, this is done through the ``CMAKE_INSTALL_PREFIX``
variable. Note that ``CMAKE_INSTALL_PREFIX`` is usually the path to a folder containing the following
subfolders: ``include``, ``lib`` and ``bin``, so you don't have to pass any additional option for linking.
Examples of valid values for ``CMAKE_INSTALL_PREFIX`` on Unix platforms are ``/usr/local``, ``/opt``.

The following commands create a directory for building, builds
the first example with cmake and then runs the program:

.. code:: bash

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=your_prefix ..
    make
    ./first_program


Second example: single contraction
----------------------------------

This second example single contracts two second order tensors:

.. code::

    #include <iostream>
    #include <tmech/tmech.h>

    int main()
    {
        constexpr std::size_t Rank{2}, Dim{3};
        tmech::tensor<double, Dim, Rank> A{1, 2, 3,
					   4, 5, 6,
           				   7, 8, 9};

        tmech::tensor<double, Dim, Rank> B{9, 8, 7,
        				   6, 5, 4,
           			 	   3, 2, 1};

        std::cout << A*B << std::endl;
        return 0;
    }

When compiled and run, this produces the following output:

.. code::

    3.000000e+01  2.400000e+01  1.800000e+01 
    8.400000e+01  6.900000e+01  5.400000e+01 
    1.380000e+02  1.140000e+02  9.000000e+01


Third example: index access
---------------------------

.. code::

    #include <iostream>
    #include <tmech/tmech.h>

    int main()
    {
        constexpr std::size_t Rank{2}, Dim{3};
        tmech::tensor<double, Dim, Rank> A{1, 2, 3,
					   4, 5, 6,
           				   7, 8, 9};

        std::cout << A(2,2) << std::endl;
        return 0;
    }

Outputs:

.. code::

    9.0

