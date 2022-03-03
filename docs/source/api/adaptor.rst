Adapting pointers of raw memory
===============================

The class `adaptor` can adapt one-dimensional raw memory pointers in place, and provide them a tensor interface.
Different memory layouts are implemented for second and fourth order tensors

- Voigt's notation for second and fourth order tensors
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
