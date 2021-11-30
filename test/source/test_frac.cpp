/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <ostream>
#include <py2cpp/fractions.hpp>

using namespace fun;

// auto operator<=>(const cpp_int& a, const cpp_int& b) -> std::strong_ordering {
//     if (operator<(a, b)) return std::strong_ordering::less;
//     if (operator>(a, b)) return std::strong_ordering::greater;
//     return std::strong_ordering::equivalent;
// }

TEST_CASE("Fraction") {
    const auto a = 3U;
    const auto b = 4U;
    const auto c = 5U;
    const auto d = 6U;

    const auto p = Fraction{a, b};
    const auto q = Fraction{c, d};

    CHECK(p == Fraction(30U, 40U));
    CHECK(p + q == Fraction(19U, 12U));
    CHECK((p - q) + q == p);
    // CHECK(p != 0);
}

TEST_CASE("Fraction Special Cases") {
    const auto p = Fraction{3, 4};
    const auto inf = Fraction{1, 0};
    const auto nan = Fraction{0, 0};
    const auto zero = Fraction{0, 1};

    CHECK(-inf < zero);
    CHECK(zero < inf);
    CHECK(-inf < p);
    CHECK(p < inf);
    CHECK(inf == inf);
    CHECK(-inf < inf);
    CHECK(inf == inf * p);
    CHECK(inf == inf * inf);
    CHECK(inf == p / zero);
    CHECK(inf == inf / zero);
    CHECK(nan == nan);
    CHECK(nan == inf * zero);
    CHECK(nan == -inf * zero);
    CHECK(nan == inf / inf);
    CHECK(nan == nan * zero);
    CHECK(nan == nan * nan);
    CHECK(inf == inf + inf);
    CHECK(nan == inf - inf);
    // CHECK( inf + p == nan ); // ???
    // CHECK( -inf + p == nan ); // ???
}
