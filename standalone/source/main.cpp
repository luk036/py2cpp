#include <cxxopts.hpp>
#include <iostream>
#include <py2cpp/py2cpp.hpp>
#include <string>

auto main(int argc, char** argv) -> int {
    cxxopts::Options options("Py2Cpp", "Python-like data structures for C++");
    options.add_options()("h,help", "Print usage")("n,count", "Range length",
                                                   cxxopts::value<int>()->default_value("10"));

    const auto result = options.parse(argc, argv);
    if (result.count("help") > 0) {
        std::cout << options.help() << '\n';
        return 0;
    }

    const int count = result["count"].as<int>();
    py::dict<int, int> d;
    for (const auto i : py::range(count)) {
        d[i] = i * i;
    }

    std::cout << "py2cpp: dict size = " << d.size() << '\n';
    std::cout << "py2cpp: range(0.." << count << ") squares:";
    for (const auto i : py::range(count)) {
        std::cout << ' ' << d[i];
    }
    std::cout << '\n';

    return 0;
}
