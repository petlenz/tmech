.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Comparison with related libraries
=================================

.. list-table::
   :header-rows: 1
   :widths: 30 14 14 14 14

   * - Feature
     - tmech
     - Eigen
     - FTensor
     - Fastor
   * - Header-only
     - Yes
     - Yes
     - Yes
     - Yes
   * - Arbitrary-rank tensors
     - Yes
     - Yes [1]_
     - Yes
     - Yes
   * - Expression templates
     - Yes
     - Yes
     - Yes
     - Yes
   * - Symbolic differentiation
     - Yes [2]_
     - No
     - No
     - No
   * - Isotropic tensor functions
     - Yes
     - No
     - No
     - No
   * - Decomposition derivatives
     - Yes
     - No
     - No
     - No
   * - SIMD support
     - Optional
     - Yes
     - No
     - Yes
   * - Index notation
     - Implicit
     - N/A
     - Yes
     - Yes

.. [1] Eigen provides arbitrary-rank tensors only through its unsupported
   ``Eigen::Tensor`` module.
.. [2] Experimental; see :doc:`symbolic_differentiation`.

The distinguishing feature of ``tmech`` is the integration of symbolic
differentiation with tensor algebra. All four libraries provide expression
templates for efficient evaluation; among them, only ``tmech`` also
differentiates tensor expressions symbolically to produce tangent operators at
compile time. General-purpose automatic-differentiation tools (such as Sacado,
CoDiPack, autodiff, and Enzyme) achieve differentiation by other means
(run-time tapes or external code generation).
