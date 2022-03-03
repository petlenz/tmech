.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  Apache-2.0 License.

   The full license is in the file LICENSE, distributed with this software.

Numerical differentiation
=========================

Nonsymmetric input and result
--------------------------------------
.. _num_diff_central-func-ref:
.. doxygenfunction:: num_diff_central(_Function __func, _Point const& __x, double const __h = 1e-7)
   :project: tmech
   



Symmetric input and result
--------------------------------------
.. _num_diff_central-func-ref:
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

