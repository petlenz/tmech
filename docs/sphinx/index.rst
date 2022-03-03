.. Copyright (c) 2021, Peter Lenz

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.


Introduction
------------

`tmech` is a C++ library meant for numerical analysis with higher-order
tensor expressions.

`tmech` provides

- an extensible expression system enabling **lazy evaluation**.
- tmech is a high quality tensor algebra library for the C++ language, aiming towards a good balance between speed and ease of use.
- Integer and floating point numbers are fully supported, complex numbers are supported only for basic arithmetic operations.
- Provides high-level syntax and functionality for easy use.
- Can be used for solid mechanics, fluid mechanics, ...
- an extensible expression system enabling **lazy broadcasting**.
- isotropic tensor functions.
- compile-time differentiation of scalar and tensorial expressions
- many more...


`tmech` requires a modern C++ compiler supporting C++17. The following C++
compilers are supported:

- On Unix platforms, gcc 4.9 or a recent version of Clang
- On Windows platforms not tested

Licensing
---------

This software is licensed under the Apache License, Version 2.0. See the LICENSE file
for details.

.. toctree::
   :caption: INSTALLATION
   :maxdepth: 2

   api/install
   api/changelog

.. toctree::
   :caption: USAGE
   :maxdepth: 2

   api/getting_started
   api/expression
   api/tensor_as_array

.. toctree::
   :caption: QUICK REFERENCE TMECH
   :maxdepth: 2

   api/tensor_functions
   api/numerical_diff
   api/compare_operators
   api/adaptor
   api/convert_to_voigt

.. toctree::
   :caption: QUICK REFERENCE SYMDIFF
   :maxdepth: 2
   
   
.. toctree::
   :caption: API REFERENCE TMECH
   :maxdepth: 2
   
   api/api_tmech
   
.. toctree::
   :caption: API REFERENCE SYMDIFF
   :maxdepth: 2
   
   api/api_symdiff
   
.. toctree::
   :caption: DEVELOPER ZONE
   :maxdepth: 2
   
   
..
	   api/multi_newton

