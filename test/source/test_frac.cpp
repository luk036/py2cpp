/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <ostream>
#include <py2cpp/fractions.hpp>

using namespace fun;

// auto operator<=>(const cpp_int& a, const cpp_int& b) -> std::strong_ordering
// {
//     if (operator<(a, b)) return std::strong_ordering::less;
//     if (operator>(a, b)) return std::strong_ordering::greater;
//     return std::strong_ordering::equivalent;
// }

TEST_CASE("Fraction") {
    const auto a = 3U;
    const auto b = 4U;
    const auto c = 5U;
    const auto d = 6U;

    const auto p = Fraction<int>{a, b};
    const auto q = Fraction<int>{c, d};

    CHECK_EQ(p, Fraction<int>(30U, 40U));
    CHECK_EQ(p + q, Fraction<int>(19U, 12U));
    CHECK_EQ((p - q) + q, p);
    // CHECK_NE(p, 0);
}

TEST_CASE("Fraction Special Cases") {
    const auto p = Fraction<int>{3, 4};
    const auto inf = Fraction<int>{1, 0};
    const auto nan = Fraction<int>{0, 0};
    const auto zero = Fraction<int>{0, 1};

    CHECK_LT(-inf, zero);
    CHECK_LT(zero, inf);
    CHECK_LT(-inf, p);
    CHECK_LT(p, inf);
    CHECK_EQ(inf, inf);
    CHECK_LT(-inf, inf);
    CHECK_EQ(inf, inf * p);
    CHECK_EQ(inf, inf * inf);
    CHECK_EQ(inf, p / zero);
    CHECK_EQ(inf, inf / zero);
    CHECK_EQ(nan, nan);
    CHECK_EQ(nan, inf * zero);
    CHECK_EQ(nan, -inf * zero);
    CHECK_EQ(nan, inf / inf);
    CHECK_EQ(nan, nan * zero);
    CHECK_EQ(nan, nan * nan);
    CHECK_EQ(inf, inf + inf);
    CHECK_EQ(nan, inf - inf);
    CHECK_EQ(inf - p, inf);
    CHECK_EQ(-inf + p, -inf);
}
