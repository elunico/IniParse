//
// Created by Thomas Povinelli on 8/5/21.
//

#ifndef PARSEINI_INISTREAM_H
#define PARSEINI_INISTREAM_H

#include <memory>
#include <string>
#include <array>


namespace tom {

template <typename char_type = char, size_t buffer_size = 512>
class inistream {
    [[maybe_unused]] std::string                           filename;
    std::ifstream                                          input;
    std::array<char_type, buffer_size>                     buf;
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
        input.read(buf.data(), buffer_size);
        max = input.gcount();
        idx = 0;
    }

public:
    explicit inistream(std::string const& filename, char_type line_separator = '\n') :
        filename(filename), line_separator(line_separator) {
        input = std::ifstream(filename);
        read_data();
    }

    [[nodiscard]] bool eof() const { return input.eof(); }

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

    [[nodiscard]] std::tuple<std::size_t, std::size_t, std::size_t> position() const {
        return std::tie(current_pos_, current_line_, current_line_pos_);
    }
};
}


#endif //PARSEINI_INISTREAM_H
