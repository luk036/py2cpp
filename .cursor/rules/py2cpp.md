# py2cpp - Cursor Rules

## Project Overview

- **Type**: C++ header-only library (Python-like data structures in C++)
- **Namespace**: `py::`
- **Location**: `include/py2cpp/`
- **Test Framework**: doctest

## Build & Test

```bash
cmake -S all -B build && cmake --build build
./build/test/Py2CppTests
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

```bash
# Single test
cmake -S all -B build
cmake --build build --target fix-format  # Auto-format
```

## Code Standards

- **Standard**: C++17
- **Style**: Google (Indent: 4, ColumnLimit: 100)
- **Naming**: `CamelCase` (classes), `snake_case` (functions/variables)
- **Private members**: leading underscore (`_lst`, `_rng`)
- **Includes**: Standard lib → third-party → project local

## Architecture

```
include/py2cpp/
├── dict.hpp       # Python dict
├── fractions.hpp # Fraction arithmetic  
├── set.hpp       # Python set
├── range.hpp    # Python range/enumerate
├── enumerate.hpp # enumerate wrapper
└── lict.hpp    # list-like container
```

## Key Conventions

- Header-only: `#pragma once`
- No type suppression (`as any`, `@ts-ignore`)
- Explicit constructors for single-arg
- Use `std::runtime_error` / `std::out_of_range`
- Tests: `test/source/*.cpp`, doctest only

## When to Iterate

- Add new structures to `include/py2cpp/`
- Update `CMakeLists.txt` version
- Create feature branches, conventional commits