//
// Created by Thomas Povinelli on 7/28/21.
//
#include "ini_entry.h"

namespace tom {

ini_entry::ini_entry(ini_entry const& other) : key_(other.key()), value_(other.value()) { }

ini_entry::ini_entry(ini_entry&& other) noexcept: key_(std::move(other.key_)), value_(std::move(other.value_)) { }

ini_entry::ini_entry(std::weak_ptr<ini_section> parent, std::string key, std::string value) :
    parent(parent), key_(std::move(key)), value_(std::move(value)) { }

ini_entry& ini_entry::operator =(ini_entry const& other) {
    if (&other != this) {
        key_   = other.key();
        value_ = other.value();
    }
    return *this;
}

ini_entry& ini_entry::operator =(ini_entry&& other) noexcept {
    if (&other != this) {
        key_   = std::move(other.key_);
        value_ = std::move(other.value_);
    }
    return *this;
}

bool ini_entry::operator ==(ini_entry const& rhs) const noexcept {
    if (&rhs == this) return true;
    return key_ == rhs.key_ && value_ == rhs.value_;
}

bool ini_entry::operator !=(ini_entry const& rhs) const noexcept { return !(*this == rhs); }

std::string const& ini_entry::key() const noexcept {
    return key_;
}

std::string const& ini_entry::value() const noexcept {
    return value_;
}

}  // namespace tom
