#include <doctest/doctest.h>

#include <array>
#include <py2cpp/range.hpp>
#include <py2cpp/enumerate.hpp>

TEST_CASE("Test enumerate") {
    const auto R = py::range(10);
    auto count = 0;
    for (const auto& p : py::enumerate(R)) {
        static_assert(sizeof p.first >= 0, "make comipler happy");
	CHECK(p.first == count);
        ++count;
    }
    CHECK(count == R.size());
}

