.. Copyright (c) 2022, Peter Lenz

   Distributed under the terms of the  BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Converting to Voigt's notation
==============================

Many finite-element solvers store symmetric second-order tensors as
6-component vectors and symmetric fourth-order tensors as :math:`6\times6`
matrices (*Voigt* notation). ``tmech`` bridges full tensor notation and Voigt
storage through the ``adaptor``, which wraps a pre-allocated raw array and
presents it as a tensor while transparently applying the solver's Voigt layout.

The adaptor is parameterised by the value type, the spatial dimension, the
tensor rank, and a layout policy. The bundled ``abq_exp`` policy implements the
Abaqus UMAT/VUMAT convention (the trailing ``true`` selects the strain layout,
where off-diagonal shear components carry the engineering factor of two):

.. code-block:: cpp

    #include <tmech/tmech.h>

    // Raw arrays as they arrive from the solver.
    double ptrC[36], ptrSig[6];
    double ptrEps[6]{1, 1, 1, 1, 1, 0.5};

    const double K{12000}, G{200};
    tmech::eye<double, 3, 2> I;
    const auto IIsym = (tmech::otimesu(I, I) + tmech::otimesl(I, I)) * 0.5;
    const auto IIvol = tmech::otimes(I, I) / 3.0;
    const auto IIdev = IIsym - IIvol;

    // Wrap the raw arrays; the adaptor handles the Voigt layout.
    tmech::adaptor<double, 3, 4, tmech::abq_exp<3>>        C(ptrC);
    tmech::adaptor<double, 3, 2, tmech::abq_exp<3>>        sig(ptrSig);
    tmech::adaptor<double, 3, 2, tmech::abq_exp<3, true>>  eps(ptrEps);

    // Compute using ordinary tensor syntax; results are written back
    // into the raw arrays in Voigt order.
    C   = 3 * K * IIvol + 2 * G * IIdev;
    sig = tmech::dcontract(C, eps);

Assigning to an adaptor writes the result back into the underlying array in the
solver's Voigt ordering, so ``ptrC`` and ``ptrSig`` above are ready to be
returned to the solver. See :doc:`the adaptor page <adaptor>` and the complete
UMAT/VUMAT programs under ``examples/adaptor/abaqus/`` for full, compilable
examples.
