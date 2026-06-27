#include <doctest/doctest.h>

#include <array>
#include <cmath>
#include <py2cpp/gen.hpp>
#include <string>
#include <utility>
#include <vector>

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

TEST_CASE("Test Generator single element") {
    auto gen = range_gen(1);
    auto it = gen.begin();
    CHECK_EQ(*it, 0);
    ++it;
    CHECK(it == gen.end());
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

TEST_CASE("Test Generator string concatenation") {
    auto gen = []() -> py::Generator<std::string> {
        co_yield std::string("foo") + "bar";
        co_yield std::string("baz");
    }();

    auto it = gen.begin();
    CHECK_EQ(*it, "foobar");
    ++it;
    CHECK_EQ(*it, "baz");
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

TEST_CASE("Test Generator moved-from is empty") {
    auto gen1 = range_gen(3);
    auto gen2 = std::move(gen1);
    for ([[maybe_unused]] auto val : gen1) {
        FAIL("moved-from generator should be empty");
    }
    auto count = 0;
    for ([[maybe_unused]] auto val : gen2) {
        ++count;
    }
    CHECK_EQ(count, 3);
}

py::Generator<int> fibonacci(int n) {
    int a = 0;
    int b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

TEST_CASE("Test Generator fibonacci") {
    auto gen = fibonacci(10);
    std::vector<int> expected = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

py::Generator<int> squares(int n) {
    for (int i = 0; i < n; ++i) {
        co_yield i* i;
    }
}

TEST_CASE("Test Generator squares") {
    auto gen = squares(5);
    std::vector<int> expected = {0, 1, 4, 9, 16};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

template <typename Container>
py::Generator<typename Container::const_reference> yield_from(const Container& c) {
    for (const auto& elem : c) {
        co_yield elem;
    }
}

TEST_CASE("Test Generator yield from vector") {
    const std::vector<int> vec = {10, 20, 30, 40, 50};
    auto gen = yield_from(vec);
    auto count = 0;
    for (auto val : gen) {
        CHECK_EQ(val, vec[count]);
        ++count;
    }
    CHECK_EQ(count, vec.size());
}

TEST_CASE("Test Generator yield from array") {
    const std::array<int, 4> arr = {7, 14, 21, 28};
    auto gen = yield_from(arr);
    auto count = 0;
    for (auto val : gen) {
        CHECK_EQ(val, arr[count]);
        ++count;
    }
    CHECK_EQ(count, arr.size());
}

TEST_CASE("Test Generator manual iteration") {
    auto gen = range_gen(3);
    auto it = gen.begin();
    CHECK(it != gen.end());
    CHECK_EQ(*it, 0);
    ++it;
    CHECK_EQ(*it, 1);
    ++it;
    CHECK_EQ(*it, 2);
    ++it;
    CHECK(it == gen.end());
}

TEST_CASE("Test Generator manual iteration with pre-increment") {
    auto gen = range_gen(2);
    auto it = gen.begin();
    CHECK_EQ(*it, 0);
    CHECK_EQ(*++it, 1);
    CHECK_EQ(++it, gen.end());
}

using IntPair = std::pair<int, int>;

py::Generator<IntPair> pairs() {
    co_yield IntPair{1, 10};
    co_yield IntPair{2, 20};
    co_yield IntPair{3, 30};
}

TEST_CASE("Test Generator with pairs") {
    auto gen = pairs();
    auto it = gen.begin();
    CHECK_EQ(it->first, 1);
    CHECK_EQ(it->second, 10);
    ++it;
    CHECK_EQ(it->first, 2);
    CHECK_EQ(it->second, 20);
    ++it;
    CHECK_EQ(it->first, 3);
    CHECK_EQ(it->second, 30);
    ++it;
    CHECK(it == gen.end());
}

py::Generator<int> no_yield() { co_return; }

TEST_CASE("Test Generator no yield") {
    auto gen = no_yield();
    CHECK(gen.begin() == gen.end());
}

TEST_CASE("Test Generator two independent generators") {
    auto gen1 = range_gen(3);
    auto gen2 = range_gen(2);

    auto it1 = gen1.begin();
    auto it2 = gen2.begin();

    CHECK_EQ(*it1, 0);
    CHECK_EQ(*it2, 0);
    ++it1;
    CHECK_EQ(*it1, 1);
    ++it2;
    CHECK_EQ(*it2, 1);
    ++it1;
    CHECK_EQ(*it1, 2);
}

TEST_CASE("Test Generator early break") {
    auto gen = range_gen(100);
    auto count = 0;
    for (auto val : gen) {
        CHECK_EQ(val, count);
        ++count;
        if (count >= 5) break;
    }
    CHECK_EQ(count, 5);
}

TEST_CASE("Test Generator large iteration") {
    constexpr auto N = 10000;
    auto gen = range_gen(N);
    auto count = 0;
    for ([[maybe_unused]] auto val : gen) {
        ++count;
    }
    CHECK_EQ(count, N);
}

TEST_CASE("Test Generator large iteration values") {
    constexpr auto N = 5000;
    auto gen = squares(N);
    auto expected = 0;
    for (auto val : gen) {
        CHECK_EQ(val, expected);
        expected = (static_cast<int>(std::sqrt(expected)) + 1)
                   * (static_cast<int>(std::sqrt(expected)) + 1);
    }
    CHECK_EQ(expected, N * N);
}

py::Generator<int> even_numbers(py::Generator<int> src) {
    for (auto val : src) {
        if (val % 2 == 0) {
            co_yield val;
        }
    }
}

TEST_CASE("Test Generator chained filter") {
    auto src = range_gen(10);
    auto filtered = even_numbers(std::move(src));
    std::vector<int> expected = {0, 2, 4, 6, 8};
    auto idx = size_t{0};
    for (auto val : filtered) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

py::Generator<int> add_one(py::Generator<int> src) {
    for (auto val : src) {
        co_yield val + 1;
    }
}

TEST_CASE("Test Generator chained map") {
    auto src = range_gen(5);
    auto mapped = add_one(std::move(src));
    std::vector<int> expected = {1, 2, 3, 4, 5};
    auto idx = size_t{0};
    for (auto val : mapped) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

TEST_CASE("Test Generator chain map then filter") {
    auto mapped = add_one(range_gen(10));
    auto filtered = even_numbers(std::move(mapped));
    std::vector<int> expected = {2, 4, 6, 8, 10};
    auto idx = size_t{0};
    for (auto val : filtered) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

struct Point {
    int x;
    int y;
};

py::Generator<Point> generate_points() {
    co_yield Point{1, 2};
    co_yield Point{3, 4};
}

TEST_CASE("Test Generator arrow operator") {
    auto gen = generate_points();
    auto it = gen.begin();
    CHECK_EQ(it->x, 1);
    CHECK_EQ(it->y, 2);
    ++it;
    CHECK_EQ(it->x, 3);
    CHECK_EQ(it->y, 4);
}

template <typename T> std::vector<T> collect(py::Generator<T> gen) {
    std::vector<T> result;
    for (auto val : gen) {
        result.push_back(std::move(val));
    }
    return result;
}

TEST_CASE("Test Generator collect to vector") {
    auto gen = range_gen(5);
    auto vec = collect(std::move(gen));
    REQUIRE_EQ(vec.size(), size_t{5});
    for (auto i = 0; i < 5; ++i) {
        CHECK_EQ(vec[i], i);
    }
}

TEST_CASE("Test Generator default constructed") {
    py::Generator<int> gen;
    CHECK(gen.begin() == gen.end());
    py::Generator<int> gen2 = std::move(gen);
    CHECK(gen2.begin() == gen2.end());
}
