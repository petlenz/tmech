.. _lazy-evaluation:


Expressions and lazy evaluation
===============================

`tmech` is based on metaprogramming technique `expression templates`.
It allows you to build expressions with no temporary evaluation.

Expressions
-----------

Assume ``a``, ``b`` and ``c`` are tensors of *same dimension and rank*, the return type of an expression such as ``a + b - c`` is **not an tensor**.
The result is an ``tensor_expression_wrapper`` which offers a basic interface but normally does not hold any value. 
Such expressions can be used to build more complex expressions:

.. code::

    auto f = a + b - c;
    auto f2 = d + 2 * f;

The expression engine avoids for basic aretmetics (+, -, outer products, basis arrangement, and scalar multiplication) the evaluation of intermediate 
results and their storage in temporary tensors. Assuming ``X``and ``Y`` are second order tensors with dimension ``dim`` and ``z`` is a scalar,

.. code::

    tmech::tensor<double, 3, 2> R = X + z*Y

will produce quite the same assembly as the following loop:

.. code::

    tmech::tensor<double, 3, 2> res;
    for(size_t i = 0; i < Dim; ++i){
    	for(size_t j = 0; j < Dim; ++j){
        	R(i, j) = X(i, j) + z*Y(i, j);
        }
    }

Lazy evaluation
---------------

An expression such as ``X + z*Y`` does not hold the result. This
allows to operate symbolically and only compute the result for the indices of interest:

.. code::

    auto F = X + Y;

    double first_res = F(0,0);
    double second_res = F(2,2);
    // Only two values have been computed

That means if you use the same expression in two assign statements, the computation of the expression will be done twice. Depending on the complexity of the computation
and the size of the data, it might be convenient to store the result of the expression in a temporary variable:

.. code::

    tmech::tensor<double, 2, 3> tmp = X + Y;
    tmech::tensor<double, 2, 3> res1 = tmp + 2 * X;
    tmech::tensor<double, 2, 3> res2 = tmp - 2 * X;

Aliasing: temporary evaluation
------------------------------ 

In the case where a tensor appears on the left and right side of the assignment operator attention is needed.
Statements like 

.. code::

	tmech::tensor<double, 3, 2> X;
	...
	X = 2*X;

or 

.. code::

	tmech::tensor<double, 3, 2> X;
	...
	X = tmech::trans(X);
	
or 

.. code::

	tmech::tensor<double, 3, 2> X;
	...
	X = X*X;
	
exhibit aliasing. In all three cases no temporary memory is used to form the reselt. 
This is harmless in the first example, but the second and third example leads to unexpected results. 
You have to force the evaluation of such an expression into a temporary result and then assign this result
to the finial `tensor`.
The simplest way to evaluate a temporary tensor is the function ``tmech::eval``

.. code::

	tmech::tensor<double, 3, 2> X;
	...
	X = tmech::eval(X*X);
	
	
Accessing elements
------------------

You can access the elements of any ``tensor_expression_wrapper`` with ``operator()``:

.. code::

    #include <tmech/tensor.h>

    tmech::tensor<double, 3, 2> A{1, 2, 3, 
                                  4, 5, 6
                                  7, 8, 9};
    auto fA = 2 * A;

    double d1 = A(0, 2);
    double d2 = fA(1, 2);
    
    tmech::tensor<double, 3, 3> B{11, 12, 13, 
                                  14, 15, 16,
                                  17, 18, 19,
                                  21, 22, 23, 
                                  24, 25, 26,
                                  27, 28, 29,
                                  31, 32, 33, 
                                  34, 35, 36,
                                  37, 38, 39};
                                     
    auto fB = 2 * B;

    double d1 = B(0, 2, 1);
    double d2 = fB(1, 2, 0);
    
    
The number of arguments to call ``operator()`` is always equal to the rank of the tensor.

Expression interface
--------------------

All ``tensor_base`` expressions in `tmech` provide the following interface:

Size
~~~~~

- ``dimension()`` returns the number of dimensions of the expression.
- ``rank()`` returns the rank of the expression.


Element access
~~~~~~~~~~~~~~

- ``operator()`` is an access operator. The number of arguments is always equal to the rank of the tensor expression.

