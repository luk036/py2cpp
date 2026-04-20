# AGENTS.md

Guidelines for agentic coding agents in the py2cpp repository.

## Project Overview

- **Type**: C++ header-only library (Python-like data structures in C++)
- **Test Framework**: doctest
- **Build Systems**: CMake (primary), XMake (alternate)
- **Language Standard**: C++17

---

## Build Commands

### CMake (Primary)

```bash
cmake -S all -B build && cmake --build build  # Build all targets
./build/test/Py2CppTests             # Run tests
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test  # Verbose tests
```

### XMake

```bash
xmake f -m release -y && xmake && xmake run test_py2cpp
```

### Single Test

```bash
./build/test/Py2CppTests -tc="pattern*"  # CMake
xmake run test_py2cpp -tc "pattern*"      # XMake
```

### Code Formatting

```bash
cmake -S all -B build
cmake --build build --target format   # Check
cmake --build build --target fix-format  # Apply
```
Requires: `clang-format==18.1.2`, `cmake_format==0.6.13`, `pyyaml`

### Documentation

```bash
cmake -S documentation -B build/doc && cmake --build build/doc --target GenerateDocs
# Output: build/doc/doxygen/html/index.html
```
Requires: Doxygen, jinja2, Pygments

### Static Analysis

```bash
cmake -S all -B build -DUSE_STATIC_ANALYZER=clang-tidy && cmake --build build
cmake -S all -B build -DUSE_SANITIZER=Address && cmake --build build
```

---

## Code Style

- **BasedOnStyle**: Google, **IndentWidth**: 4, **ColumnLimit**: 100
- **Classes**: `CamelCase`, **Functions/Variables**: `snake_case`
- **Constants**: `kCamelCase` or `SCREAMING_SNAKE_CASE`
- **Namespaces**: lowercase (e.g., `py::`)
- **Private members**: leading underscore (e.g., `_lst`)

### Organization

```cpp
#pragma once

#include <vector>
#include <algorithm>

#include "enumerate.hpp"
#include "range.hpp"

namespace py { ... }  // namespace py
```

### Includes

Standard library first, then third-party, then project local. Use `#pragma once`.

---

## Error Handling

Throw `std::runtime_error` for runtime errors, `std::out_of_range` for index errors.

```cpp
const T& at(const key_type& key) const {
    if (key >= size()) throw std::out_of_range("key out of range");
    return _lst[key];
}
```

---

## Type Safety

- **NEVER use `as any`** or type suppression
- Prefer `const` correctness
- Use `explicit` for single-argument constructors

---

## Testing

Tests in `test/source/*.cpp`, use doctest:

```cpp
#define DOCTEST_CONFIG_USE_STD_HEADERS
#include <doctest/doctest.h>

TEST_CASE("description") { REQUIRE(a == b); }
```

Test public interface only.

---

## CMake

- Minimum 3.14, use CPM.cmake
- Header-only: `INTERFACE` library with `PUBLIC`/`INTERFACE` includes
- Column limit: 100 in `.cmake-format`

---

## Git

- Conventional commit messages
- No force pushes to main
- Use feature branches

---

## Additional Notes

- Header-only: add to `include/py2cpp/`
- All symbols in `<py::` namespace
- Update version in `CMakeLists.txt` (VERSION field)
- CI: Ubuntu, macOS, Windows via GitHub Actions

---

## Troubleshooting

- **Build fails**: Ensure deps installed or use CPM cache
- **Format fails**: `pip install clang-format==18.1.2 cmake_format==0.6.13 pyyaml`
- **Tests fail**: Check CMake build type (Debug vs Release)