.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Symbolic differentiation
========================

.. note::

   The ``symdiff`` module is an **experimental** component of the library. The
   tensor-algebra engine is production-tested, whereas ``symdiff`` is under
   active development and its API may change.

The ``symdiff`` module (around 70 header files) extends the expression-template
concept to symbolic algebra. Users declare symbolic tensor variables, build
expressions from them, and invoke differentiation to obtain exact derivative
expressions. Higher-order derivatives are obtained by repeated application. The
module includes symbolic representations of scalar and tensorial constants,
binary operations, standard mathematical functions, and tensor-specific
operations (contractions, outer products, basis changes), together with
Newton--Raphson solvers built on the symbolic Jacobian.

Minimal example
---------------

Differentiating a scalar-valued function of a tensor variable (the determinant,
whose derivative is the cofactor):

.. code-block:: cpp

    using T2 = tmech::tensor<double,3,2>;
    symdiff::variable<T2, 0> x;

    auto f  = tmech::det(x);              // scalar-valued function of a tensor
    auto df = symdiff::derivative(f, x);  // closed-form derivative: cof(x)

Tangent stiffness of a linear elastic model
-------------------------------------------

A more representative use case is the automatic derivation of the tangent
stiffness tensor for a linear elastic constitutive model:

.. code-block:: cpp

    using T2 = tmech::tensor<double,3,2>;
    symdiff::variable<T2, 0> eps;
    symdiff::constant<double, 0> lambda(150, "lambda");
    symdiff::constant<double, 1> mu(200, "mu");
    symdiff::real<double, 2, 0, 1> two;   // symbolic scalar 2
    symdiff::constant<T2, 2> I("I");
    I = tmech::eye<double,3,2>();

    // Stress: sigma = lambda tr(eps) I + 2 mu eps
    auto sig = lambda * tmech::trace(tmech::as_sym(eps)) * I
             + two * mu * tmech::as_sym(eps);

    // Tangent: C = d(sigma)/d(eps) -- derived automatically
    auto C = symdiff::derivative(sig, eps);

Both examples are compiled as part of the example suite; see
``examples/symdiff/`` in the repository.
