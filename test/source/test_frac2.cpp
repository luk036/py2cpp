#include <doctest/doctest.h>
#include <ostream>
#include <py2cpp/fractions.hpp>

using namespace fun;

TEST_CASE("Test Fraction methods") {
    auto p = Fraction<int>{3, 4};
    const auto q = Fraction<int>{5, 6};

    SUBCASE("reciprocal") {
        p.reciprocal();
        CHECK(p == Fraction<int>{4, 3});
    }

    SUBCASE("operator*") {
        CHECK(p * q == Fraction<int>{15, 24});
        CHECK(p * 2 == Fraction<int>{6, 4});
        CHECK(2 * p == Fraction<int>{6, 4});
    }

    SUBCASE("operator/") {
        CHECK(p / q == Fraction<int>{18, 20});
        CHECK(p / 2 == Fraction<int>{3, 8});
        CHECK(2 / p == Fraction<int>{8, 3});
    }

    SUBCASE("operator+") {
        CHECK(p + q == Fraction<int>{38, 24});
        CHECK(p + 2 == Fraction<int>{11, 4});
        CHECK(2 + p == Fraction<int>{11, 4});
    }

    SUBCASE("operator-") {
        CHECK(p - q == Fraction<int>{-2, 24});
        CHECK(p - 2 == Fraction<int>{-5, 4});
        CHECK(2 - p == Fraction<int>{5, 4});
    }

    SUBCASE("operator+=") {
        p += q;
        CHECK(p == Fraction<int>{38, 24});
    }

    SUBCASE("operator-=") {
        p -= q;
        CHECK(p == Fraction<int>{-2, 24});
    }

    SUBCASE("operator*=") {
        p *= q;
        CHECK(p == Fraction<int>{15, 24});
    }

    SUBCASE("operator/=") {
        p /= q;
        CHECK(p == Fraction<int>{18, 20});
    }

    SUBCASE("comparison") {
        CHECK(Fraction<int>{1, 2} < Fraction<int>{2, 3});
        CHECK(Fraction<int>{1, 2} <= Fraction<int>{2, 3});
        CHECK(Fraction<int>{2, 3} > Fraction<int>{1, 2});
        CHECK(Fraction<int>{2, 3} >= Fraction<int>{1, 2});
        CHECK(Fraction<int>{1, 2} == Fraction<int>{2, 4});
        CHECK(Fraction<int>{1, 2} != Fraction<int>{2, 3});
    }
}
