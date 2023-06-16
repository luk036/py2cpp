#pragma once

#include <cstddef>  // import size_t
#include <iterator> // import std::begin() std::end()
#include <type_traits>
#include <utility>

namespace py {

namespace detail {

/**
 * @brief
 *
 * @tparam T
 */
template <typename T> struct EnumerateIterator {
  typedef decltype(std::begin(std::declval<T>())) TIter;
  using iter_ref = typename std::iterator_traits<TIter>::reference;

  size_t i;
  TIter iter;

  /**
   * @brief
   *
   * @param other
   * @return true
   * @return false
   */
  auto operator!=(const EnumerateIterator &other) const -> bool {
    return iter != other.iter;
  }

  /**
   * @brief
   *
   * @return EnumerateIterator&
   */
  EnumerateIterator &operator++() {
    ++i;
    ++iter;
    return *this;
  }

  /**
   * @brief
   *
   * @return std::pair<size_t, iter_ref>
   */
  auto operator*() -> std::pair<size_t, iter_ref> {
    return std::pair<size_t, iter_ref>{i, *iter};
  }
};

/**
 * @brief
 *
 * @tparam T
 */
template <typename T> struct EnumerateIterableWrapper {
  T &iterable;

  /**
   * @brief
   *
   * @return EnumerateIterator<T>
   */
  auto begin() const -> EnumerateIterator<T> {
    return EnumerateIterator<T>{0, std::begin(iterable)};
  }

  /**
   * @brief
   *
   * @return EnumerateIterator<T>
   */
  auto end() const -> EnumerateIterator<T> {
    return EnumerateIterator<T>{0, std::end(iterable)};
  }
};

} // namespace detail

/**
 * @brief
 *
 * @tparam T
 * @param[in] iterable
 * @return detail::EnumerateIterableWrapper<T>
 */
template <typename T>
inline auto enumerate(T &iterable) -> detail::EnumerateIterableWrapper<T> {
  return detail::EnumerateIterableWrapper<T>{iterable};
}

/**
 * @brief
 *
 * @tparam T
 * @param iterable
 * @return detail::EnumerateIterableWrapper<const T>
 */
template <typename T>
inline auto const_enumerate(const T &iterable)
    -> detail::EnumerateIterableWrapper<const T> {
  return detail::EnumerateIterableWrapper<const T>{iterable};
}

} // namespace py
