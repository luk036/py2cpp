#pragma once

#include <string>

namespace py2cpp {

    /**  Language codes to be used with the Py2Cpp class */
    enum class LanguageCode { EN, DE, ES, FR };

    /**
     * @brief A class for saying hello in multiple languages
     */
    class Py2Cpp {
        std::string name;

      public:
        /**
         * @brief Creates a new py2cpp
         * @param[in] name the name to greet
         */
        Py2Cpp(std::string name);

        /**
         * @brief Creates a localized string containing the greeting
         * @param[in] lang the language to greet in
         * @return a string containing the greeting
         */
        std::string greet(LanguageCode lang = LanguageCode::EN) const;
    };

}  // namespace py2cpp
