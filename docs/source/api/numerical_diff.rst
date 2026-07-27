.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Numerical differentiation
=========================

Both functions take an optional stencil-size template parameter selecting the
accuracy order of the central-difference scheme: ``2`` (default) gives the
classic two-point rule :math:`\mathcal{O}(h^2)`, ``5`` a five-point rule
:math:`\mathcal{O}(h^4)`, and ``7`` a seven-point rule
:math:`\mathcal{O}(h^6)`. Higher orders lower the truncation error at the cost
of extra function evaluations and generally want a larger step ``h``.

.. code::

    // default two-point O(h^2)
    auto d2 = tmech::num_diff_central(func, X);
    // five-point O(h^4)
    auto d4 = tmech::num_diff_central<void, 5>(func, X);
    // with an index permutation and a five-point stencil
    auto dd = tmech::num_diff_central<tmech::sequence<1,2,3,4>, 5>(func, X);

Nonsymmetric input and result
--------------------------------------
.. _num_diff_central-func-ref:
.. doxygenfunction:: num_diff_central(_Function __func, _Point const& __x, double const __h = 1e-7)
   :project: tmech




Symmetric input and result
--------------------------------------
.. _num_diff_sym_central-func-ref:
.. doxygenfunction:: num_diff_sym_central(_Function __func, _Point const& __x, double const __h = 1e-7)
   :project: tmech



..
	.. math::
	\frac{\partial \phi}{\partial C_{ij}}
	= \frac{\phi[\SecondT{C}_{ij}^{+\epsilon}] - \phi[\SecondT{C}_{ij}^{-\epsilon}]}{\epsilon} + \mathcal{O}(\epsilon^2),
	where for nonsymmetric arguments :math:`\SecondT{C}_{ij}^{+\epsilon}= \SecondT{C} +\frac{\epsilon}{2} ~\Ei\otimes\Ej` and :math:`\SecondT{C}_{ij}^{-\epsilon}= \SecondT{C} - \frac{\epsilon}{2} ~\Ei\otimes\Ej` and for symmetric arguments
	:math:`\SecondT{C}_{ij}^{+\epsilon}= \SecondT{C} +\frac{\epsilon}{2}~\text{sym} \left(\Ei\otimes\Ej\right)` and :math:`\SecondT{C}_{ij}^{-\epsilon}= \SecondT{C} - \frac{\epsilon}{2} ~\text{sym}\left(\Ei\otimes\Ej\right)`
	.. math::
	\frac{\partial \FourthT{S}}{\partial C_{ij}}
	= \frac{\FourthT{S}[\SecondT{C}_{ij}^{+\epsilon}] - \FourthT{S}[\SecondT{C}_{ij}^{-\epsilon}]}{\epsilon}  + \mathcal{O}(\epsilon^2),
	where for nonsymmetric arguments :math:`\SecondT{C}_{ij}^{+\epsilon}= \SecondT{C} +\frac{\epsilon}{2} ~\Ei\otimes\Ej` and :math:`\SecondT{C}_{ij}^{-\epsilon}= \SecondT{C} - \frac{\epsilon}{2} ~\Ei\otimes\Ej`
	and for symmetric arguments :math:`\SecondT{C}_{ij}^{+\epsilon}= \SecondT{C} +\frac{\epsilon}{2}~\text{sym}\left(\Ei\otimes\Ej\right)` and :math:`\SecondT{C}_{ij}^{-\epsilon}= \SecondT{C} - \frac{\epsilon}{2}~\text{sym}\left(\Ei\otimes\Ej\right)`

