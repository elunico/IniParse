//
// Created by Thomas Povinelli on 8/3/21.
//

#ifndef PARSEINI_PARSE_ERROR_H
#define PARSEINI_PARSE_ERROR_H
#include <exception>
#include <string>
#include <utility>

namespace tom {
class parse_error : std::exception {

public:
    explicit parse_error(std::string string);

    std::string message;

    [[nodiscard]] const char* what() const noexcept override;
};

class empty_section_name: parse_error {
public:
    explicit empty_section_name(std::string string) : parse_error(std::move(string)) {}
};


}


#endif //PARSEINI_PARSE_ERROR_H
