//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_FILE_H
#define PARSEINI_INI_FILE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "ini_entry.h"
#include "ini_section.h"
#include "utils.h"

namespace tom {

struct ini_file : std::enable_shared_from_this<ini_file> {
private:
    std::unordered_map<std::string, std::shared_ptr<ini_section>> smap{ };

    // used for efficient key access through file
    mutable bool                                    dirty = true;
    mutable std::vector<std::weak_ptr<ini_section>> lazy_section_cache;

public:
    std::string const name;

    ini_file() = delete;

    ini_file(const ini_file&) = delete;

    ini_file(ini_file&&) = default;

    ini_file& operator =(const ini_file&) = delete;

    ini_file& operator =(ini_file&&) = delete;

    explicit ini_file(std::string name_);

    bool add_section(const std::string& section_name, std::string* parent_name);

    bool add_section(std::shared_ptr<ini_section> section);

    void remove_section(std::string const& name);

    std::shared_ptr<ini_section> get_section(std::string const& name) const;

    [[nodiscard]] std::vector<std::weak_ptr<ini_section>> sections() const;

    std::shared_ptr<ini_entry> get_entry(std::string const& key) const;

    ini_section& operator [](std::string const& name);

    friend std::ostream& operator <<(std::ostream&, ini_file const&);

    ~ini_file() = default;
};

}  // namespace tom

#endif  // PARSEINI_INI_FILE_H
