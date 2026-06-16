/**
 * @file gen.hpp
 * @brief Minimal C++20 coroutine-based generator
 *
 * Provides a Generator template that mimics Python-style generators
 * using C++20 coroutines with co_yield support.
 */

#pragma once

#include <coroutine>
#include <exception>
#include <iterator>
#include <type_traits>
#include <utility>

namespace py {

    /**
     * @brief Minimal generator using C++20 coroutines.
     *
     * Models a Python-style generator that lazily yields values of type T.
     * Used with co_yield inside a coroutine function.
     *
     * Supports both value types (Generator<int>) and reference types
     * (Generator<Container&>), using pointer storage to avoid copies.
     *
     * @tparam T Value type to yield
     */
    template <typename T> class Generator {
      public:
        struct promise_type {
            using value_type = std::remove_reference_t<T>;
            using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
            using pointer_type = value_type*;

            pointer_type m_value{};

            auto yield_value(std::remove_reference_t<T>& value) noexcept {
                m_value = std::addressof(value);
                return std::suspend_always{};
            }

            auto yield_value(std::remove_reference_t<T>&& value) noexcept {
                m_value = std::addressof(value);
                return std::suspend_always{};
            }

            auto initial_suspend() noexcept { return std::suspend_always{}; }
            auto final_suspend() noexcept { return std::suspend_always{}; }
            void return_void() noexcept {}
            void unhandled_exception() noexcept { std::terminate(); }

            auto get_return_object() noexcept {
                return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
            }
        };

      private:
        using handle_type = std::coroutine_handle<promise_type>;
        handle_type coro_{};

        explicit Generator(handle_type coro) noexcept : coro_(coro) {}

      public:
        Generator() noexcept = default;
        ~Generator() {
            if (coro_) coro_.destroy();
        }

        Generator(Generator&& other) noexcept : coro_(std::exchange(other.coro_, {})) {}
        Generator& operator=(Generator&& other) noexcept {
            if (this != &other) {
                if (coro_) coro_.destroy();
                coro_ = std::exchange(other.coro_, {});
            }
            return *this;
        }

        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        class iterator {
            handle_type coro_{};

          public:
            using iterator_category = std::input_iterator_tag;
            using value_type = std::remove_reference_t<T>;
            using difference_type = std::ptrdiff_t;
            using reference = std::conditional_t<std::is_reference_v<T>, T, T&>;
            using pointer = std::add_pointer_t<value_type>;

            iterator() noexcept = default;
            explicit iterator(handle_type coro) noexcept : coro_(coro) {}

            iterator& operator++() {
                coro_.resume();
                if (coro_.done()) coro_ = {};
                return *this;
            }
            void operator++(int) { ++*this; }

            reference operator*() const noexcept {
                return static_cast<reference>(*coro_.promise().m_value);
            }

            pointer operator->() const noexcept { return std::addressof(operator*()); }

            bool operator==(const iterator& other) const noexcept { return coro_ == other.coro_; }
            bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        };

        iterator begin() {
            if (!coro_) return iterator{};
            coro_.resume();
            if (coro_.done()) return iterator{};
            return iterator{coro_};
        }

        iterator end() noexcept { return iterator{}; }
    };

}  // namespace py
