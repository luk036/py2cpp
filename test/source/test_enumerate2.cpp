#include <doctest/doctest.h>

#include <py2cpp/enumerate.hpp>
#include <vector>

TEST_CASE("Test enumerate with std::vector") {
    auto V = std::vector<int>{1, 2, 3, 4, 5};
    auto count = 0;
    for (const auto &p : py::enumerate(V)) {
        CHECK(p.first == count);
        CHECK(p.second == V[count]);
        ++count;
    }
    CHECK(count == V.size());
}

TEST_CASE("Test const_enumerate with std::vector") {
    const auto V = std::vector<int>{1, 2, 3, 4, 5};
    auto count = 0;
    for (const auto &p : py::const_enumerate(V)) {
        CHECK(p.first == count);
        CHECK(p.second == V[count]);
        ++count;
    }
    CHECK(count == V.size());
}
