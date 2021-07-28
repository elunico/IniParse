//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_FILE_H
#define PARSEINI_INI_FILE_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "utils.h"
#include "ini_entry.h"
#include "ini_section.h"
namespace tom {

struct ini_file : std::enable_shared_from_this<ini_file> {
private:
    std::vector<std::shared_ptr<ini_section>>                     sections;
    std::unordered_map<std::string, std::shared_ptr<ini_section>> smap;

public:
    std::string const name;

    [[nodiscard]] std::vector<std::shared_ptr<ini_section>> const& get_sections() const;

    std::shared_ptr<ini_section> get_section(std::string const& name) const;

    std::shared_ptr<ini_entry> get_entry(std::string const& key) const;

    bool add_section(const std::string& section_name, std::string* parent_name);

    bool add_section(std::shared_ptr<ini_section> section);

    friend std::ostream& operator <<(std::ostream&, ini_file const&);

    ini_file(std::string const& name_) : name(name_), sections(std::vector<std::shared_ptr<ini_section>>{ }) {

    }

    ini_file() = delete;

    ini_file(const ini_file&) = delete;

    ini_file(ini_file&&) = default;

    ini_file& operator =(const ini_file&) = delete;

    ini_file& operator =(ini_file&&) = delete;

    ~ini_file();
};

}


#endif //PARSEINI_INI_FILE_H
