//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_SECTION_H
#define PARSEINI_INI_SECTION_H

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <unordered_map>
//#include "ini_entry.h"


namespace tom {
struct ini_file;

struct ini_section : std::enable_shared_from_this<ini_section> {
private:
    std::vector<std::shared_ptr<ini_entry>>                     entries_;
    std::unordered_map<std::string, std::shared_ptr<ini_entry>> emap;

public:
    std::string                name;
    std::weak_ptr<ini_section> parent;
    std::weak_ptr<ini_file>    owner;

    std::shared_ptr<ini_entry> get_entry(std::string const& key) const;

    std::vector<std::shared_ptr<ini_entry>> const& entries() const noexcept;

    // you must add entries using the add_entry method. NEVER directly manipulate the map or vector
    bool add_entry(std::string const& key, std::string const& value);

    bool add_entry(std::shared_ptr<ini_entry> const& entry);

    // you must remove entries using the add_entry method. NEVER directly manipulate the map or vector
    bool remove_entry(std::string const& key);

    // returns a pair of <value_, present>
    // If the key_ is present, then the second element is true
    // If the key_ is not present, then value_ == "" and present == false
    std::pair<std::string, bool> get_value(std::string const& key) const;

    ~ini_section();

    ini_section(
        std::weak_ptr<ini_file> owner,
        std::weak_ptr<ini_section> parent,
        std::string name,
        std::vector<std::shared_ptr<ini_entry>> entries
    ) :

        owner(std::move(owner)), parent(std::move(parent)), name(std::move(name)), entries_(std::move(entries)) { }

};

}

#endif //PARSEINI_INI_SECTION_H