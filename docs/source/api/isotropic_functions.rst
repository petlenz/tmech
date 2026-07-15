.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Isotropic tensor functions
==========================

``tmech`` implements isotropic tensor functions -- ``exp``, ``log``, ``sqrt``,
``pow``, and positive/negative part operators -- via spectral decomposition
(Miehe 1993; Miehe and Lambrecht 2001). Given a symmetric rank-2 tensor
:math:`\mathbf{A}`, the library computes its eigenvalues :math:`\lambda_i` and
eigenvectors :math:`\mathbf{n}_i`, applies the scalar function to each
eigenvalue, and reconstructs the tensor result:

.. math::

    f(\mathbf{A}) = \sum_{i} f(\lambda_i)\, \mathbf{n}_i \otimes \mathbf{n}_i

First and second derivatives are computed analytically using the eigenbasis,
with proper treatment of repeated eigenvalues to avoid singularities. These
functions integrate with the :doc:`symbolic differentiation
<symbolic_differentiation>` module, enabling automatic tangent computation for
models involving tensor logarithms or exponentials.
