.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Tensor decompositions
=====================

``tmech`` provides tensor decompositions that expose not only the decomposition
factors but also their derivative tensors, which are required for consistent
linearization when constitutive models are formulated in terms of stretch or
rotation tensors.

Polar decomposition
-------------------

The polar decomposition factors a deformation gradient as

.. math::

    \mathbf{F} = \mathbf{R}\,\mathbf{U} = \mathbf{V}\,\mathbf{R}

into a rotation :math:`\mathbf{R}` and the right and left stretch tensors
:math:`\mathbf{U}` and :math:`\mathbf{V}`. It is computed via Newton iteration
or eigenvalue-based methods. The decomposition exposes the derivative tensors
:math:`\partial \mathbf{R}/\partial \mathbf{F}`,
:math:`\partial \mathbf{U}/\partial \mathbf{F}`, and
:math:`\partial \mathbf{V}/\partial \mathbf{F}`.

Eigenvalue decomposition
------------------------

Eigenvalue (spectral) decomposition is supported for 2D and 3D symmetric
tensors,

.. math::

    \mathbf{A} = \sum_{i} \lambda_i\, \mathbf{E}_i

exposing the eigenvalues :math:`\lambda_i`, eigenvectors, and the eigenbasis
:math:`\mathbf{E}_i`, together with their derivatives.
