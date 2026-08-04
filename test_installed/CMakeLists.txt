cmake_minimum_required(VERSION 3.14)
project(Py2CppInstalledTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verify that the installed package can be found and linked via find_package.
find_package(Py2Cpp REQUIRED)

add_executable(py2cpp_installed_test main.cpp)
target_link_libraries(py2cpp_installed_test PRIVATE Py2Cpp::Py2Cpp)

enable_testing()
add_test(NAME py2cpp_installed_test COMMAND py2cpp_installed_test)
