//
// Created by Thomas Povinelli on 8/3/21.
//

#include "illegal_state.h"

#include <utility>

tom::illegal_state::illegal_state(std::string string) : std::exception() {
    illegal_state::message = std::move(string);
}

const char* tom::illegal_state::what() const noexcept {
    return message.c_str();
}
