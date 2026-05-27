#pragma once

#include <cassert>
#include <coroutine>
#include <exception>
#include <iterator>
#include <utility>

namespace py {

    /**
     * @brief Recursive generator using C++20 coroutines with symmetric transfer.
     *
     * Like Generator<T>, but supports yielding another RecursiveGenerator<T>
     * directly via co_yield. This enables stackless recursive generation -
     * ideal for lazy tree/graph traversal.
     *
     * Uses promise chaining so only one coroutine frame is active at any depth.
     *
     * @tparam T Value type to yield
     */
    template <typename T> class [[nodiscard]] RecursiveGenerator {
      public:
        class promise_type final {
          public:
            promise_type() noexcept
                : m_value(nullptr), m_exception(nullptr), m_root(this), m_parentOrLeaf(this) {}

            promise_type(const promise_type&) = delete;
            promise_type(promise_type&&) = delete;

            RecursiveGenerator get_return_object() noexcept {
                return RecursiveGenerator{std::coroutine_handle<promise_type>::from_promise(*this)};
            }

            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void return_void() noexcept {}

            void unhandled_exception() noexcept { m_exception = std::current_exception(); }

            std::suspend_always yield_value(T& value) noexcept {
                m_value = std::addressof(value);
                return {};
            }

            std::suspend_always yield_value(T&& value) noexcept {
                m_value = std::addressof(value);
                return {};
            }

            auto yield_value(RecursiveGenerator&& generator) noexcept {
                return yield_value(generator);
            }

            auto yield_value(RecursiveGenerator& generator) noexcept {
                struct awaitable {
                    awaitable(promise_type* childPromise) : m_childPromise(childPromise) {}

                    bool await_ready() noexcept { return this->m_childPromise == nullptr; }

                    void await_suspend(std::coroutine_handle<promise_type>) noexcept {}

                    void await_resume() {
                        if (this->m_childPromise != nullptr) {
                            this->m_childPromise->throw_if_exception();
                        }
                    }

                  private:
                    promise_type* m_childPromise;
                };

                if (generator.m_promise != nullptr) {
                    m_root->m_parentOrLeaf = generator.m_promise;
                    generator.m_promise->m_root = m_root;
                    generator.m_promise->m_parentOrLeaf = this;
                    generator.m_promise->resume();

                    if (!generator.m_promise->is_complete()) {
                        return awaitable{generator.m_promise};
                    }

                    m_root->m_parentOrLeaf = this;
                }

                return awaitable{nullptr};
            }

            template <typename U> std::suspend_never await_transform(U&&) = delete;

            void destroy() noexcept {
                std::coroutine_handle<promise_type>::from_promise(*this).destroy();
            }

            void throw_if_exception() {
                if (m_exception != nullptr) {
                    std::rethrow_exception(std::move(m_exception));
                }
            }

            bool is_complete() noexcept {
                return std::coroutine_handle<promise_type>::from_promise(*this).done();
            }

            T& value() noexcept {
                assert(this == m_root);
                assert(!is_complete());
                return *m_parentOrLeaf->m_value;
            }

            void pull() noexcept {
                assert(!m_parentOrLeaf->is_complete());

                m_parentOrLeaf->resume();

                while (m_parentOrLeaf != this && m_parentOrLeaf->is_complete()) {
                    m_parentOrLeaf = m_parentOrLeaf->m_parentOrLeaf;
                    m_parentOrLeaf->resume();
                }
            }

          private:
            void resume() noexcept {
                std::coroutine_handle<promise_type>::from_promise(*this).resume();
            }

            T* m_value;
            std::exception_ptr m_exception;
            promise_type* m_root;
            promise_type* m_parentOrLeaf;
        };

        RecursiveGenerator() noexcept : m_promise(nullptr) {}

        RecursiveGenerator(RecursiveGenerator&& other) noexcept
            : m_promise(std::exchange(other.m_promise, nullptr)) {}

        RecursiveGenerator& operator=(RecursiveGenerator&& other) noexcept {
            if (this != &other) {
                if (m_promise != nullptr) {
                    m_promise->destroy();
                }
                m_promise = std::exchange(other.m_promise, nullptr);
            }
            return *this;
        }

        RecursiveGenerator(const RecursiveGenerator&) = delete;
        RecursiveGenerator& operator=(const RecursiveGenerator&) = delete;

        ~RecursiveGenerator() {
            if (m_promise != nullptr) {
                m_promise->destroy();
            }
        }

        class iterator {
          public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = std::remove_reference_t<T>;
            using reference = std::conditional_t<std::is_reference_v<T>, T, T&>;
            using pointer = std::add_pointer_t<T>;

            iterator() noexcept : m_promise(nullptr) {}

            explicit iterator(promise_type* promise) noexcept : m_promise(promise) {}

            bool operator==(const iterator& other) const noexcept {
                return m_promise == other.m_promise;
            }

            bool operator!=(const iterator& other) const noexcept {
                return m_promise != other.m_promise;
            }

            iterator& operator++() {
                assert(m_promise != nullptr);

                m_promise->pull();
                if (m_promise->is_complete()) {
                    auto* temp = m_promise;
                    m_promise = nullptr;
                    temp->throw_if_exception();
                }

                return *this;
            }

            void operator++(int) { (void)operator++(); }

            reference operator*() const noexcept {
                assert(m_promise != nullptr);
                return static_cast<reference>(m_promise->value());
            }

            pointer operator->() const noexcept { return std::addressof(operator*()); }

          private:
            promise_type* m_promise;
        };

        iterator begin() {
            if (m_promise != nullptr) {
                m_promise->pull();
                if (!m_promise->is_complete()) {
                    return iterator(m_promise);
                }

                m_promise->throw_if_exception();
            }

            return iterator(nullptr);
        }

        iterator end() noexcept { return iterator(nullptr); }

        void swap(RecursiveGenerator& other) noexcept { std::swap(m_promise, other.m_promise); }

      private:
        friend class promise_type;

        explicit RecursiveGenerator(std::coroutine_handle<promise_type> h) noexcept
            : m_promise(&h.promise()) {}

        promise_type* m_promise;
    };

    template <typename T> void swap(RecursiveGenerator<T>& a, RecursiveGenerator<T>& b) noexcept {
        a.swap(b);
    }

}  // namespace py
