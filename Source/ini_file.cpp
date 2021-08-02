//
// Created by Thomas Povinelli on 7/28/21.
//

#include "ini_file.h"

namespace tom {

std::vector<std::weak_ptr<ini_section>> ini_file::get_sections() const {
    if (dirty) {
        lazy_section_cache = std::vector<std::weak_ptr<ini_section>>();
        std::for_each(std::cbegin(smap),
                      std::cend(smap),
                      [this](auto a) { lazy_section_cache.push_back(std::get<1>(a)); });
        dirty = false;
    }
    return lazy_section_cache;
}

std::shared_ptr<ini_entry> ini_file::get_entry(std::string const& key) const {
    auto ptr = get_sections();
    for (auto& section : ptr)
        if (auto entry = section.lock()->get_entry(key); entry != nullptr)
            return entry;

    return nullptr;
}

std::shared_ptr<ini_section> ini_file::get_section(
    std::string const& name
) const {
    return get_or_nullptr(smap, name);
}

ini_file::~ini_file() = default;

std::ostream& operator <<(std::ostream& os, ini_file const& self) {
    auto sections = self.get_sections();
    for (auto const& section : sections)
        os << *section.lock() << "\n";

    return os;
}

bool ini_file::add_section(std::string const& section_name, std::string* parent_name = nullptr) {
    dirty = true;
    auto const& parent = parent_name != nullptr ? get_section(*parent_name) : std::weak_ptr<ini_section>{ };
    std::shared_ptr<ini_section> section = std::make_shared<ini_section>(this->weak_from_this(), parent, section_name);
    return add_section(section);
}

bool ini_file::add_section(std::shared_ptr<ini_section> section) {
    dirty = true;
    auto exists = smap.find(section->name) != smap.end();
    smap[section->name] = section;
    return exists;
}

ini_section& ini_file::operator [](std::string const& name) {
    dirty = true;
    auto sec = get_or_nullptr(smap, name);
    if (sec == nullptr) {
        add_section(name);
        sec = get_or_nullptr(smap, name);
    }
    return *sec;
}

void ini_file::remove_section(std::string const& name) {
    dirty = true;
    smap.erase(name);
}

}  // namespace tom
