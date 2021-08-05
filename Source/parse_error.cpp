//
// Created by Thomas Povinelli on 8/3/21.
//

#include "parse_error.h"

#include <utility>

tom::parse_error::parse_error(std::string string) : std::exception() {
    parse_error::message = std::move(string);
}

const char* tom::parse_error::what() const noexcept {
    return message.c_str();
}
