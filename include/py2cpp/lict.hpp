#pragma once

#include "enumerate.hpp"
#include "range.hpp"
#include <vector>

namespace py {

template <typename T> class Lict {
  using key_type = size_t;
  using value_type = std::pair<size_t, T>;
  using iterator = py::Range<key_type>::iterator;

private:
  std::vector<T> _lst;
  py::Range<key_type> _rng;

public:
  Lict(std::vector<T> lst)
      : _lst(std::move(lst)), _rng{py::range(lst.size())} {}

  T &operator[](const key_type &key) { return this->_lst[key]; }

  const T &operator[](const key_type &key) const { return this->_lst[key]; }

  // void __delitem__() { throw std::runtime_error("NotImplementedError"); }

  iterator begin() { return this->_rng.begin(); }

  iterator end() { return this->_rng.end(); }

  bool contains(const key_type& key) const {
    return this->_rng.contains(key);
  }

  size_t size() { return this->_lst.size(); }

  auto& values() { return this->_lst; }

  const auto& values() const { return this->_lst; }

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
