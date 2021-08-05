//
// Created by Thomas Povinelli on 7/28/21.
//

#include <iostream>
#include "ini_parser.h"
#include "parse_error.h"

namespace tom {

ini_parser::~ini_parser() = default;

std::string const& ini_parser::get_filename() const noexcept {
    return filename;
}

void ini_parser::pop_section_() {
    if (current_section_ != nullptr)
        current_section_ = current_section_->parent.lock();
}

std::size_t ini_parser::drop_space() {
    const std::locale& locale = std::locale();
    std::size_t n = 0;

    while (std::isspace(stream.peek(), locale)) {
        stream.consume();
        n++;
    }
    return n;
}


std::shared_ptr<ini_section> ini_parser::try_consume_section() {
    if (stream.peek() != '[')
        return nullptr;

    stream.consume(); // discard the opening [ section marker

    char        c;
    std::string name{};
    while ((c = stream.consume()) != ']')
        name.push_back(c);


    if (name.empty())
        throw tom::empty_section_name("Cannot have section with empty name: " + current_pos_s());


    return std::make_shared<ini_section>(std::weak_ptr<ini_file>{inifile},
                                         std::weak_ptr<ini_section>{current_section_},
                                         name);
}

bool ini_parser::is_comment_char(char chr) const noexcept {
    return std::any_of(begin(comment_chars), end(comment_chars), [chr](auto c) { return c == chr; });
}

bool ini_parser::is_value_identifier_char(char c) const noexcept {
    return !is_comment_char(c) && c != line_separator;
}

bool ini_parser::is_key_identifier_char(char c) const noexcept {
    return !is_comment_char(c) && c != line_separator && c != '=';
}

bool ini_parser::try_consume_comment() {
    drop_space();

    if (!is_comment_char(stream.peek())) {
        return false;
    }

    while (stream.peek() != line_separator)
        stream.consume();

    return true;
}

std::shared_ptr<ini_entry> ini_parser::try_consume_entry() {
    std::string key{};
    std::string value{};
    char        c;

    // after dropping initial whitespace, consume all valid key_ chars
    while (is_key_identifier_char(stream.peek())) {
        c = stream.consume();
        key.push_back(c);
    }

    // If we reach the end of an identifier and don't find an equals, we have a
    // malformed line key_ with no value_
    if (stream.peek() != '=')
        return nullptr;

    stream.consume(); // discard equals sign

    // repeat the process for the value_, but do not drop initial whitespace
    while (is_value_identifier_char(stream.peek())) {
        c = stream.consume();
        value.push_back(c);
    }

    // cut the string to the new line so we can start fresh with the next line
    while (stream.peek() != line_separator)
        stream.consume();


    return std::make_shared<ini_entry>(std::weak_ptr<ini_section>{current_section_}, key, value);
}

ini_file ini_parser::parse() {
    while (!stream.eof()) {
        drop_space();

        auto sec = try_consume_section();

        if (sec != nullptr) {
            if (current_section_ != nullptr)
                inifile->add_section(current_section_);

            current_section_ = std::shared_ptr<ini_section>(sec);

            drop_space();
            continue;
        } else {
            if (current_section_ == nullptr)
                current_section_ = std::make_shared<ini_section>(std::weak_ptr<ini_file>{inifile},
                                                                 std::weak_ptr<ini_section>{},
                                                                 std::string("<Default Section>"));
        }

        // consume comment first to check for # at line
        bool i = try_consume_comment();
        if (i) {
            drop_space();
            continue;
        }

        auto entry = try_consume_entry();

        if (entry != nullptr) {
            current_section_->add_entry(std::shared_ptr<ini_entry>(entry));
            drop_space();
            continue;
        }

        // consume comment again to see if line ends with a comment
        i = try_consume_comment();
        if (i) {
            drop_space();
            continue;
        }

        std::string s = current_pos_s();
        throw tom::parse_error(s);
    }

    inifile->add_section(current_section_);

    return std::move(*inifile);
}

std::string ini_parser::current_pos_s() const {
    std::stringstream s{};
    auto const [cpos, cline, cline_pos] = stream.position();
    s << "INI Parsing of " << get_filename() << " has failed at line: " << cline << ", col: "
      << cline_pos << " (" << cpos << ")";
    return s.str();
}

ini_parser::ini_parser(std::string const& filename, std::vector<char> comment_chars, char line_separator) :
    inifile(std::make_unique<ini_file>(filename)),
    filename(filename),
    stream(inistream{filename}),
    comment_chars(std::move(comment_chars)),
    line_separator(line_separator) { }

}  // namespace tom
