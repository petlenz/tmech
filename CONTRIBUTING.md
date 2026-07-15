# Contributing to tmech

Thanks for your interest in improving **tmech**! Contributions of all
kinds are welcome — bug reports, documentation fixes, new tensor
operations, performance work, and additional tests.

## Getting help / asking questions

- **Questions and usage help:** open a [GitHub Discussion](https://github.com/petlenz/tmech/discussions)
  or a [GitHub Issue](https://github.com/petlenz/tmech/issues) with the
  `question` label.
- **Bug reports and feature requests:** use the
  [issue tracker](https://github.com/petlenz/tmech/issues) and pick the
  matching template.

When reporting a bug, please include a minimal reproducer, your compiler
and version, the C++ standard used, the OS, and whether `TMECH_USE_XSIMD`
was enabled.

## Development setup

tmech is header-only, so there is nothing to compile to *use* it. To work
on it and run the test suite:

```bash
cmake -B build -S . -DTMECH_BUILD_TESTS=ON
cmake --build build -j
cd build && ctest --output-on-failure
```

To exercise the SIMD path as well, configure a second build with
`-DTMECH_USE_XSIMD=ON`. Please make sure the suite passes in **both**
configurations, and under both `Debug` and `Release`, before opening a
pull request — this mirrors what CI checks.

## Making a change

1. [Fork](https://docs.github.com/en/get-started/quickstart/fork-a-repo) the repository.
2. Create a branch with a descriptive name (e.g. `fix-inverse-singular-guard`).
3. Implement your change and **add or update tests** in `tests/tmech_test.h`.
   New numerical code should be checked against an analytical or reference
   result, not just a smoke test.
4. Keep the build warning-clean — the test target is compiled with
   `-Wall -Wextra -Wpedantic -Wconversion -Werror` (and `/WX` on MSVC).
5. Open a [pull request](https://docs.github.com/en/pull-requests) against the
   `master` branch. Fill in the PR template and describe what you changed
   and why.

For larger features, please open an issue first so we can discuss the
approach before you invest significant effort.

## Coding conventions

- C++17; no compiler-specific extensions on public code paths (guard any
  behind the existing `TMECH_*` macros).
- Follow the existing `*_bones.h` / `*_meat.h` split for new expression
  wrappers.
- No `std::cout`/`printf` on library code paths.

## Code of conduct

By participating you agree to abide by our
[Code of Conduct](CODE_OF_CONDUCT.md).

## License

By contributing, you agree that your contributions will be licensed under
the project's [BSD 3-Clause License](LICENSE).
