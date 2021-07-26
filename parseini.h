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

#define ALL_5(cls, spec)       \
  cls() = spec;                \
  cls(cls&) = spec;            \
  cls(cls&&) = spec;           \
  cls& operator=(cls&) = spec; \
  cls& operator=(cls&&) = spec;

namespace tom {
struct ini_entry;
struct ini_file;

struct ini_section {
  ini_file* owner;
  ini_section* parent;
  std::string name;
  std::vector<ini_entry*> entries;

  ~ini_section();

  ALL_5(ini_section, delete);
};

struct ini_entry {
  ini_section* parent;
  std::string key;
  std::string value;

  ALL_5(ini_entry, delete);
};

struct ini_file {
  std::string name;
  std::vector<ini_section*> sections;

  [[nodiscard]] std::vector<ini_section*> const& get_sections() const;

  // FIXME: This is actually what fixed the segfault. Deleting all constructors
  // and using the default move constructor?
  // FIXME: add correct constructors for the right methods

  ini_file() = delete;
  ini_file(const ini_file&) = delete;
  ini_file(ini_file&&) = default;

  ini_file& operator=(const ini_file&) = delete;
  ini_file& operator=(ini_file&&) = delete;

  ~ini_file();
};

class ini_parser {
  // conetent fields
  ini_file* inifile;
  std::string filename;
  std::string content;

  // position tracking

  // current line starts at 1 for error message not 0
  int current_line_ = 1;
  int current_line_pos_;
  int current_pos_;

  // implementation fields
  ini_section* current_section_{};

  void pop_section_();

  void drop_initial_whitespace();

  ini_section* try_consume_section();

  ini_entry* try_consume_entry();

  int try_consume_comment();

  void drop_to_newline();

  template <typename ch>
  void increment_pos_counts(ch n);

  ALL_5(ini_parser, delete);

 public:
  explicit ini_parser(std::string const& filename);

  ini_file parse();

  [[nodiscard]] std::string const& get_filename() const noexcept;

  ~ini_parser();
};

std::ostream& operator<<(std::ostream& os, ini_section const& self);

std::ostream& operator<<(std::ostream& os, ini_entry const& self);
}  // namespace tom

#endif  // PARSEINI_PARSEINI_H
