//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_PARSER_H
#define PARSEINI_INI_PARSER_H

#include <memory>
#include <string>
#include "utils.h"
#include "ini_file.h"
namespace  tom {

class ini_parser {
    // conetent fields
    std::shared_ptr<ini_file> inifile;
    std::string               filename;
    std::string               content;

    // parameterized fields
    std::vector<char> comment_chars  = { '#', ';' };
    char              line_separator = '\n';

    // position tracking

    // current line starts at 1 for error message not 0
    int current_line_     = 1;
    int current_line_pos_ = 0;
    int current_pos_      = 0;

    // implementation fields
    std::shared_ptr<ini_section> current_section_{ };

    // sets the current section to the current section's parent if it exists
    void pop_section_();

    // removes all initial whitespace from the string until the first non-space
    // char returns the number of chars removed
    std::string::size_type drop_initial_whitespace();

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

    // erases all whitespace until and including the next \n char, and returns the
    // count of chars removed
    std::string::size_type drop_to_newline();

    // Increments the position counters for line and chars. Should be called each time the parser advances through the content
    template <typename ch>
    void increment_pos_counts(ch n);

    // delete all implicit constructors / operators ALL_5(ini_parser, delete);

public:
    bool is_comment_char(char chr) const noexcept;

    bool is_value_identifier_char(char c) const noexcept;

    bool is_key_identifier_char(char c) const noexcept;

    explicit ini_parser(
        std::string filename, std::vector<char> comment_chars = { '#', ';' }, char line_separator = '\n'
    );

    // the main method the user of this class will call. Takes the content of the file and parses it into the ini_file data structure
    ini_file parse();

    // accessor method for the filename field
    [[nodiscard]] std::string const& get_filename() const noexcept;

    // default destructor
    ~ini_parser();
};

}

#endif //PARSEINI_INI_PARSER_H
