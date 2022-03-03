Tensors as arrays
=================

Internal memory layout
----------------------

A tensor of `tmech` consists of a contiguous one-dimensional array combined with an indexing scheme that maps
unsigned integers to the location of an element in the array. The number of indices is defined by the 
`rank` and the range of the indices by the `dimension` of a tensor.
The scheme used to map indices into a location in the array is given by this formula

.. math::

	i = \sum_{j=0}^{R-1} i_j D^{R-1-j},
	
where :math:`R` denotes the rank, :math:`D` the dimension, :math:`i_j` is the :math:`j`th index of a tensor and :math:`i\in[0,...,D^{R}]` is the index 
for array indexing.
This produces the following storing scheme for second order tensor

.. math::

	\begin{bmatrix}
	11&
	12&
	13&
	21&
	22&
	23&
	31&
	32&
	33
	\end{bmatrix}
	
and for fourth order tensor

.. math::

	&\left[ 1111 ~1112 ~1113 ~1121 ~1122 ~1123 ~1131 ~1132 ~1133~
	1211 ~1212 ~1213 ~1221 ~1222 ~1223 ~1231 ~1232 ~1233\right.\\
	&\left. 1311 ~1312 ~1313 ~1321 ~1322 ~1323 ~1331 ~1332 ~1333~
 	2111 ~2112 ~2113 ~2121 ~2122 ~2123 ~2131 ~2132 ~2133\right.\\
	&\left. 2211 ~2212 ~2213 ~2221 ~2222 ~2223 ~2231 ~2232 ~2233~
	2311 ~2312 ~2313 ~2321 ~2322 ~2323 ~2331 ~2332 ~2333\right.\\
	&\left. 3111 ~3112 ~3113 ~3121 ~3122 ~3123 ~3131 ~3132 ~3133~
	3211 ~3212 ~3213 ~3221 ~3222 ~3223 ~3231 ~3232 ~3233\right. \\
	&\left. 3311 ~3312 ~3313 ~3321 ~3322 ~3323 ~3331 ~3332 ~3333\right].
	
Performance
-----------

For tensors up to the rank equal to 8 and dimension 3 the memory is stack-allocated which makes them more efficient. 
For higher rank tensors the memory is heap-allocated.



