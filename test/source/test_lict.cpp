#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK, TEST_CASE

#include <py2cpp/lict.hpp>  // for Lict, key_iterator
#include <vector>

TEST_CASE("Test Lict") {
    const auto S = py::Lict<double>{std::vector<double>{0.6, 0.7, 0.8}};
    CHECK_EQ(S.size(), 3U);
    CHECK(S.contains(1));
    auto count = 0;
    for ([[maybe_unused]] const auto& p : S) {
        ++count;
    }
    CHECK_EQ(count, 3);
}
