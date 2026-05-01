#include <doctest/doctest.h>

#include <py2cpp/dict.hpp>

TEST_CASE("Test py::dict methods") {
    using E = std::pair<double, int>;
    auto S = py::dict<double, int>{E{0.1, 1}, E{0.3, 3}, E{0.4, 4}};

    SUBCASE("contains") {
        CHECK(S.contains(0.1));
        CHECK(!S.contains(0.2));
    }

    SUBCASE("get") {
        CHECK_EQ(S.get(0.1, 0), 1);
        CHECK_EQ(S.get(0.2, 0), 0);
    }

    SUBCASE("items") {
        auto items = S.items();
        CHECK_EQ(items.size(), 3);
    }

    SUBCASE("copy") {
        auto S2 = S.copy();
        CHECK_EQ(S2.size(), 3);
        S2[0.5] = 5;
        CHECK_EQ(S.size(), 3);
        CHECK_EQ(S2.size(), 4);
    }

    SUBCASE("len") { CHECK_EQ(py::len(S), 3); }

    SUBCASE("operator<") {
        CHECK(0.1 < S);
        CHECK(!(0.2 < S));
    }
}
