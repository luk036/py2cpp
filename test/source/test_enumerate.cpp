#include <doctest/doctest.h>  // for ResultBuilder, CHECK, TestCase, TEST...

#include <py2cpp/enumerate.hpp>  // for enumerate, iterable_wrapper
#include <py2cpp/range.hpp>      // for range, iterable_wrapper
#include <utility>               // for pair
#include <vector>

TEST_CASE("Test enumerate") {
    auto R = py::range(10);
    auto count = 0;
    for (const auto& p : py::enumerate(R)) {
        static_assert(sizeof p.first >= 0, "make comipler happy");
        CHECK_EQ(p.first, count);
        ++count;
    }
    CHECK_EQ(count, R.size());
}

TEST_CASE("Test enumerate with const vector") {
    const std::vector<int> V = {10, 20, 30, 40, 50};
    size_t count = 0;
    for (const auto& p : py::enumerate(V)) {
        CHECK_EQ(p.first, count);
        CHECK_EQ(p.second, V[count]);
        ++count;
    }
    CHECK_EQ(count, V.size());
}

TEST_CASE("Test enumerate with const vector") {
    const std::vector<int> V = {10, 20, 30, 40, 50};
    size_t count = 0;
    for (const auto& p : py::const_enumerate(V)) {
        CHECK_EQ(p.first, count);
        CHECK_EQ(p.second, V[count]);
        ++count;
    }
    CHECK_EQ(count, V.size());
}
