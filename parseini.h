#ifndef PARSEINI_PARSEINI_H
#define PARSEINI_PARSEINI_H

#include <fstream>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#define show_expr(c) \
  std::cout << "[DEBUG :" << __LINE__ << "] " #c << "=" << (c) << std::endl

namespace tom {
struct ini_entry;
struct ini_file;

struct ini_section {
    ini_file   * owner;
    ini_section* parent;
    std::string            name;
    std::vector<ini_entry*> entries;

    ~ini_section();
};

struct ini_entry {
    ini_section* parent;
    std::string key;
    std::string value;
};

struct ini_file {
    std::string              name;
    std::vector<ini_section*> sections;

    [[nodiscard]] std::vector<ini_section*> const& get_sections() const;

    ~ini_file();
};

class ini_parser {
    ini_file* inifile;
    std::string filename;
    std::string content;

    // imple fields
    ini_section* current_section_{};

    void pop_section_();

    static ini_entry parse_entry(ini_section* parent, std::string& s);

    ini_section parse_section(ini_section* parent, std::string& s);

    void spleef_whitespace();

    ini_section* consume_section_optional();

    ini_entry* consume_entry();

    void consume_to_newline();

public:
    explicit ini_parser(std::string const& filename);

    ini_file parse();

    [[nodiscard]] std::string get_filename() const noexcept;

    ~ini_parser();
};

std::ostream& operator <<(std::ostream& os, ini_section const& self);

std::ostream& operator <<(std::ostream& os, ini_entry const& self);
}  // namespace tom

#endif  // PARSEINI_PARSEINI_H
