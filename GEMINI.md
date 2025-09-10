# GEMINI.md

## Project Overview

This is a C++ header-only library named `py2cpp`. It aims to provide Python-like features in C++. The project is set up as a template with a focus on modern C++ and CMake practices, including integrated testing, documentation, and CI/CD.

The project uses `doctest` for its test framework and can be built with either CMake or XMake.

## Building and Running

### CMake

The project is structured with a main `CMakeLists.txt` that defines the library and several subdirectories for different build targets. The `all` directory is provided for a convenient development workflow to build all targets at once.

**Build all targets:**

```bash
cmake -S all -B build
cmake --build build
```

**Run tests (after building all targets):**

```bash
./build/test/Py2CppTests
```

or

```bash
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

**Run the standalone application (after building all targets):**

```bash
./build/standalone/Py2Cpp --help
```

### XMake

The project also supports `xmake`. The CI pipeline uses `xmake` for testing.

**Build and run tests with XMake:**

```bash
xmake f -m release -y
xmake
xmake run test_py2cpp
```

## Development Conventions

### Coding Style

The project follows the Google C++ Style Guide with an indent width of 4 spaces. The configuration is defined in the `.clang-format` file.

### Code Formatting

`clang-format` is used for formatting C++ code and `cmake-format` for CMake files.

**Check and apply formatting changes:**

```bash
# First, configure the project (if not already done)
cmake -S all -B build

# View changes
cmake --build build --target format

# Apply changes
cmake --build build --target fix-format
```

### Testing

Tests are written using the `doctest` framework and are located in the `test/source` directory.

### Documentation

Documentation is generated using Doxygen.

**Build the documentation:**

```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
```

The generated documentation can be found in `build/doc/doxygen/html/index.html`.
