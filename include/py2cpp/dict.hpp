#pragma once

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <unordered_map>
#include <utility>

// template <typename T> using Value_type = typename T::value_type;

namespace py {

    /**
     * @brief Iterator adapter for accessing keys in map-like containers
     *
     * Provides an iterator that dereferences to the key component of
     * key-value pairs in map-like containers.
     *
     * @tparam Iter The underlying iterator type for key-value pairs
     */
    template <typename Iter> struct key_iterator : Iter {
        /**
         * @brief Construct a key iterator from an underlying iterator
         *
         * @param[in] it The underlying iterator for key-value pairs
         */
        explicit key_iterator(Iter it) : Iter(it) {}

        /**
         * @brief Dereference to get the key (const version)
         *
         * @return const auto& Reference to the key
         */
        auto operator*() const -> const auto & { return Iter::operator*().first; }

        /**
         * @brief Dereference to get the key (non-const version)
         *
         * @return const auto& Reference to the key
         */
        auto operator*() -> const auto & { return Iter::operator*().first; }

        /**
         * @brief Pre-increment operator
         *
         * @return key_iterator& Reference to this iterator
         */
        auto operator++() -> key_iterator & {
            Iter::operator++();
            return *this;
        }
        /**
         * @brief Post-increment operator
         *
         * @return key_iterator Copy of this iterator before increment
         */
        auto operator++(int) -> key_iterator {
            auto old = *this;
            ++*this;
            return old;
        }
    };

    /**
     * @brief Python-like dictionary implementation
     *
     * A dictionary class that extends std::unordered_map with Python-like
     * convenience methods and functionality.
     *
     * @tparam Key The key type
     * @tparam T The value type
     */
    template <typename Key, typename T> class dict : public std::unordered_map<Key, T> {
        using Self = dict<Key, T>;
        using Base = std::unordered_map<Key, T>;

      public:
        using value_type = std::pair<const Key, T>;
        using key_type = Key;

        /**
         * @brief Default constructor
         *
         * Creates an empty dictionary.
         */
        dict() : Base{} {}

        /**
         * @brief Construct a dict from an initializer list
         *
         * Creates a dictionary from a list of key-value pairs.
         *
         * @param[in] init Initializer list of key-value pairs
         */
        dict(std::initializer_list<value_type> init) : Base{init} {}

        /**
         * @brief Check if the dictionary contains a specific key
         *
         * @param[in] key
         * @return true if the key is contained in the dictionary, false otherwise
         */
        auto contains(const Key &key) const -> bool { return this->find(key) != this->end(); }

        /**
         * @brief Get a value with a default fallback
         *
         * Returns the value associated with the key, or the default value
         * if the key is not found in the dictionary.
         *
         * @param[in] key The key to look up
         * @param[in] default_value The default value to return if key is not found
         * @return T The value associated with the key, or the default value
         */
        auto get(const Key &key, const T &default_value) const -> T {
            if (!contains(key)) {
                return default_value;
            }
            return this->at(key);
        }

        /**
         * @brief Get iterator to the beginning of keys
         *
         * Returns an iterator that yields keys from the dictionary.
         *
         * @return auto Iterator to the first key
         */
        auto begin() -> key_iterator<decltype(Base::begin())> {
            return key_iterator<decltype(Base::begin())>{Base::begin()};
        }

        /**
         * @brief Get iterator to the end of keys
         *
         * Returns an iterator past the last key in the dictionary.
         *
         * @return auto Iterator past the last key
         */
        auto end() -> key_iterator<decltype(Base::end())> {
            return key_iterator<decltype(Base::end())>{Base::end()};
        }

        /**
         * @brief Get const iterator to the beginning of keys
         *
         * Returns a const iterator that yields keys from the dictionary.
         *
         * @return auto Const iterator to the first key
         */
        auto begin() const -> key_iterator<decltype(Base::begin())> {
            return key_iterator<decltype(Base::begin())>{Base::begin()};
        }

        /**
         * @brief Get const iterator to the end of keys
         *
         * Returns a const iterator past the last key in the dictionary.
         *
         * @return auto Const iterator past the last key
         */
        auto end() const -> key_iterator<decltype(Base::end())> {
            return key_iterator<decltype(Base::end())>{Base::end()};
        }

        /**
         * @brief
         *
         * @return std::unordered_map<Key, T>&
         */
        auto items() -> Base & { return *this; }

        /**
         * @brief
         *
         * @return const std::unordered_map<Key, T>&
         */
        auto items() const -> const Base & { return *this; }

        /**
         * @brief
         *
         * @return _Self
         */
        auto copy() const -> Self { return *this; }

        /**
         * @brief
         *
         * @return _Self&
         */
        auto operator[](const Key &k) const -> const T & {
            return Base::at(k);  // luk: a bug in std::unordered_map?
        }

        /**
         * @brief
         *
         * @return _Self&
         */
        auto at(const Key &k) const -> const T & {
            return Base::at(k);  // luk: a bug in std::unordered_map?
        }

        /**
         * @brief
         *
         * @return _Self&
         */
        auto operator[](const Key &k) -> T & { return Base::operator[](k); }

        /**
         * @brief
         *
         * @return _Self&
         */
        auto operator=(const Self &) -> Self & = delete;

        /**
         * @brief
         *
         * @return _Self&
         */
        auto operator=(Self &&) noexcept -> dict & = default;

        /**
         * @brief Move Constructor (default)
         *
         */
        dict(dict<Key, T> &&) noexcept = default;

        ~dict() = default;

        // private:
        /**
         * @brief Construct a new dict object
         *
         * Copy through explicitly the public copy() function!!!
         */
        dict(const dict<Key, T> &) = default;
    };

    /**
     * @brief
     *
     * @tparam Key
     * @tparam T
     * @param[in] key
     * @param[in] m
     * @return true if the key is contained in the dictionary, false otherwise
     */
    template <typename Key, typename T> inline auto operator<(const Key &key, const dict<Key, T> &m) noexcept
        -> bool {
        return m.contains(key);
    }

    /**
     * @brief
     *
     * @tparam Key
     * @tparam T
     * @param[in] m
     * @return size_t
     */
    template <typename Key, typename T> inline auto len(const dict<Key, T> &m) noexcept -> size_t {
        return m.size();
    }

    /**
     * @brief Template Deduction Guide
     *
     * @tparam Key
     * @tparam T
     */
    // template <typename Key, typename T>
    // dict(std::initializer_list<std::pair<const Key, T>>) -> dict<Key, T>;

    // template <class Sequence>
    // dict(const Sequence& S)
    //     -> dict<std::remove_cv_t<decltype(*std::begin(S))>, size_t>;

}  // namespace py
