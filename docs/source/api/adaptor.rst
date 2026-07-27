.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Adapting pointers of raw memory
===============================

The class `adaptor` can adapt one-dimensional raw memory pointers in place, and provide them a tensor interface.
Different memory layouts are implemented for second and fourth order tensors

- Voigt's notation for second and fourth order tensors
- Mandel's notation for second and fourth order tensors
- Full representation for second and fourth order tensors

Full functionality of all available functions remains.

Adapting raw pointers Voigt's notation
--------------------------------------

Without shear strains
^^^^^^^^^^^^^^^^^^^^^ 
.. code::

    #include <tmech/tmech.hpp>

    int main()
    {
        //using voigt's notation
        constexpr std::size_t Rank = 2;
        constexpr std::size_t Dim = 3;
        double ptr[6]{1,2,3,4,5,6};
        tmech::adaptor<double, Dim, Rank, tmech::voigt<Dim>> a(data);
        tmech::tensor<double, Dim, Rank> b{1,6,5, 6,2,4, 5,4,3};
        std::cout<<std::boolalpha<<(a==b)<<std::endl;
    }

Running the program, produces the following output:

.. code::

    true

Shear strains
^^^^^^^^^^^^^
Shear strains are multiplied with 0.5

.. code::

    #include <tmech/tmech.hpp>

    int main()
    {
        //using voigt's notation
        constexpr std::size_t Rank = 2;
        constexpr std::size_t Dim = 3;
        double ptr[6]{1,2,3,4*2,5*2,6*2};
        tmech::adaptor<double, Dim, Rank, tmech::voigt<Dim,true>> a(data);
        tmech::tensor<double, Dim, Rank> b{1,6,5, 6,2,4, 5,4,3};
        std::cout<<std::boolalpha<<(a==b)<<std::endl;
    }

Running the program, produces the following output:

.. code::

    true

Adapting raw pointers Mandel's notation
---------------------------------------

Mandel's notation uses the same component ordering as Voigt, but scales the
off-diagonal slots by :math:`\sqrt{2}` (rank 2) and the shear--shear blocks by
:math:`2` (rank 4). This makes the packing an *isometry*, so a double
contraction becomes a plain dot product
(:math:`A:B = \mathrm{mandel}(A)\cdot\mathrm{mandel}(B)`) and a rank-4 action
becomes a matrix--vector product. A dense linear solve over the packed storage
is therefore equivalent to the tensor system --- which is what Voigt cannot
provide. There is deliberately no shear-strain option.

.. code::

    #include <tmech/tmech.hpp>

    int main()
    {
        //using mandel's notation {11, 22, 33, √2·23, √2·13, √2·12}
        constexpr std::size_t Rank = 2;
        constexpr std::size_t Dim = 3;
        const double s = std::sqrt(2.0);
        double ptr[6]{1,2,3, s*4, s*5, s*6};
        tmech::adaptor<double, Dim, Rank, tmech::mandel<Dim>> a(ptr);
        tmech::tensor<double, Dim, Rank> b{1,6,5, 6,2,4, 5,4,3};
        std::cout<<std::boolalpha<<(a==b)<<std::endl;
    }

Running the program, produces the following output:

.. code::

    true

Adapting raw pointers full representation
-----------------------------------------

.. code::

    #include <tmech/tmech.hpp>

    int main()
    {
        constexpr std::size_t Rank = 2;
        constexpr std::size_t Dim = 3;
        double ptr[9]{11,12,13,21,22,23,31,32,33};
        tmech::adaptor<double, Dim, Rank, tmech::full<Dim>> a(data);
        tmech::tensor<double, Dim, Rank> b{11,12,13,21,22,23,31,32,33};
        std::cout<<std::boolalpha<<(a==b)<<std::endl;
    }

Running the program, produces the following output:

.. code::

    true
