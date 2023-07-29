#pragma once

#include "enumerate.hpp"
#include "range.hpp"
#include <vector>

namespace py {

/**
 * @brief Dict-like data structure by std::vector and Range
 *
 * @tparam T
 */
template <typename T> class Lict {
  public:
    using key_type = size_t;
    using value_type = T;
    using iterator = py::Range<key_type>::iterator;
    using const_iterator = py::Range<key_type>::iterator;

  private:
    py::Range<key_type> _rng;
    std::vector<T> _lst;

  public:
    /**
     * @brief Construct a new Lict object
     *
     * @param lst
     */
    explicit Lict(std::vector<T> lst)
        : _rng{py::range(lst.size())}, _lst(std::move(lst)) {}

    /**
     * @brief
     *
     * @param key
     * @return T&
     */
    T &operator[](const key_type &key) { return this->_lst[key]; }

    /**
     * @brief
     *
     * @param key
     * @return const T&
     */
    const T &operator[](const key_type &key) const {
        return this->_lst.at(key);
    }

    /**
     * @brief
     *
     * @param key
     * @return const T&
     */
    const T &at(const key_type &key) const { return this->_lst.at(key); }

    // void erase() { throw std::runtime_error("NotImplementedError"); }

    /**
     * @brief
     *
     * @return iterator
     */
    iterator begin() const { return this->_rng.begin(); }

    /**
     * @brief
     *
     * @return iterator
     */
    iterator end() const { return this->_rng.end(); }

    /**
     * @brief
     *
     * @param key
     * @return true
     * @return false
     */
    bool contains(const key_type &key) const {
        return this->_rng.contains(key);
    }

    /**
     * @brief
     *
     * @return size_t
     */
    size_t size() const { return this->_rng.size(); }

    /**
     * @brief
     *
     * @return auto&
     */
    auto &values() { return this->_lst; }

    /**
     * @brief
     *
     * @return const auto&
     */
    const auto &values() const { return this->_lst; }

    /**
     * @brief
     *
     * @return auto
     */
    auto items() { return py::enumerate(this->_lst); }
};

} // namespace py

// int main() {
//   Lict<int> a(std::vector<int>(8, 0));
//   for (int i : a) {
//     a[i] = i * i;
//   }
//   for (auto i : a) {
//     std::cout << i << ": " << a[i] << std::endl;
//   }
//   std::cout << a.__contains__(3) << std::endl;
//   return 0;
// }
