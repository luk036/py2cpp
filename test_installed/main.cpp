#include <iostream>
#include <py2cpp/py2cpp.hpp>

auto main() -> int {
    py::dict<int, int> d;
    d[1] = 2;
    d[3] = 4;
    const auto ok = (d.size() == 2) && (d[1] == 2) && (d[3] == 4);

    std::cout << "py2cpp installed test: dict size=" << d.size() << "\n";
    return ok ? 0 : 1;
}
