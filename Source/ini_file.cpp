//
// Created by Thomas Povinelli on 7/28/21.
//

#include "ini_file.h"

namespace tom {
std::vector<std::shared_ptr<ini_section>> const& ini_file::get_sections() const {
    return sections;
}

std::shared_ptr<ini_entry> ini_file::get_entry(std::string const& key) const {
    for (auto& section : sections)
        for (auto& entry : section->entries())
            if (entry->key() == key)
                return entry;

    return nullptr;
}

std::shared_ptr<ini_section> ini_file::get_section(std::string const& name) const {
    for (auto& section : sections)
        if (section->name == name)
            return section;

    return nullptr;
}

ini_file::~ini_file() = default;


std::ostream& operator <<(std::ostream& os, ini_file const& self) {
    os << self.name << " (sections: " << self.sections.size() << ")\n";
    for (auto const& section : self.sections)
        os << *section << "\n";

    return os;
}


bool ini_file::add_section(std::string const& section_name, std::string* parent_name = nullptr) {
    auto const& parent = parent_name != nullptr ? get_section(*parent_name) : std::weak_ptr<ini_section>{ };
    std::shared_ptr<ini_section> section = std::make_shared<ini_section>(this->weak_from_this(),
                                                                         parent,
                                                                         section_name,
                                                                         std::vector<std::shared_ptr<ini_entry>>{ });
    return add_section(section);
}

bool ini_file::add_section(std::shared_ptr<ini_section> section) {
    auto exists = smap.find(section->name) != smap.end();
    smap[section->name] = section;
    sections.push_back(section);
    return exists;
}

}