#ifndef PARSEINI_UTILS_H
#define PARSEINI_UTILS_H

#include <fstream>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <sstream>
#include <utility>

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
struct ini_section;

static std::string readfile(std::string const& filename) {
    std::string       line;
    std::ifstream     myfile(filename);
    std::stringstream text{ };
    if (myfile.is_open()) {
        while (getline(myfile, line))
            text << line << '\n';

        myfile.close();
    } else {
        abort();
    }
    return text.str();
}

std::ostream& operator <<(std::ostream& os, ini_entry const& self);

std::ostream& operator <<(std::ostream& os, ini_section const& self);

template<typename T, typename K>
T get_or_nullptr(std::unordered_map<K, T> const& map, K const& key)  noexcept {
    if (map.find(key) == map.end()) {
        return nullptr;
    }
    return map.at(key);
}

}


#endif  // PARSEINI_UTILS_H
