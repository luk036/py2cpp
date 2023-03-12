#include <fmt/format.h>
#include <py2cpp/greeter.h>

using namespace py2cpp;
using namespace std;

Py2Cpp::Py2Cpp(string _name) : name(move(_name)) {}

std::string Py2Cpp::greet(LanguageCode lang) const {
  switch (lang) {
  default:
  case LanguageCode::EN:
    return fmt::format("Hello, {}!", name);
  case LanguageCode::DE:
    return fmt::format("Hallo {}!", name);
  case LanguageCode::ES:
    return fmt::format("¡Hola {}!", name);
  case LanguageCode::FR:
    return fmt::format("Bonjour {}!", name);
  }
}
