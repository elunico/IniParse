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
  std::weak_ptr<ini_file> owner;
  std::weak_ptr<ini_section> parent;
  std::string name;
  std::vector<std::shared_ptr<ini_entry>> entries;

  std::shared_ptr<ini_entry> get_entry(std::string const& key);

  // returns a pair of <value, present>
  // If the key is present, then the second element is true
  // If the key is not present, then value == "" and present == false
  std::pair<std::string, bool> get_value(std::string const& key);

  ~ini_section();

  ini_section(std::weak_ptr<ini_file> owner,
              std::weak_ptr<ini_section> parent,
              std::string name,
              std::vector<std::shared_ptr<ini_entry>> entries)
      : owner(owner), parent(parent), name(name), entries(entries) {}
};

struct ini_entry {
  std::weak_ptr<ini_section> parent;
  std::string key;
  std::string value;

  ini_entry(std::weak_ptr<ini_section> parent, std::string key, std::string value): parent(parent), key(key), value(value) {}

  template <typename T, typename AdapterFunc>
  T adapt_value(AdapterFunc adapter) const {
    return adapter(value);
  }

  constexpr static auto const int_adapter = [](std::string value) {
    std::stringstream geek(value);
    int x = 0;
    geek >> x;
    return x;
  };

  constexpr static auto const long_adapter = [](std::string value) {
    std::stringstream geek(value);
    long x = 0l;
    geek >> x;
    return x;
  };

  constexpr static auto const double_adapter = [](std::string value) {
    std::stringstream geek(value);
    double x = 0.0;
    geek >> x;
    return x;
  };

  constexpr static auto const bool_adapter = [](std::string value) {
    std::stringstream geek(value);
    bool x = false;
    geek >> x;
    return x;
  };

  constexpr static auto const unsigned_adapter = [](std::string value) {
    std::stringstream geek(value);
    unsigned x = 0u;
    geek >> x;
    return x;
  };

  constexpr static auto const unsigned_long_adapter = [](std::string value) {
    std::stringstream geek(value);
    unsigned long x = 0UL;
    geek >> x;
    return x;
  };

  ALL_5(ini_entry, delete);
};

struct ini_file {
  std::string name;
  std::vector<std::shared_ptr<ini_section>> sections;

  [[nodiscard]] std::vector<std::shared_ptr<ini_section>> const& get_sections()
      const;

  std::shared_ptr<ini_section> get_section(std::string const& name);
  std::shared_ptr<ini_entry> get_entry(std::string const& key);

  friend std::ostream& operator<<(std::ostream&, ini_file const&);

  ini_file(std::string const& name_) : name(name_), sections(std::vector<std::shared_ptr<ini_section>>{}) {

  }

  ini_file() = delete;
  ini_file(const ini_file&) = delete;
  ini_file(ini_file&&) = default;

  ini_file& operator=(const ini_file&) = delete;
  ini_file& operator=(ini_file&&) = delete;

  ~ini_file();
};

class ini_parser {
  // conetent fields
  std::shared_ptr<ini_file> inifile;
  std::string filename;
  std::string content;

  // position tracking

  // current line starts at 1 for error message not 0
  int current_line_ = 1;
  int current_line_pos_ = 0;
  int current_pos_ = 0;

  // implementation fields
  std::shared_ptr<ini_section> current_section_{};

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
