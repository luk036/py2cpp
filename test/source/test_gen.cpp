#include <doctest/doctest.h>

#include <py2cpp/gen.hpp>
#include <string>

py::Generator<int> range_gen(int n) {
    for (int i = 0; i < n; ++i) {
        co_yield i;
    }
}

TEST_CASE("Test Generator iterate integers") {
    auto gen = range_gen(5);
    auto count = 0;
    for (auto val : gen) {
        CHECK_EQ(val, count);
        ++count;
    }
    CHECK_EQ(count, 5);
}

TEST_CASE("Test Generator empty") {
    auto gen = range_gen(0);
    CHECK(gen.begin() == gen.end());
}

TEST_CASE("Test Generator strings") {
    auto gen = []() -> py::Generator<std::string> {
        co_yield "hello";
        co_yield "world";
    }();

    auto it = gen.begin();
    CHECK_EQ(*it, "hello");
    ++it;
    CHECK_EQ(*it, "world");
    ++it;
    CHECK(it == gen.end());
}

TEST_CASE("Test Generator move semantics") {
    auto gen1 = range_gen(3);
    auto gen2 = std::move(gen1);
    auto count = 0;
    for ([[maybe_unused]] auto val : gen2) {
        ++count;
    }
    CHECK_EQ(count, 3);
}

TEST_CASE("Test Generator single element") {
    auto gen = range_gen(1);
    auto it = gen.begin();
    CHECK_EQ(*it, 0);
    ++it;
    CHECK(it == gen.end());
}
