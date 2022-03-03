

Single contraction
------------------
  
.. math::

    \SecondT{A}\SecondT{B}&=A_{ik}B_{kj}\BasisSecond{i}{j}\\
    \SecondT{A}\FourthT{C}&=A_{in}C_{njkl}\BasisFourth{i}{j}{k}{l}\\
    \FourthT{C}\SecondT{A}&=C_{ijkn}A_{nl}\BasisFourth{i}{j}{k}{l}




Double contraction
------------------

.. math::

    \SecondT{A}:\SecondT{B}&=A_{ij}B_{ij}\\
    \SecondT{A}:\FourthT{C}&=A_{kl}C_{klij}\BasisSecond{i}{j}\\
    \FourthT{C}:\SecondT{A}&=C_{ijkl}A_{kl}\BasisSecond{i}{j}\\
    \FourthT{C}:\FourthT{A}&=C_{ijkl}A_{klmn}\BasisFourth{i}{j}{m}{n}



Inner product two fourth order tensors
--------------------------------------

.. math::

    \FourthT{A}::\FourthT{B}&=C_{ijkl}A_{ijkl}



Inner and outer contraction of second and fourth order tensors
--------------------------------------------------------------


.. math::

    \FourthT{A}\contractio\SecondT{B}&=A_{ijkl}B_{jk}\BasisSecond{i}{l}\\
    \SecondT{A}\contractio\FourthT{C}&=A_{il}C_{ijkl}\BasisSecond{j}{k}\\
    \FourthT{A}\contractoi\SecondT{B}&=A_{ijmn}B_{in}\BasisSecond{j}{m}= \SecondT{A}\contractio\FourthT{C}\\
    \SecondT{A}\contractoi\FourthT{C}&=A_{jm}C_{ijmn}\BasisSecond{i}{n}=\FourthT{A}\contractio\SecondT{B}




Inner and outer contraction of fourth order tensors
---------------------------------------------------

.. math::

    \FourthT{A}\contractio\FourthT{B}&=A_{ijmn}B_{jlrm}\BasisFourth{i}{l}{r}{n}\\
    \FourthT{A}\contractoi\FourthT{B}&=A_{ijmn}B_{kins}\BasisFourth{k}{j}{m}{s}



Inner and outer double contraction of sixth and fourth order tensors
--------------------------------------------------------------------

.. math::

    \SixthT{A}\dcontractio\FourthT{B}&=A_{ijmnkl}B_{jmnk}\BasisSecond{i}{l}\\
    \FourthT{B}\dcontractio\SixthT{A}&=B_{ijkl}A_{ijmnkl}\BasisSecond{m}{n}\\
    \FourthT{B}\dcontractoi\SixthT{A}&=B_{jmnk}A_{ijmnkl}\BasisSecond{i}{l}\\
    \SixthT{A}\dcontractoi\FourthT{B}&=A_{ijmnkl}A_{ijkl}\BasisSecond{m}{n}
    


Inner and outer double contraction of two sixth-order tensors
-------------------------------------------------------------

.. math::

    \SixthT{A}\dcontractio\SixthT{B}&=A_{ijmnkl}B_{jmopnk}\BasisFourth{i}{l}{o}{p}\\
    \SixthT{A}\dcontractoi\SixthT{B}&=A_{ijmnkl}B_{oijklp}\BasisFourth{i}{l}{o}{p}



Inner and outer double contraction of eight and fourth-order tensors
--------------------------------------------------------------------

.. math::

    \EightT{A}\dcontractio\FourthT{B}&=A_{ijklmnqr}B_{klmn}\BasisFourth{i}{j}{k}{l}\\
    \FourthT{B}\dcontractio\EightT{A}&=B_{ijqr}A_{ijklmnqr}\BasisFourth{k}{l}{m}{n}\\
    \FourthT{B}\dcontractoi\EightT{A}&=B_{ijklmnqr}A_{ijqr}\BasisFourth{k}{l}{m}{n}\\
    \EightT{A}\dcontractoi\FourthT{B}&=A_{klmn}B_{ijklmnqr}\BasisFourth{i}{j}{k}{l}
    

Inner and outer double contraction of eight and sixth-order tensors
--------------------------------------------------------------------

.. math::

    \EightT{A}\dcontractio\SixthT{B}&=A_{ijklmnqr}B_{klopmn}\BasisSixth{i}{j}{q}{r}{o}{p}\\
    \SixthT{B}\dcontractio\EightT{A}&=B_{oijqrp}A_{ijklmnqr}\BasisSixth{o}{p}{k}{l}{m}{n}\\
    \SixthT{B}\dcontractoi\EightT{A}&=B_{klopmn}A_{ijklmnqr}\BasisSixth{o}{p}{i}{j}{q}{r}\\
    \EightT{A}\dcontractoi\SixthT{B}&=A_{ijklmnqr}B_{oijqrp}\BasisSixth{i}{j}{q}{r}{o}{p}
    
    
Inner and outer double contraction of two eight-order tensors
-------------------------------------------------------------

.. math::

    \EightT{A}\dcontractio\EightT{B}&=A_{ijklmnop}B_{klqrstmn}\BasisEigth{i}{j}{o}{p}{q}{r}{s}{t}\\
    \EightT{A}\dcontractoi\EightT{B}&=A_{ijklmnop}B_{qrijopst}\BasisEigth{i}{j}{o}{p}{q}{r}{s}{t}


Fourth order tensor product
---------------------------

.. math::

    \FourthT{C} &= \SecondT{A}\otimes\SecondT{B} = A_{ij} B_{kl}\BasisFourth{i}{j}{k}{l}\\
    \FourthT{C} &= \SecondT{A}\times\SecondT{B}= A_{ij} B_{kl}\BasisFourth{i}{k}{l}{j}\\
    \FourthT{C} &= \SecondT{A}\boxtimes\SecondT{B}= A_{ij} B_{kl}\BasisFourth{i}{k}{j}{l}



Sixth order tensor product
--------------------------

.. math::

    \SixthT{C} &= \SecondT{A}\otimes\FourthT{B} = A_{ij} B_{klmn}\BasisSixth{i}{j}{k}{l}{m}{n}\\
    \SixthT{C} &= \FourthT{A}\otimes\SecondT{B} = A_{ijkl} B_{mn}\BasisSixth{i}{j}{k}{l}{m}{n}\\
    \SixthT{C} &= \SecondT{A}\times\FourthT{B}= A_{ij} B_{klmn}\BasisSixth{i}{k}{l}{m}{n}{j}\\
    \SixthT{C} &= \FourthT{A}\times\SecondT{B}= A_{ijkl} B_{mn}\BasisSixth{i}{m}{j}{k}{n}{l}\\
    \SixthT{C} &= \SecondT{A}\boxtimes\FourthT{B}= A_{ij} B_{klmn}\BasisSixth{i}{k}{l}{m}{j}{n}\\
    \SixthT{C} &= \FourthT{A}\boxtimes\SecondT{B}= A_{ijkl} B_{mn}\BasisSixth{i}{m}{j}{k}{l}{n}



Scalar-valued function of a second-order tensor
-----------------------------------------------

.. math::

    \Delta f = \frac{d}{ds}f(\mathbf{A} + s\Delta\mathbf{A})|_{s=0} = \frac{d}{ds}f({A}_{ij} + s\Delta{A}_{ij})|_{s=0} = \frac{\partial f}{\partial A_{ij}}\Delta A_{ij} = f,{\mathbf{A}}:\Delta\mathbf{A}
    
where

.. math::
    f,{\mathbf{A}} = \frac{\partial f}{\partial \mathbf{A}} = \frac{\partial f}{\partial A_{ij}} \mathbf{e}_i \otimes \mathbf{e}_j
    

Second-order-valued tensor function of a second-order tensor
------------------------------------------------------------

.. math::

    \SecondT{A} = \SecondT{A}(\SecondT{C}):\Delta \SecondT{C} = \frac{d}{ds}\SecondT{A}(\SecondT{C} + s\Delta\SecondT{C})|_{s=0} = \SecondT{A}_{,\SecondT{C}} \contractio\Delta \SecondT{C}
    
where

.. math::

    \SecondT{A}_{,\SecondT{C}} = \frac{\partial \SecondT{A}}{\partial \SecondT{C}} 
    = \frac{\partial A_{ij}}{\partial C_{kl}} \BasisFourth{i}{k}{l}{j}

This has the advantage :math:`\SecondT{F}[\SecondT{D}] = \SecondT{A}\SecondT{B}[\SecondT{C}[\SecondT{D}]]`.

.. math::

    \SecondT{F}_{,\SecondT{D}} = (\SecondT{A}\SecondT{B})_{,\SecondT{C}}\contractio \SecondT{C}_{,\SecondT{D}}
    
    
    
    

Fourth-order-valued tensor function of a fourth-order tensor
------------------------------------------------------------ 
    
.. math::

    \FourthT{A} = \FourthT{A}(\FourthT{C}):\Delta \FourthT{C} = \frac{d}{ds}\FourthT{A}(\FourthT{C} + s\Delta\FourthT{C})|_{s=0} = \FourthT{A}_{,\FourthT{C}} \dcontractio\Delta \FourthT{C}
    
where

.. math::

    \FourthT{A}_{,\FourthT{C}} = \frac{\partial \FourthT{A}}{\partial \FourthT{C}} 
    = \frac{\partial A_{ijkl}}{\partial C_{mnop}} \BasisEigth{i}{k}{m}{n}{o}{p}{l}{j}
    
    
This has the advantage :math:`\FourthT{F}[\FourthT{D}] = \FourthT{A}:\FourthT{B}[\FourthT{C}[\FourthT{D}]]`.
    

.. math::

    \FourthT{F}_{,\FourthT{D}} = (\FourthT{A}:\FourthT{B})_{,\FourthT{C}}\dcontractio \FourthT{C}_{,\FourthT{D}}
    
    
    
    

Fourth-order-valued tensor function of a second-order tensor
------------------------------------------------------------

.. math::

    \FourthT{A} = \FourthT{A}(\SecondT{C}):\Delta \SecondT{C} = \frac{d}{ds}\FourthT{A}(\SecondT{C} + s\Delta\SecondT{C})|_{s=0} = \FourthT{A}_{,\SecondT{C}} \contractio\Delta \SecondT{C}
    
where

.. math::

    \FourthT{A}_{,\SecondT{C}} = \frac{\partial \FourthT{A}}{\partial \SecondT{C}} 
    = \frac{\partial A_{ijkl}}{\partial C_{mn}} \BasisSixth{i}{k}{m}{n}{l}{j}
    

This has the advantage :math:`\FourthT{F}[\SecondT{D}] = \FourthT{A}:\FourthT{B}[\SecondT{C}[\SecondT{D}]]`.


.. math::

    \SecondT{F}_{,\SecondT{D}} = (\FourthT{A}:\FourthT{B})_{,\SecondT{C}}\contractio \SecondT{C}_{,\SecondT{D}}











Derivation of a second order tensor
-----------------------------------

.. math::

    \SecondT{A}_{,\SecondT{A}} = \SecondT{I}\otimes\SecondT{I}\\
    \SecondT{A}^T_{,\SecondT{A}} = \SecondT{I}\boxtimes\SecondT{I}




Derivation of otimes tensor product
-----------------------------------

.. math::

    \frac{\partial \SecondT{A} \otimes \SecondT{B}}{\partial \SecondT{C}} &= \frac{\partial A_{ij} B_{kl}}{\partial C_{mn}} \BasisSixth{i}{j}{m}{n}{k}{l}\\
    &=(D_{imnj} B_{kl} + A_{ij} E_{kmnl}) \BasisSixth{i}{j}{m}{n}{k}{l}\\
    & = (\SecondT{A}_{,\SecondT{C}})^R \otimes \SecondT{B} + \SecondT{A} \otimes {}^R (\SecondT{B}_{,\SecondT{C}})





Derivation of times tensor product
----------------------------------

.. math::

    \frac{\partial \SecondT{A} \times \SecondT{B}}{\partial \SecondT{C}} &=
    \frac{\partial A_{ij} B_{kl}}{\partial C_{mn}}
    \BasisSixth{i}{k}{m}{n}{l}{j} \\
    &= (D_{imnj} B_{kl} + A_{ij} E_{kmnl}) \BasisSixth{i}{k}{m}{n}{l}{j}\\
    &= \SecondT{A}_{,\SecondT{C}} \times \SecondT{B} + \SecondT{A} \times \SecondT{B}_{,\SecondT{C}} 




Derivation of squad times tensor product
----------------------------------------

.. math::

    \frac{\partial \SecondT{A} \boxtimes \SecondT{B}}{\partial \SecondT{C}} &=\frac{\partial A_{ij} B_{kl}}{\partial C_{mn}}\BasisSixth{i}{k}{m}{n}{j}{l} \\
    &= (D_{imnj}B_{kl} + A_{ij} E_{kmnl}) \BasisSixth{i}{k}{m}{n}{j}{l}  \\
    &=  \SecondT{A}_{,\SecondT{C}} \boxtimes \SecondT{B} + \SecondT{A} \boxtimes \SecondT{B}_{,\SecondT{C}} 




Derivation of the double product second and second order tensor
---------------------------------------------------------------

.. math::

    \frac{\partial \SecondT{A}:\SecondT{B}}{\partial \SecondT{C}} 
    &= \frac{\partial A_{ij} B_{ij}}{\partial C_{mn}}\BasisSecond{m}{n}\\
    &=(D_{imnj} B_{ij} + A_{ij} E_{imnj})\BasisSecond{i}{j} \\
    &=\FourthT{A}_{,\SecondT{C}} \contractoi \SecondT{B} + \FourthT{A} \contractio \SecondT{B}_{,\SecondT{C}}





Derivation of the double product fourth and second order tensor
---------------------------------------------------------------

.. math::

    \frac{\partial \FourthT{A}:\SecondT{B}}{\partial \SecondT{C}} 
    &= \frac{\partial A_{ijkl} B_{kl}}{\partial C_{mn}}\BasisFourth{i}{m}{n}{j}\\
    &=(D_{ijmnkl} B_{kl} + A_{ijkl} E_{kmnl})\BasisFourth{i}{m}{n}{j} \\
    &=\FourthT{A}_{,\SecondT{C}} : \SecondT{B} + \FourthT{A} : (\SecondT{B}_{,\SecondT{C}} )^R

.. math::

    \frac{\partial \SecondT{B}:\FourthT{A}}{\partial \SecondT{C}} 
    &= \frac{\partial B_{kl} A_{klij} }{\partial C_{mn}}\BasisFourth{i}{m}{n}{j}\\
    &=(E_{kmnl} A_{klij} + B_{kl} D_{klmnij})\BasisFourth{i}{m}{n}{j} \\
    &={}^R(\SecondT{B}_{,\SecondT{C}}) : \FourthT{A} + \SecondT{B} : \FourthT{A}_{,\SecondT{C}}




Derivation of the double product fourth and fourth order tensor
---------------------------------------------------------------

.. math::

    \frac{\partial \FourthT{A}:\FourthT{B}}{\partial \SecondT{C}} 
    &= \frac{\partial A_{ijkl} B_{klmn}}{\partial C_{op}}\BasisSixth{i}{j}{o}{p}{m}{n}\\
    &=(D_{ijopkl} B_{klmn} + A_{ijkl} E_{klopmn})\BasisSixth{i}{j}{o}{p}{m}{n}\\
    &=\FourthT{A}_{,\SecondT{C}} : \FourthT{B} + \FourthT{A}: \FourthT{B}_{,\SecondT{C}}
    


Derivation of the scalar product fourth and fourth order tensor
---------------------------------------------------------------

.. math::

    \frac{\partial \FourthT{A}::\FourthT{B}}{\partial \SecondT{C}} 
    &= \frac{\partial A_{ijkl} B_{ijkl}}{\partial C_{mn}}\BasisSecond{m}{n}\\
    &=(D_{ijmnkl} B_{ijkl} + A_{ijkl} E_{ijmnkl})\BasisSecond{m}{n}\\
    &=\FourthT{A}_{,\SecondT{C}} \dcontractoi \FourthT{B} + \FourthT{A} \dcontractio \FourthT{B}_{,\SecondT{C}}



Derivation of the inner product of two fourth-order tensor
---------------------------------------------------------------

.. math::

    \frac{\partial \FourthT{A}::\FourthT{B}}{\partial \FourthT{C}} 
    &= \frac{\partial A_{ijkl} B_{ijkl}}{\partial C_{opqr}}\BasisFourth{o}{p}{q}{r}\\
    &=(D_{ijopqrkl} B_{ijkl} + A_{ijkl} E_{ijopqrkl})\BasisSixth{i}{j}{o}{p}{m}{n}\\
    &=\FourthT{A}_{,\SecondT{C}} \dcontractoi \FourthT{B} + \FourthT{A} \dcontractio \FourthT{B}_{,\SecondT{C}}



Derivation of a symmetric second order tensor
---------------------------------------------

.. math::

    \SecondT{A}_{,\SecondT{A}} = \frac{\partial\left(\frac{1}{2}\left(\SecondT{A} + \SecondT{A}^T\right)\right)}{\partial \SecondT{A}} = \frac{1}{2}\left(\SecondT{I}\otimes\SecondT{I} + \SecondT{I}\boxtimes\SecondT{I}\right) = \FourthT{S}





Derivation of a skew symmetric second order tensor
--------------------------------------------------

.. math::

    \SecondT{A}_{,\SecondT{A}} = \frac{\partial\left(\frac{1}{2}\left(\SecondT{A} - \SecondT{A}^T\right)\right)}{\partial \SecondT{A}} = \frac{1}{2}\left(\SecondT{I}\otimes\SecondT{I} - \SecondT{I}\boxtimes\SecondT{I}\right)= \FourthT{A}





Derivation of a scalar-valued function
--------------------------------------


.. math::

    \phi[\SecondT{A}]_{,\SecondT{A}} =
    \frac{\partial \phi}{\partial \SecondT{A}} = \frac{\partial \phi}{\partial A_{ij}}





Derivation of second order tensor functions
-------------------------------------------

.. math::

    \SecondT{F}[\SecondT{A}[\SecondT{X}]]_{,\SecondT{X}} =
    \frac{\partial \SecondT{F}[\SecondT{A}[\SecondT{X}]]}{\partial \SecondT{A}} \contractio \frac{\partial \SecondT{A}[\SecondT{X}]}{\partial \SecondT{X}}




Derivation of symmetric second order tensor functions 
-----------------------------------------------------

.. math::

    \SecondT{A} = \SecondT{A}^T
    
    and 
    
.. math::
    \SecondT{X} = \SecondT{X}^T

.. math::

    \SecondT{F}[\SecondT{A}[\SecondT{X}]]_{,\SecondT{X}} =\left(
    \frac{\partial \SecondT{F}[\SecondT{A}[\SecondT{X}]]}{\partial \SecondT{A}} \contractio \frac{\partial \SecondT{A}[\SecondT{X}]}{\partial \SecondT{X}}\right)\contractio\FourthT{I}^{sym}




Derivation of skew symmetric second order tensor functions 
----------------------------------------------------------

.. math::

    \SecondT{A} = -\SecondT{A}^T
    
     and 

.. math::
     
     \SecondT{X} = -\SecondT{X}^T

.. math::

    \SecondT{F}[\SecondT{A}[\SecondT{X}]]_{,\SecondT{X}} =\left(
    \frac{\partial \SecondT{F}[\SecondT{A}[\SecondT{X}]]}{\partial \SecondT{A}} \contractio \frac{\partial \SecondT{A}[\SecondT{X}]}{\partial \SecondT{X}}\right)\contractio\FourthT{I}^{skew}




Inverse
-------

.. math::

    \SecondT{A} \SecondT{A}\inv = \SecondT{A}\inv \SecondT{A} = \eye




Determinante
------------

.. math::

    \partial_{\SecondT{A}} \text{det}(\SecondT{A}) = \text{det}(\SecondT{A})\SecondT{A}^{-T}




Euclidean norm
--------------

.. math::

    \partial_{\SecondT{A}} ||\SecondT{A}|| = \partial_{\SecondT{A}} \sqrt{\SecondT{A}:\SecondT{A}} = \frac{1}{||\SecondT{A}||} \SecondT{A}




Principal invariants of a second order tensor
---------------------------------------------

.. math::

    I_A &=\text{tr}(\SecondT{A}) =  \SecondT{A}:\SecondT{I} \\
    II_A &= \frac{1}{2}\left(\text{tr}(\SecondT{A})^2 - \text{tr}(\SecondT{A}^2)\right) \\
    III_A &=  \text{det}(\SecondT{A}) 




Derivatives of the principal invariants of a second order tensor
----------------------------------------------------------------

.. math::

    \partial_{\SecondT{A}} I_A &= \partial_{\SecondT{A}}\text{tr}(\SecondT{A}) = \partial_{\SecondT{A}} \SecondT{A}:\SecondT{I} = \SecondT{I}\\
    \partial_{\SecondT{A}} II_A &= \partial_{\SecondT{A}}\frac{1}{2}\left(\text{tr}(\SecondT{A})^2 - \text{tr}(\SecondT{A}^2)\right) = I_A\SecondT{I} - \SecondT{A}\\
    \partial_{\SecondT{A}}  III_A &= \partial_{\SecondT{A}} det(\SecondT{A}) = \text{det}(\SecondT{A})\SecondT{A}^{-T}




