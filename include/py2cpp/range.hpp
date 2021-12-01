#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

template <typename T> using Value_type = typename T::value_type;

namespace py {

    template <typename T> inline constexpr auto range(T start, T stop) {
        struct _iterator {
            T i;
            constexpr auto operator!=(const _iterator& other) const -> bool {
                return this->i != other.i;
            }
            constexpr auto operator==(const _iterator& other) const -> bool {
                return this->i == other.i;
            }
            constexpr auto operator*() const -> const T& { return this->i; }
            constexpr auto operator*() -> T& { return this->i; }
            constexpr auto operator++() -> _iterator& {
                ++this->i;
                return *this;
            }
            constexpr auto operator++(int) -> _iterator {
                auto temp = *this;
                ++*this;
                return temp;
            }
        };

        struct iterable_wrapper {
          public:
            using value_type [[maybe_unused]] = T;  // luk:
            using key_type [[maybe_unused]] = T;    // luk:
            using iterator = _iterator;             // luk
            T start;
            T stop;
            [[nodiscard]] constexpr auto begin() const { return iterator{this->start}; }
            [[nodiscard]] constexpr auto end() const { return iterator{this->stop}; }
            [[nodiscard]] constexpr auto empty() const -> bool { return this->stop == this->start; }
            [[nodiscard]] constexpr auto size() const -> size_t {
                return static_cast<size_t>(this->stop - this->start);
            }
            constexpr auto operator[](size_t n) const -> T {
                return T(this->start + n);
            }  // no bounds checking
            [[nodiscard]] constexpr auto contains(T n) const -> bool {
                return !(n < this->start) && n < this->stop;
            }
        };

        if (stop < start) {
            stop = start;
        }
        return iterable_wrapper{start, stop};
    }

    template <typename T> inline auto range(T stop) { return range(T(0), stop); }

}  // namespace py
