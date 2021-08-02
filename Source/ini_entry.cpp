//
// Created by Thomas Povinelli on 7/28/21.
//
#include "ini_entry.h"

namespace tom {

std::string const& ini_entry::key() const noexcept {
    return key_;
}

std::string const& ini_entry::value() const noexcept {
    return value_;
}

}  // namespace tom
