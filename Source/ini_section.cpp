//
// Created by Thomas Povinelli on 7/28/21.
//

#include "ini_section.h"
#include "ini_entry.h"

namespace tom {

ini_section::ini_section(std::weak_ptr<ini_file> owner, std::weak_ptr<ini_section> parent, std::string name) :
    owner(std::move(owner)), parent(std::move(parent)), name(std::move(name)) { }

ini_section::ini_section(ini_section const& other) = default;
ini_section::ini_section(ini_section&& other) noexcept = default;

ini_section& ini_section::operator =(ini_section const& other) {
    if (&other != this) {
        name   = other.name;
        owner  = other.owner;
        parent = other.parent;
        emap   = other.emap;
        dirty  = true;
    }
    return *this;
}

ini_section& ini_section::operator =(ini_section&& other) noexcept {
    if (&other != this) {
        name   = std::move(other.name);
        owner  = std::move(other.owner);
        parent = std::move(other.parent);
        emap   = std::move(other.emap);
        dirty  = true;
    }
    return *this;
}

bool ini_section::add_entry(std::string const& key, std::string const& value) {
    //  dirty = true; // unnecessary becasue we call add_entry()
    auto entry = std::make_shared<ini_entry>(this->weak_from_this(), key, value);
    return add_entry(entry);
}

bool ini_section::add_entry(std::shared_ptr<ini_entry> const& entry) {
    dirty = true;
    auto const& key = entry->key();
    auto result = false;
    if (emap.find(key) != emap.end())
        result = true;

    emap[key] = entry;
    return result;
}

bool ini_section::remove_entry(std::string const& key) {
    dirty = true;
    auto v = (emap.erase(key) >= 1);
    return v;
}

std::shared_ptr<ini_entry> ini_section::get_entry(std::string const& key) const noexcept {
    return get_or_nullptr(emap, key);
}

std::vector<std::weak_ptr<ini_entry>> const& ini_section::entries() const {
    if (dirty) {
        entry_cache = std::vector<std::weak_ptr<ini_entry>>{ };
        std::for_each(std::begin(emap), std::end(emap), [this](auto a) {
            this->entry_cache.push_back(std::get<1>(a));
        });
        dirty = false;
    }
    return entry_cache;
}

std::pair<std::string, bool> ini_section::get_value(std::string const& key) const {
    auto entry = get_entry(key);
    if (entry == nullptr) {
        return std::pair<std::string, bool>{ "", false };
    } else {
        return std::pair<std::string, bool>{ entry->value(), true };
    }
}

std::string const& ini_section::operator [](const std::string& key) {
    dirty = true; // unnecessary bc const ref return value
    auto entry = get_or_nullptr(emap, key);
    if (entry == nullptr) {
        add_entry(key, "");
        entry = get_or_nullptr(emap, key);
    }
    return entry->value();
}


ini_section::~ini_section() = default;

}  // namespace tom
