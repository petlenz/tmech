Basic arithmetic
================

Addition
^^^^^^^^

.. _operator_add-func-ref:
.. doxygenfunction:: operator+(_TensorLHS &&, _TensorRHS &&)
   :project: tmech

Subtraction
^^^^^^^^^^^

.. _operator_min-func-ref:
.. doxygenfunction:: operator-(_TensorLHS &&, _TensorRHS &&)
   :project: tmech

Single contraction
^^^^^^^^^^^^^^^^^^

.. _operator_mul-func-ref:
.. doxygenfunction:: operator*(_TensorLHS &&, _TensorRHS &&)
   :project: tmech
   
Scalar update
^^^^^^^^^^^^^
 
.. _operator_scalar_mul-func-ref:
.. doxygenfunction:: operator*(_Scalar &&, _Tensor &&)
   :project: tmech

.. _operator_scalar_dic-func-ref:
.. doxygenfunction:: operator/(_Tensor &&, _Scalar &&)
   :project: tmech
   
Basis rearrangement
===================

General basis rearrangement
^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
.. _basis_change-func-ref:
.. doxygenfunction:: tmech::basis_change(_Tensor &&)
   :project: tmech

.. _basis_change-func-ref:
.. doxygenfunction:: basis_change(_Tensor &&)
   :project: tmech
Transposition
^^^^^^^^^^^^^
.. _trans-func-ref:
.. doxygenfunction:: trans(_Tensor &&)
   :project: tmech
   
Left basis transposition
^^^^^^^^^^^^^^^^^^^^^^^^ 
.. _transl-func-ref:
.. doxygenfunction:: transl(_Tensor &&)
   :project: tmech

Outer product
=============
General outer product
^^^^^^^^^^^^^^^^^^^^^ 
.. _outer_product-func-ref:
.. doxygenfunction:: outer_product(_TensorLHS &&, _TensorRHS &&)
   :project: tmech

Dyadic product
^^^^^^^^^^^^^^ 
.. _otimes-func-ref:
.. doxygenfunction:: tmech::otimes(_TensorLHS &&, _TensorRHS &&);
   :project: tmech
   
Lower dyadic product
^^^^^^^^^^^^^^^^^^^^ 
.. _otimesl-func-ref:
.. doxygenfunction:: otimesl(_TensorLHS &&, _TensorRHS &&);
   :project: tmech

Upper dyadic product
^^^^^^^^^^^^^^^^^^^^ 
.. _otimesu-func-ref:
.. doxygenfunction:: otimesu(_TensorLHS &&, _TensorRHS &&);
   :project: tmech
   
Inner product
=============
General inner product
^^^^^^^^^^^^^^^^^^^^^ 
.. _inner_product-func-ref:
.. doxygenfunction:: inner_product(_TensorLHS &&, _TensorRHS &&);
   :project: tmech

Double contraction
^^^^^^^^^^^^^^^^^^ 
.. _double_contraction-func-ref:
.. doxygenfunction:: dcontract(_TensorLHS &&, _TensorRHS &&);
   :project: tmech

Fourth contraction
^^^^^^^^^^^^^^^^^^ 
.. _fourth_contraction-func-ref:
.. doxygenfunction:: ddcontract(_TensorLHS &&, _TensorRHS &&);
   :project: tmech

Invers of a tensor
==================

Second order tensors and fourth order tensors with minior-symmetry
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _inv-func-ref:
.. doxygenfunction:: inv(_Tensor &&)
   :project: tmech

Fully anisotropic tensors
^^^^^^^^^^^^^^^^^^^^^^^^^

.. _invf-func-ref:
.. doxygenfunction:: invf(_Tensor &&)
   :project: tmech
   
Volumetric and deviatoric parts
===============================

Volumetric part
^^^^^^^^^^^^^^^
.. _vol-func-ref:
.. doxygenfunction:: vol(_Tensor &&)
   :project: tmech

Deviatoric part
^^^^^^^^^^^^^^^

.. _dev-func-ref:
.. doxygenfunction:: dev(_Tensor && __tensor)
   :project: tmech

Symmetric and skew-symmetric parts
==================================

Symmetric part
^^^^^^^^^^^^^^
.. _sym-func-ref:
.. doxygenfunction:: sym(_Tensor && __tensor)
   :project: tmech

Skew-symmetric part
^^^^^^^^^^^^^^^^^^^
.. _skew-func-ref:
.. doxygenfunction:: skew(_Tensor && __tensor)
   :project: tmech
   
Isotropic tensor functions
==========================

General function
^^^^^^^^^^^^^^^^

.. _general_iso_func-func-ref:
.. doxygenfunction:: isotropic_tensor_function(_Tensor && __tensor)
   :project: tmech

Square root
^^^^^^^^^^^

.. _sqrt-func-ref:
.. doxygenfunction:: sqrt(_Tensor && __tensor)
   :project: tmech
   
Exponential map
^^^^^^^^^^^^^^^

.. _exp_sym-func-ref:
.. doxygenfunction:: exp_sym(_Tensor && __tensor)
   :project: tmech
   
Logarithmic map
^^^^^^^^^^^^^^^

.. _log-func-ref:
.. doxygenfunction:: log(_Tensor && __tensor)
   :project: tmech
   
   
Positive-negative decomposition
===============================

General method
^^^^^^^^^^^^^^

.. _positive_negative_decomposition_-func-ref:
.. doxygenfunction:: positive_negative_decomposition(_Tensor && __tensor)
   :project: tmech


Only positive part
^^^^^^^^^^^^^^^^^^

.. _positive-func-ref:
.. doxygenfunction:: positive(_Tensor && __tensor)
   :project: tmech

Only negative part
^^^^^^^^^^^^^^^^^^

.. _negative-func-ref:
.. doxygenfunction:: negative(_Tensor && __tensor)
   :project: tmech
   
Eigen-decomposition
===================

.. _eigen_decompositoin-func-ref:
.. doxygenfunction:: eigen_decomposition(_Tensor && __tensor)
   :project: tmech
   
   
Sign decomposition
==================

.. _sign-func-ref:
.. doxygenfunction:: sign(_Tensor &&__tensor, typename std::decay<_Tensor>::type::value_type __eps = 5e-7, std::size_t __max_iter = 10)
   :project: tmech

Polar decomposition
===================

.. _polar_decomposition-func-ref:
.. doxygenfunction:: polar_decomposition
   :project: tmech
   
Exponential map
===============

.. _exponential_map-func-ref:
.. doxygenfunction:: exp(_Tensor && __tensor)
   :project: tmech
