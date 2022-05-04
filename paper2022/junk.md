

Function        | Description 
--------------- | --------------------------------------------
.rank()         | rank of a tensor (static constexpr)
.dimension()    | dimension of a tensor (static constexpr)
.size()         | total number of elements (static constexpr)
--------------- | 
(i,j,k,l,...)   | access the i,j,k,l,...-th element
.raw_data()     | obtain the raw memory pointer to element data
--------------- | 
.fill(k)        | set all elements to be equal to $k$
.randn()        | as above, but the elements are set to uniformly distributed random values in the [0,1] interval
.randu()        | as above, but use a Gaussian/normal distribution with $\mu = 0$ and $\sigma = 1$
--------------- |
iterators:      | STL-style iterators for accessing elements
.begin()        | iterator pointing at the first element
.end()          | iterator pointing at the past-the-end element
.cbegin()       | constant iterator pointing at the first element
.cend()         | constant iterator pointing at the past-the-end element
--------------- |
.print(ostream) | print elements to ostream

Table: Member functions of the tensor class (subset).


Operation   | Description
----------- | --------------------------------------------
A+B         | add tensor A and B of same rank and dimension
A-B         | subtract tensor B from A of same rank and dimension
A*B         | single inner product of tensor A and B of same rank and dimension. The last basis and first basis, respectively are contracteted 
A/k         | element-wise division of A by scalar k
a\*A, A\*a  | multiply tensor A by scalar k

Table: Tensor operations involving overloaded C++ operator functions  (subset).


Function                                  | Description 
----------------------------------------- | ------------------------------------------------
invf(X)                                   | Full inverse of an anisotropic fourth-order tensor. No symmetry is taken into account. 
inv()                                     |  Inverse of a second order tensor and a fourth-order tensor with minior-symmetry. 
cof(X)                                    |  Cofactors of the second-order tensor X 
adj(X)                                    |  Adjoint of the second-order tensor X 
----------------------------------------- |
skew(X)                                   |  Skew-symmetric part of the second-order tensor X 
sym(X)                                    |  Symmetric part of the second-order tensor X 
----------------------------------------- |
vol(X)                                    |  Volumetric part of the second-order tensor X 
dev(X)                                    |  Deviatoric part of the second-order tensor X 
----------------------------------------- |
Sign tensor decomposition:                | Sign tensor decomposition  of a second-order tensor $\boldsymbol{X} = \boldsymbol{S}\boldsymbol{N}$ with $\boldsymbol{S} = \text{sign}(\boldsymbol{X})$ and $\boldsymbol{N} = \sqrt{\boldsymbol{X}\boldsymbol{X}}$. 
signX = sign(X)                           |
signX.S()                                 | sign part of X
signX.N()                                 | squared part of X*X
----------------------------------------- |
Positive and negative decomposition:      |  decomposition of a symmetric second-order tensor $\boldsymbol{A} = \boldsymbol{A}^{+} + \boldsymbol{A}^{-}$. The positive and negative part are evaluated with an eigendecomposition.
pnX = positive_negative_decomposition(X)  | Provides both parts, positive and negative, respectively. Eigendecomposition is done once.
pnX.positive()                            | positive part of X 
pnX.negative()                            | negative part of X
positive(X)                               | positive part of X. Useful if only positive part is needed.
negative(X)                               | negative part of X. Useful if only negative part is needed.
----------------------------------------- |
Polar decomposition                       |
----------------------------------------- |
Eigendecomposition                        | 


Table: tensor decompositions and related functions (subset).



Function               | Description
---------------------- |----------------------------------------------------------------
eye$\langle$Type,Dim,Rank$\rangle$()   | indentity tensor where each basis pair forms a Kronecker-Delta
---------------------  | 
zeros$\langle$Type,Dim,Rank$\rangle$() | tensor with all elements set to one
---------------------  | 
randn$\langle$Type,Dim,Rank$\rangle$() | matrix with uniformly distributed random values in the [0,1] interval
randu$\langle$Type,Dim,Rank$\rangle$() | matrix with random values from a normal distribution with $\mu=0$ and $\sigma=1$

Table: Functions for generating tensors (subset).




Function                            | Description
------------------------------------------ |----------------------------------------------------------------
almost_equal(A,B,tol) | Compares two tensor expressions if they are numerical equal
------------------------------------------ |
outer_product$\langle$SeqLHS,SeqRHS$\rangle$(A,B) | General outer product. Controlled by template parameters SeqLHS and SeqRHS. Bases contained in sequence SeqLHS are used for ordered element acces in A. Bases contained in SeqRHS are used for ordered element acces in B expression. 
otimes(A,B)                                       | The dyadic product $\otimes$ of a tensor object A with order $n$ and a tensor object B of order $m$ is a tensor of order $m$ + $n$ and defined by $A_{i_{1},...,i_{n},i_{n+1},...,i_{m}} = A_{i_{i},...,i_{n}} B_{i_{n+1},...,i_{m}}$
otimesu(A,B)                                      | The dyadic product $\overline\otimes$ of two second-order tensors is defined by $C_{ijkl}= A_{ik} B_{jl}$
otimesl(A,B)                                      | The dyadic product $\underline\otimes$ of two second-order tensors is defined by $C_{ijkl}= A_{il} B_{jk}$
------------------------------------------ |
inner_product$\langle$SeqLHS,SeqRHS$\rangle$(A,B) | General inner product. Controlled by template parameters SeqLHS and SeqRHS. Bases contained in sequence SeqLHS are contracted with bases contained in sequence SeqLHS.
dcontract(A,B) | A double contraction between two tensors objects contracts the two most right and left indices of A and B, respectively. The result of a double contraction between a tensor of order $n$ and a tensor of order $m$ is a tensor of order $m + n - 4$.
ddcontract(A,B) | A fourth contraction between two tensors objects contracts the fourth most right and left indices. The result of a double contraction between a tensor of order $n$ and a tensor of order $m$ is a tensor of order $m + n - 8$.
------------------------------------------ |
cross(A,B) | Cross product of two first-order tensors.
dot(A,B) | Dot product of two first-order tensors.
------------------------------------------ |
basis_change$\langle$Seq$\rangle$(A) | General basis rearrangement. Controlled by template parameter Seq, which contains the new order of bases.
trans(A) | Wrapper function for transposition for a second-order tensor and major transposition of a fourth-order tensor.
------------------------------------------ |
convert_tensor_to_voigt() | Converts a tensor into Voigt's notation

Table: Tensor valued functions (subset).













----------------------------------------- |
isotropic_tensor_function\<Func\>(X)      |  Isotropic tensor function of the symmetric second-order tensor X. Template parameter of class type Func needs to provid the two static functions ::apply(lambda) and ::derivative(lambda), where lambda is an eigenvalue.



