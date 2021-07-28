#ifndef PARSEINI_PARSEINI_H
#define PARSEINI_PARSEINI_H

#include <fstream>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

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

struct ini_section : std::enable_shared_from_this<ini_section> {
private:
    std::vector<std::shared_ptr<ini_entry>>                     entries_;
    std::unordered_map<std::string, std::shared_ptr<ini_entry>> emap;

public:
    std::string                name;
    std::weak_ptr<ini_section> parent;
    std::weak_ptr<ini_file>    owner;

    std::shared_ptr<ini_entry> get_entry(std::string const& key) const;

    std::vector<std::shared_ptr<ini_entry>> const& entries() const noexcept;

    // you must add entries using the add_entry method. NEVER directly manipulate the map or vector
    bool add_entry(std::string const& key, std::string const& value);

    bool add_entry(std::shared_ptr<ini_entry> const& entry);

    // you must remove entries using the add_entry method. NEVER directly manipulate the map or vector
    bool remove_entry(std::string const& key);

    // returns a pair of <value_, present>
    // If the key_ is present, then the second element is true
    // If the key_ is not present, then value_ == "" and present == false
    std::pair<std::string, bool> get_value(std::string const& key) const;

    ~ini_section();

    ini_section(
        std::weak_ptr<ini_file> owner, std::weak_ptr<ini_section> parent, std::string name,
        std::vector<std::shared_ptr<ini_entry>> entries
    ) : owner(owner), parent(parent), name(name), entries_(entries) { }

};

struct ini_entry {
private:
    std::string key_;
    std::string value_;

public:
    std::weak_ptr<ini_section> parent;

    ini_entry(std::weak_ptr<ini_section> parent, std::string key, std::string value) :
        parent(parent), key_(key), value_(value) { }

    std::string const& key() const noexcept;

    std::string const& value() const noexcept;

    template <typename T, typename AdapterFunc>
    T adapt_value(AdapterFunc adapter) const {
        return adapter(value_);
    }

    constexpr static auto const int_adapter = [](std::string value) {
        std::stringstream geek(value);
        int               x = 0;
        geek >> x;
        return x;
    };

    constexpr static auto const long_adapter = [](std::string value) {
        std::stringstream geek(value);
        long              x = 0l;
        geek >> x;
        return x;
    };

    constexpr static auto const double_adapter = [](std::string value) {
        std::stringstream geek(value);
        double            x = 0.0;
        geek >> x;
        return x;
    };

    constexpr static auto const bool_adapter = [](std::string value) {
        std::stringstream geek(value);
        bool              x = false;
        geek >> x;
        return x;
    };

    constexpr static auto const unsigned_adapter = [](std::string value) {
        std::stringstream geek(value);
        unsigned          x = 0u;
        geek >> x;
        return x;
    };

    constexpr static auto const unsigned_long_adapter = [](std::string value) {
        std::stringstream geek(value);
        unsigned long     x = 0UL;
        geek >> x;
        return x;
    };

    ALL_5(ini_entry, delete);
};

struct ini_file : std::enable_shared_from_this<ini_file> {
private:
    std::vector<std::shared_ptr<ini_section>>                     sections;
    std::unordered_map<std::string, std::shared_ptr<ini_section>> smap;

public:
    std::string const name;

    [[nodiscard]] std::vector<std::shared_ptr<ini_section>> const& get_sections() const;

    std::shared_ptr<ini_section> get_section(std::string const& name) const;

    std::shared_ptr<ini_entry> get_entry(std::string const& key) const;

    bool add_section(const std::string& section_name, std::string* parent_name);

    bool add_section(std::shared_ptr<ini_section> section);

    friend std::ostream& operator <<(std::ostream&, ini_file const&);

    ini_file(std::string const& name_) : name(name_), sections(std::vector<std::shared_ptr<ini_section>>{ }) {

    }

    ini_file() = delete;

    ini_file(const ini_file&) = delete;

    ini_file(ini_file&&) = default;

    ini_file& operator =(const ini_file&) = delete;

    ini_file& operator =(ini_file&&) = delete;

    ~ini_file();
};

class ini_parser {
    // conetent fields
    std::shared_ptr<ini_file> inifile;
    std::string               filename;
    std::string               content;

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
    // create 1 constructor for filename
    explicit ini_parser(std::string const& filename);

    // the main method the user of this class will call. Takes the content of the file and parses it into the ini_file data structure
    ini_file parse();

    // accessor method for the filename field
    [[nodiscard]] std::string const& get_filename() const noexcept;

    // default destructor
    ~ini_parser();
};

std::ostream& operator <<(std::ostream& os, ini_section const& self);

std::ostream& operator <<(std::ostream& os, ini_entry const& self);
}  // namespace tom

#endif  // PARSEINI_PARSEINI_H
