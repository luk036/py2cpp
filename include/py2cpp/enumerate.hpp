#pragma once

#include <cstddef>   // import size_t
#include <iterator>  // import std::begin() std::end()
#include <type_traits>
#include <utility>

namespace py {

    namespace detail {

        /**
         * @brief Iterator for enumerated access to container elements
         *
         * Provides iterator functionality that yields index-value pairs when
         * iterating over containers, similar to Python's enumerate() function.
         *
         * @tparam T The container or range type to enumerate
         */
        template <typename T> struct EnumerateIterator {
            typedef decltype(std::begin(std::declval<T>())) TIter;
            using iter_ref = typename std::iterator_traits<TIter>::reference;

            size_t i;
            TIter iter;

            /**
             * @brief Check inequality between iterators
             *
             * Compares the underlying iterators to determine if they point to different elements.
             *
             * @param[in] other The other iterator to compare with
             * @return true if the iterators are not equal, false otherwise
             */
            auto operator!=(const EnumerateIterator &other) const -> bool {
                return iter != other.iter;
            }

            /**
             * @brief Pre-increment operator
             *
             * Increments both the index counter and the underlying iterator.
             *
             * @return EnumerateIterator& Reference to this iterator
             */
            EnumerateIterator &operator++() {
                ++i;
                ++iter;
                return *this;
            }

            /**
             * @brief Dereference operator
             *
             * Returns a pair containing the current index and a reference to the element.
             *
             * @return std::pair<size_t, iter_ref> Pair of (index, element_reference)
             */
            auto operator*() -> std::pair<size_t, iter_ref> {
                return std::pair<size_t, iter_ref>{i, *iter};
            }
        };

        /**
         * @brief Wrapper for making containers enumerable
         *
         * Provides begin/end methods that return EnumerateIterator instances,
         * enabling range-based for loops with index-value pairs.
         *
         * @tparam T The container type to wrap
         */
        template <typename T> struct EnumerateIterableWrapper {
            T &iterable;

            /**
             * @brief Get iterator to the beginning
             *
             * Returns an EnumerateIterator pointing to the start of the container.
             *
             * @return EnumerateIterator<T> Iterator to the beginning
             */
            auto begin() const -> EnumerateIterator<T> {
                return EnumerateIterator<T>{0, std::begin(iterable)};
            }

            /**
             * @brief Get iterator to the end
             *
             * Returns an EnumerateIterator pointing past the end of the container.
             *
             * @return EnumerateIterator<T> Iterator past the end
             */
            auto end() const -> EnumerateIterator<T> {
                return EnumerateIterator<T>{0, std::end(iterable)};
            }
        };

    }  // namespace detail

    /**
     * @brief Create an enumerable wrapper for a container
     *
     * Returns a wrapper that allows iteration over container elements with their indices,
     * similar to Python's enumerate() function.
     *
     * @tparam T The container type
     * @param[in] iterable Reference to the container to enumerate
     * @return detail::EnumerateIterableWrapper<T> Wrapper for enumerated iteration
     */
    template <typename T> inline auto enumerate(T &iterable)
        -> detail::EnumerateIterableWrapper<T> {
        return detail::EnumerateIterableWrapper<T>{iterable};
    }

    /**
     * @brief enumerate(const T &iterable)
     *
     * The `const_enumerate(const T &iterable)` function is a utility function that
     * allows you to iterate over a constant container or range and also get the
     * index of each element in the iteration. It returns an instance of the
     * `detail::EnumerateIterableWrapper<const T>` class, which provides a
     * range-based for loop compatible interface. This function is useful when you
     * want to iterate over a constant container without modifying its elements.
     *
     * @tparam T
     * @param[in] iterable
     * @return detail::EnumerateIterableWrapper<const T>
     */
    template <typename T> inline auto enumerate(const T &iterable)
        -> detail::EnumerateIterableWrapper<const T> {
        return detail::EnumerateIterableWrapper<const T>{iterable};
    }

    /**
     * @brief (deprecated) const_enumerate(const T &iterable)
     */
    template <typename T> inline auto const_enumerate(const T &iterable)
        -> detail::EnumerateIterableWrapper<const T> {
        return detail::EnumerateIterableWrapper<const T>{iterable};
    }
}  // namespace py
