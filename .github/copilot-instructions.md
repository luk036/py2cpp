# GitHub Copilot Instructions for py2cpp

## Project Context

This is a C++17 header-only library providing Python-like data structures (`list`, `dict`, `set`, `range`, `enumerate`, `Fraction`). All code lives in the `py::` namespace.

## Key Contexts

- **Header files**: `include/py2cpp/*.hpp`
- **Tests**: `test/source/*.cpp` (doctest framework)
- **Build**: `cmake -S all -B build && cmake --build build`

## Coding Rules

1. **C++17 only** - No C++20/23 features
2. **Header-only** - Use `#pragma once`
3. **No type hacks** - Never use `reinterpret_cast`, `const_cast`, or C-style casts
4. **Exceptions** - Throw `std::runtime_error` or `std::out_of_range`
5. **Naming** - `CamelCase` classes, `snake_case` functions/variables
6. **Private** - Prefix with underscore (`_lst`)

## Avoid

- Adding new dependencies (use existing: doctest, CPM.cmake)
- Creating new directories (add to existing `include/py2cpp/`)
- Changing build system (CMake + XMake)

## Common Tasks

- **Add new class**: Create in `include/py2cpp/xxx.hpp`, add test in `test/source/test_xxx.cpp`
- **Run tests**: `./build/test/Py2CppTests` or `CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test`
- **Format code**: `cmake --build build --target fix-format`

## Style

- Indent: 4 spaces (Google style)
- Column limit: 100
- Order includes: Standard lib → third-party → project local