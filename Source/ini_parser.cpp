//
// Created by Thomas Povinelli on 7/28/21.
//

#include <iostream>
#include "ini_parser.h"
#include "illegal_state.h"

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
    char        c;

    while (std::isspace(stream.peek(), locale)) {
        increment_pos_counts(c);
        c = stream.consume();
        n++;
    }
    return n;
}

template <typename ch>
void ini_parser::increment_pos_counts(ch c) {
    current_pos_++;
    if (c == line_separator) {
        current_line_++;
        current_line_pos_ = 0;
    }
}

std::shared_ptr<ini_section> ini_parser::try_consume_section() {
    if (stream.peek() != '[')
        return nullptr;

    assert(stream.consume() == '[');

    char        c;
    std::string name{};
    while (stream.peek() != ']') {
        c = stream.consume();
        name.push_back(c);
        increment_pos_counts(c);
    }

    if (name.empty()) {
        throw tom::illegal_state("Cannot have section with empty name: " + current_pos_s());
    }

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

    char c;
    while (stream.peek() != '\n') {
        c = stream.consume();
        increment_pos_counts(c);
    }

    return true;
}

std::shared_ptr<ini_entry> ini_parser::try_consume_entry() {
    std::string key{};
    std::string value{};
    char        c;

    // after dropping initial whitespace, consume all valid key_ chars
    while (is_key_identifier_char(stream.peek())) {
        c = stream.consume();
        increment_pos_counts(c);
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
        increment_pos_counts(c);
        value.push_back(c);
    }

    // cut the string to the new line so we can start fresh with the next line
    while (stream.peek() != '\n') {
        increment_pos_counts(c);
        stream.consume();
    }

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
        throw std::invalid_argument(s);
    }

    inifile->add_section(current_section_);

    return std::move(*inifile);
}

std::string ini_parser::current_pos_s() const {
    std::stringstream s;
    s << "INI Parsing of " << get_filename() << " has failed at line: " << current_line_ << ", col: "
      << current_line_pos_ << " (" << current_pos_ << ")";
    return s.str();
}

ini_parser::ini_parser(std::string const& filename, std::vector<char> comment_chars, char line_separator) :
    inifile(std::make_unique<ini_file>(filename)),
    filename(filename),
    stream(inistream{filename}),
    comment_chars(std::move(comment_chars)),
    line_separator(line_separator) { }

}  // namespace tom
