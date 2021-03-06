//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_SECTION_H
#define PARSEINI_INI_SECTION_H

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
// #include "ini_entry.h"

namespace tom {

struct ini_entry;
struct ini_file;

struct ini_section : std::enable_shared_from_this<ini_section> {
private:
    std::unordered_map<std::string, std::shared_ptr<ini_entry>> emap{ };
    mutable std::vector<std::weak_ptr<ini_entry>>               entry_cache{ };
    mutable bool                                                dirty = true;

public:
    std::string                name;
    std::weak_ptr<ini_section> parent;
    std::weak_ptr<ini_file>    owner;

    ini_section(ini_section const& other);
    ini_section(ini_section&& other) noexcept;

    ini_section(std::weak_ptr<ini_file> owner, std::weak_ptr<ini_section> parent, std::string name);

    ini_section& operator =(ini_section const& other);

    ini_section& operator =(ini_section&& other) noexcept;

    // you must add entries using the add_entry method. NEVER directly manipulate
    // the map or vector
    bool add_entry(std::string const& key, std::string const& value);

    bool add_entry(std::shared_ptr<ini_entry> const& entry);

    // you must remove entries using the add_entry method. NEVER directly
    // manipulate the map or vector
    bool remove_entry(std::string const& key);

    std::shared_ptr<ini_entry> get_entry(std::string const& key) const noexcept;

    std::vector<std::weak_ptr<ini_entry>> const& entries() const;

    // returns a pair of <value_, present>
    // If the key_ is present, then the second element is true
    // If the key_ is not present, then value_ == "" and present == false
    std::pair<std::string, bool> get_value(std::string const& key) const;

    std::string const& operator [](std::string const& key);

    ~ini_section();
};

}  // namespace tom

#endif  // PARSEINI_INI_SECTION_H
