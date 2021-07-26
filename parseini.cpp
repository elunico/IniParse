#ifndef PARSEINI_H
#define PARSEINI_H

#include "parseini.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <stdexcept>

namespace tom {
static std::string readfile(std::string const& filename) {
  std::string line;
  std::ifstream myfile(filename);
  std::stringstream text{};
  if (myfile.is_open()) {
    while (getline(myfile, line))
      text << line << '\n';

    myfile.close();
  } else {
    abort();
  }
  return text.str();
}

ini_section::~ini_section() {
  for (auto& v : entries)
    delete v;
  entries.clear();
}

std::vector<ini_section*> const& ini_file::get_sections() const {
  return sections;
}

ini_file::~ini_file() {
  for (auto& v : sections)
    delete v;
  sections.clear();
}

ini_parser::ini_parser(std::string const& filename)
    : inifile(new ini_file{filename}),
      filename(filename),
      current_section_(nullptr) {
  content = readfile(filename);
}

ini_parser::~ini_parser() {
  delete inifile;
}

std::string ini_parser::get_filename() const noexcept {
  return filename;
}

ini_section ini_parser::parse_section(ini_section* parent, std::string& s) {
  const auto i = s.find_first_of('[');
  const auto j = s.find_first_of(']');

  if (i == std::string::npos)
    return ini_section{inifile, parent, ""};

  auto v = s.substr(i + 1, j - 1);

  s.erase(0, j + 1);
  s.shrink_to_fit();

  return ini_section{inifile, parent, v};
}

ini_entry ini_parser::parse_entry(ini_section* parent, std::string& s) {
  using size = std::string::size_type;
  auto locale = std::locale{};

  size ks = 0;
  while (std::isspace(s[ks], locale))
    ks++;

  size ke = ks;
  while (std::isalnum(s[ke], locale) || std::isspace(s[ke], locale))
    ke++;

  std::string key = s.substr(ks, ke - ks);

  assert(s[ke] == '=');

  size vs = ke + 1;
  size ve = vs;
  while (std::isalnum(s[ve], locale) ||
         std::isspace(s[ve], locale) && s[ve] != '\n')
    ve++;

  std::string value = s.substr(vs, ve - vs);

  size eat = ve;
  while (eat < s.size() && s[eat++] != '\n')
    ;

  s.erase(0, eat);
  s.shrink_to_fit();

  return ini_entry{parent, key, value};
}

void ini_parser::pop_section_() {
  if (current_section_ != nullptr)
    current_section_ = current_section_->parent;
}

void ini_parser::spleef_whitespace() {
  std::string::size_type n = 0;
  while (std::isspace(content[n], std::locale{}))
    n++;
  content.erase(0, n);
  content.shrink_to_fit();
}

void ini_parser::consume_to_newline() {
  std::string::size_type n = 0;
  while (std::isspace(content[n], std::locale{}))
    n++;
  content.erase(0, n);
  content.shrink_to_fit();
}

ini_section* ini_parser::consume_section_optional() {
  if (content[0] != '[') {
    return nullptr;
  }
  std::string::size_type n = 0;
  while (content[n] != ']')
    n++;

  std::string name = content.substr(1, n - 1);
  content.erase(0, n + 1);
  content.shrink_to_fit();
  return new ini_section{inifile, current_section_, name,
                         std::vector<ini_entry*>{}};
}

ini_entry* ini_parser::consume_entry() {
  auto& s = content;
  using size = std::string::size_type;
  auto locale = std::locale{};

  size ks = 0;
  while (std::isspace(s[ks], locale))
    ks++;

  size ke = ks;
  while (std::isalnum(s[ke], locale) || std::isspace(s[ke], locale)) {
    ke++;
  }

  std::string key = s.substr(ks, ke - ks);

  assert(s[ke] == '=');

  size vs = ke + 1;
  size ve = vs;
  while (std::isalnum(s[ve], locale) ||
         std::isspace(s[ve], locale) && s[ve] != '\n') {
    ve++;
  }

  std::string value = s.substr(vs, ve - vs);

  size eat = ve;
  while (eat < s.size() && s[eat++] != '\n')
    ;

  s.erase(0, eat);
  s.shrink_to_fit();
  // TODO: Big bug with address of here
  return new ini_entry{current_section_, key, value};
}

ini_file ini_parser::parse() {
  while (!content.empty()) {
    spleef_whitespace();
    auto sec = consume_section_optional();
    if (sec != nullptr) {
      if (current_section_ != nullptr)
        inifile->sections.push_back(current_section_);

      current_section_ = sec;
    } else {
      if (current_section_ == nullptr)
        current_section_ =
            new ini_section{inifile, nullptr, "<Default Section>"};
    }
    consume_to_newline();
    auto entry = consume_entry();
    current_section_->entries.push_back(entry);

    consume_to_newline();
  }

  inifile->sections.push_back(current_section_);

  return std::move(*inifile);
}

std::ostream& operator<<(std::ostream& os, ini_section const& self) {
  os << "ini_section(" << self.name
     << ") [parent: " << (self.parent != nullptr ? self.parent->name : "null")
     << "]\n\tentries: ";
  for (const auto& a : self.entries)
    os << a << ", ";

  os << "|;";
  return os;
}

std::ostream& operator<<(std::ostream& os, ini_entry const& self) {
  os << "ini_entry(" << self.key << "=" << self.value
     << ") [p: " << (self.parent != nullptr ? self.parent->name : "null")
     << "]@";
  return os;
}
}  // namespace tom

#endif
