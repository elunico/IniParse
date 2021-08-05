//
// Created by Thomas Povinelli on 8/5/21.
//

#ifndef PARSEINI_INISTREAM_H
#define PARSEINI_INISTREAM_H

#include <memory>
#include <string>
#include <array>


namespace tom {

template <typename T>
constexpr bool is_pow_2(T n) {
    size_t cnt = 0;
    while (n) {
        cnt += (n & 1);
        n >>= 1;
    }
    return cnt == 1;
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
