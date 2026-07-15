# tmech examples

Each subdirectory is a small, self-contained program demonstrating one feature
of the library. They are built automatically when the project is configured
with `-DTMECH_BUILD_EXAMPLES=ON` (the default):

```bash
cmake -B build -S . -DTMECH_BUILD_EXAMPLES=ON
cmake --build build
```

The compiled binaries land under `build/examples/`. To build and run a single
example manually (header-only, so no linking is required):

```bash
g++ -std=c++17 -Iinclude examples/tensor_products/tensor_products.cpp -o tp && ./tp
```

## Index

| Example | What it shows |
|---|---|
| `tensor_products/` | The three fourth-order outer products `otimes`, `otimesu`, `otimesl` and the general `outer_product`. |
| `contractions/` | Single contraction (`*`), double contraction (`dcontract`), and the general `inner_product` with explicit index sequences. |
| `tensor_inverse/` | Second- and fourth-order inverse (`inv`, `invf`). |
| `eigenvalues/` | Symmetric eigenvalue problems, eigenbasis/eigenvector access, and the repeated-eigenvalue paths. |
| `polar_decomposition/` | Polar decomposition `F = R·U = V·R` and the rotation/stretch tensors. |
| `base_rearrangement/` | Transposition and basis-change (index re-ordering) for second- and fourth-order tensors. |
| `symdiff/linear_elasticity/` | Compile-time symbolic differentiation of a stress function to obtain the tangent modulus. |
| `symdiff/system_nonlinear_equations/` | Building and solving a nonlinear system with the `symdiff` Newton machinery. |
| `j2_plasticity/` | A worked J2 (von Mises) plasticity return-mapping algorithm. |
| `adaptor/voigt/`, `adaptor/full/` | Wrapping raw arrays as tensors with Voigt vs. full layout, in 2D and 3D. |
| `adaptor/abaqus/` | Complete Abaqus UMAT (implicit) and VUMAT (explicit) subroutine skeletons. |
| `paper2022/elasticity/` | The linear-elasticity example accompanying the project's reference paper. |

See the [documentation](https://tmech.readthedocs.io/) for the theory behind
each operation, and `docs/source/api/getting_started.rst` for a guided
introduction.
