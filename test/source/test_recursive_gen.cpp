#include <doctest/doctest.h>

#include <py2cpp/recursive_gen.hpp>
#include <string>
#include <vector>

// --- Non-recursive baseline (same as Generator) ---

py::RecursiveGenerator<int> rec_range_gen(int n) {
    for (int i = 0; i < n; ++i) {
        co_yield i;
    }
}

TEST_CASE("Test RecursiveGenerator iterate integers") {
    auto gen = rec_range_gen(5);
    auto count = 0;
    for (auto val : gen) {
        CHECK_EQ(val, count);
        ++count;
    }
    CHECK_EQ(count, 5);
}

TEST_CASE("Test RecursiveGenerator empty") {
    auto gen = rec_range_gen(0);
    CHECK(gen.begin() == gen.end());
}

TEST_CASE("Test RecursiveGenerator single element") {
    auto gen = rec_range_gen(1);
    auto it = gen.begin();
    CHECK_EQ(*it, 0);
    ++it;
    CHECK(it == gen.end());
}

TEST_CASE("Test RecursiveGenerator move semantics") {
    auto gen1 = rec_range_gen(3);
    auto gen2 = std::move(gen1);
    auto count = 0;
    for ([[maybe_unused]] auto val : gen2) {
        ++count;
    }
    CHECK_EQ(count, 3);
}

// --- Recursive: tree traversal ---

struct TreeNode {
    int value;
    std::vector<TreeNode> children;
};

py::RecursiveGenerator<int> traverse(const TreeNode& node) {
    auto val = node.value;
    co_yield val;
    for (const auto& child : node.children) {
        co_yield traverse(child);
    }
}

TEST_CASE("Test RecursiveGenerator tree traversal") {
    // Tree: 1 -> children 2,3,4; 2 -> children 5,6
    TreeNode tree{
        1, {TreeNode{2, {TreeNode{5, {}}, TreeNode{6, {}}}}, TreeNode{3, {}}, TreeNode{4, {}}}};

    auto gen = traverse(tree);
    auto expected = std::vector{1, 2, 5, 6, 3, 4};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

TEST_CASE("Test RecursiveGenerator single leaf") {
    TreeNode leaf{42, {}};
    auto gen = traverse(leaf);
    auto it = gen.begin();
    CHECK_EQ(*it, 42);
    ++it;
    CHECK(it == gen.end());
}

TEST_CASE("Test RecursiveGenerator deep recursion") {
    // Build a chain: 1 -> 2 -> 3 -> ... -> 100
    // Each node has exactly one child
    TreeNode root{1, {}};
    auto* current = &root;
    for (int i = 2; i <= 100; ++i) {
        current->children.push_back(TreeNode{i, {}});
        current = &current->children[0];
    }

    auto gen = traverse(root);
    auto count = 0;
    for (auto val : gen) {
        CHECK_EQ(val, count + 1);
        ++count;
    }
    CHECK_EQ(count, 100);
}

// --- Recursive: nested recursion ---

// A generator that yields 1, then yields from a sub-generator, then yields 2
py::RecursiveGenerator<int> nested() {
    co_yield 1;
    co_yield []() -> py::RecursiveGenerator<int> {
        co_yield 10;
        co_yield 20;
    }();
    co_yield 2;
}

TEST_CASE("Test RecursiveGenerator nested") {
    auto gen = nested();
    auto expected = std::vector{1, 10, 20, 2};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

// A generator that yields from 3 levels of recursion
py::RecursiveGenerator<int> level3() {
    co_yield 1;
    co_yield []() -> py::RecursiveGenerator<int> {
        co_yield 2;
        co_yield []() -> py::RecursiveGenerator<int> {
            co_yield 3;
            co_yield 4;
        }();
        co_yield 5;
    }();
    co_yield 6;
}

TEST_CASE("Test RecursiveGenerator three levels") {
    auto gen = level3();
    auto expected = std::vector{1, 2, 3, 4, 5, 6};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

// --- Recursive: string type ---

py::RecursiveGenerator<std::string> str_traverse(const TreeNode& node) {
    auto s = std::to_string(node.value);
    co_yield s;
    for (const auto& child : node.children) {
        co_yield str_traverse(child);
    }
}

TEST_CASE("Test RecursiveGenerator strings") {
    TreeNode tree{1, {TreeNode{2, {}}, TreeNode{3, {}}}};
    auto gen = str_traverse(tree);
    auto expected = std::vector<std::string>{"1", "2", "3"};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}

// --- Recursive: empty sub-generator ---

py::RecursiveGenerator<int> with_empty_sub() {
    co_yield 1;
    co_yield rec_range_gen(0);  // yields nothing
    co_yield 2;
}

TEST_CASE("Test RecursiveGenerator empty sub-generator") {
    auto gen = with_empty_sub();
    auto expected = std::vector{1, 2};
    auto idx = size_t{0};
    for (auto val : gen) {
        CHECK_EQ(val, expected[idx]);
        ++idx;
    }
    CHECK_EQ(idx, expected.size());
}
