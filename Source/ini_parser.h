//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_PARSER_H
#define PARSEINI_INI_PARSER_H

#include <memory>
#include <string>
#include "ini_file.h"
#include "utils.h"
#include "parse_error.h"
#include "inistream.h"
#include <array>

namespace tom {

namespace {
}

class ini_parser {
    // conetent fields
    std::shared_ptr<ini_file> inifile;
    std::string               filename;
    inistream<>               stream;

    // parameterized fields
    std::vector<char> comment_chars  = {'#', ';'};
    char              line_separator = '\n';

    // implementation fields
    std::shared_ptr<ini_section> current_section_{};

    // sets the current section to the current section's parent if it exists
    void pop_section_();

    // removes all initial whitespace from the string until the first non-space
    // char returns the number of chars removed or 0 if none are removed
    std::string::size_type drop_space();

    // trys to parse out a section in the ini file. If it cannot it returns
    // nullptr otherwise it returns a freshly new'd section
    // this method does not change the whitespace, and a \n will still be present
    // after it runs
    std::shared_ptr<ini_section> try_consume_section();

    // trys to parse out a entry in the ini file. If it cannot it returns nullptr
    // otherwise it returns a freshly new'd entry
    // this method does not change the whitespace, and a \n will still be present
    // after it runs
    std::shared_ptr<ini_entry> try_consume_entry();

    // trys to parse out a comment in the ini file. If it cannot it returns
    // false otherwise if it does it returns true
    bool try_consume_comment();

    // delete all implicit constructors / operators ALL_5(ini_parser, delete);

public:
    bool is_comment_char(char chr) const noexcept;

    bool is_value_identifier_char(char c) const noexcept;

    bool is_key_identifier_char(char c) const noexcept;

    explicit ini_parser(
        std::string const& filename, std::vector<char> comment_chars = {'#', ';'}, char line_separator = '\n'
    );

    // the main method the user of this class will call. Takes the content of the
    // file and parses it into the ini_file data structure
    ini_file parse();

    // accessor method for the filename field
    [[nodiscard]] std::string const& get_filename() const noexcept;

    // default destructor
    ~ini_parser();

    std::string current_pos_s() const;
};

}  // namespace tom

#endif  // PARSEINI_INI_PARSER_H
