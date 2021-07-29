//
// Created by Thomas Povinelli on 7/28/21.
//


#include "ini_entry.h"
#include "ini_section.h"


namespace tom {

std::shared_ptr<ini_entry> ini_section::get_entry(std::string const& key) const {
    return get_or_nullptr(emap, key);
}

std::pair<std::string, bool> ini_section::get_value(std::string const& key) const {
    auto entry = get_entry(key);
    if (entry == nullptr) {
        return std::pair<std::string, bool>{ "", false };
    } else {
        return std::pair<std::string, bool>{ entry->value(), true };
    }
}

bool ini_section::add_entry(std::string const& key, std::string const& value) {
    auto entry = std::make_shared<ini_entry>(this->weak_from_this(), key, value);
    return add_entry(entry);
}

bool ini_section::add_entry(std::shared_ptr<ini_entry> const& entry) {
    auto const& key = entry->key();
    auto result = false;
    if (emap.find(key) != emap.end())
        result = true;

    emap[key] = entry;
    entries_.push_back(entry);
    return result;
}

bool ini_section::remove_entry(const std::string& key) {
    bool v = (std::remove_if(std::begin(entries_), std::end(entries_), [key](std::shared_ptr<ini_entry> a) {
        return a->key() == key;
    }) != std::end(entries_));
    v &= (emap.erase(key) >= 1);
    return v;
}

std::vector<std::shared_ptr<ini_entry>> const& ini_section::entries() const noexcept {
    return entries_;
}

ini_section::~ini_section() = default;




}