#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>

#if __cpp_constexpr >= 201304
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14 inline
#endif

namespace py {

/**
 * @brief
 *
 * @tparam T
 */
template <typename T> struct RangeIterator {
  using iterator_category = std::output_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T *;               // or also value_type*
  using reference = T &;             // or also value_type&
  using const_reference = const T &; // or also value_type&
  using key_type = T;                // luk:

  T i;

  /**
   * @brief
   *
   * @param other
   * @return true
   * @return false
   */
  constexpr auto operator!=(const RangeIterator &other) const -> bool {
    return this->i != other.i;
  }

  /**
   * @brief
   *
   * @param other
   * @return true
   * @return false
   */
  constexpr auto operator==(const RangeIterator &other) const -> bool {
    return this->i == other.i;
  }

  /**
   * @brief
   *
   * @return const_reference
   */
  CONSTEXPR14 auto operator*() const -> const_reference { return this->i; }

  /**
   * @brief
   *
   * @return reference
   */
  CONSTEXPR14 auto operator*() -> reference { return this->i; }

  /**
   * @brief
   *
   * @return RangeIterator&
   */
  CONSTEXPR14 auto operator++() -> RangeIterator & {
    ++this->i;
    return *this;
  }

  /**
   * @brief
   *
   * @return RangeIterator
   */
  CONSTEXPR14 auto operator++(int) -> RangeIterator {
    auto temp = *this;
    ++(*this);
    return temp;
  }
};

/**
 * @brief
 *
 * @tparam T
 */
template <typename T> struct Range {
public:
  using iterator = RangeIterator<T>;
  using value_type = T;
  using key_type = T;

  // static_assert(sizeof(value_type) >= 0, "make compiler happy");
  // static_assert(sizeof(key_type) >= 0, "make compiler happy");

  T start;
  T stop;

  /**
   * @brief
   *
   * @return iterator
   */
  constexpr auto begin() const -> iterator { return iterator{this->start}; }

  /**
   * @brief
   *
   * @return iterator
   */
  constexpr auto end() const -> iterator { return iterator{this->stop}; }

  /**
   * @brief
   *
   * @return true
   * @return false
   */
  constexpr auto empty() const -> bool { return this->stop == this->start; }

  /**
   * @brief
   *
   * @return size_t
   */
  constexpr auto size() const -> size_t {
    return static_cast<size_t>(this->stop - this->start);
  }

  /**
   * @brief
   *
   * @param n
   * @return T
   */
  constexpr auto operator[](size_t n) const -> T {
    return T(this->start + n);
  } // no bounds checking

  /**
   * @brief
   *
   * @param n
   * @return true
   * @return false
   */
  constexpr auto contains(T n) const -> bool {
    return !(n < this->start) && n < this->stop;
  }
};

/**
 * @brief
 *
 * @tparam T
 * @param start
 * @param stop
 * @return Range<T>
 */
template <typename T> CONSTEXPR14 auto range(T start, T stop) -> Range<T> {
  if (stop < start) {
    stop = start;
  }
  return Range<T>{start, stop};
}

/**
 * @brief
 *
 * @tparam T
 * @param stop
 * @return Range<T>
 */
template <typename T> CONSTEXPR14 auto range(T stop) -> Range<T> {
  return range(T(0), stop);
}

} // namespace py
