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
#include <array>

namespace tom {

namespace {
template <typename T>
constexpr bool is_pow_2(T n) {
    std::size_t cnt = 0;
    while (n) {
        cnt += (n & 1);
        n >>= 1;
    }
    return cnt == 1;
}
}

template <typename char_type = char, size_t buffer_size = 512>
class inistream {
    static_assert(is_pow_2(buffer_size), "buffer_size must be a power of 2");

    [[maybe_unused]] std::string                           filename;
    std::ifstream                                          input;
    // todo: always allocate in class, allows user to decide to call new and do auto vs heap allocation?
//    std::array<char_type, buffer_size>                     buf;
    std::unique_ptr<char_type[]>                           buf = std::make_unique<char_type[]>(buffer_size);
    typename std::array<char_type, buffer_size>::size_type idx = 0;
    typename std::array<char_type, buffer_size>::size_type max = buffer_size;

    // line separator
    char_type line_separator;

    // stream tracking

    // current line starts at 1 for error message not 0
    int current_line_     = 1;
    int current_line_pos_ = 0;
    int current_pos_      = 0;

    void read_data() {
        input.read(buf.get(), buffer_size);
        max = input.gcount();
        idx = 0;
    }

public:
    explicit inistream(std::string const& filename, char_type line_separator = '\n') :
        filename(filename), line_separator(line_separator) {
        input = std::ifstream(filename);
        read_data();
    }

    bool eof() const { return input.eof(); }

    void increment_pos_counts(char_type c) {
        current_pos_++;
        if (c == line_separator) {
            current_line_++;
            current_line_pos_ = 0;
        }
    }

    char_type peek() const noexcept {
        return buf[idx];
    }

    char_type consume() {
        char_type c = buf[idx];
        if (idx + 1 == max) {
            read_data();
        } else {
            idx++;
        }
        increment_pos_counts(c);
        return c;
    }

    std::tuple<std::size_t, std::size_t, std::size_t> position() const {
        return std::tie(current_pos_, current_line_, current_line_pos_);
    }
};

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
