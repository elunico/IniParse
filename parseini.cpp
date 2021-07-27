#ifndef PARSEINI_H
#define PARSEINI_H

#include "parseini.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

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

std::shared_ptr<ini_entry> ini_section::get_entry(std::string const& key) {
  for (auto& entry : entries) {
    if (entry->key == key) {
      return entry;
    }
  }
  return nullptr;
}

std::pair<std::string, bool> ini_section::get_value(std::string const& key) {
  for (auto& entry : entries) {
    if (entry->key == key) {
      return std::pair<std::string, bool>{entry->value, true};
    }
  }
  return std::pair<std::string, bool>{"", false};
}

ini_section::~ini_section() {
  // for (auto& v : entries)
  //   delete v;
  // entries.clear();
}

std::vector<std::shared_ptr<ini_section>> const& ini_file::get_sections()
    const {
  return sections;
}

std::shared_ptr<ini_entry> ini_file::get_entry(std::string const& key) {
  for (auto& section : sections) {
    for (auto& entry : section->entries) {
      if (entry->key == key) {
        return entry;
      }
    }
  }
  return nullptr;
}

std::shared_ptr<ini_section> ini_file::get_section(std::string const& name) {
  for (auto& section : sections) {
    if (section->name == name) {
      return section;
    }
  }
  return nullptr;
}

ini_file::~ini_file() {
  // for (auto& v : sections)
  //   delete v;
  // sections.clear();
}

ini_parser::ini_parser(std::string const& filename)
    : inifile(std::unique_ptr<ini_file>{new ini_file{filename}}),
      filename(filename),
      current_section_(nullptr) {
  content = readfile(filename);
}

ini_parser::~ini_parser() {}

std::string const& ini_parser::get_filename() const noexcept {
  return filename;
}

void ini_parser::pop_section_() {
  if (current_section_ != nullptr)
    current_section_ = current_section_->parent.lock();
}

std::string::size_type ini_parser::drop_initial_whitespace() {
  std::string::size_type n = 0;
  const std::locale& locale = std::locale{};
  while (std::isspace(content[n], locale)) {
    increment_pos_counts(content[n]);
    n++;
  }
  content.erase(0, n);
  content.shrink_to_fit();
  return n;
}

template <typename ch>
void ini_parser::increment_pos_counts(ch c) {
  current_pos_++;
  if (c == '\n') {
    current_line_++;
    current_line_pos_ = 0;
  }
}

std::string::size_type ini_parser::drop_to_newline() {
  std::string::size_type n = 0;
  while (std::isspace(content[n], std::locale{})) {
    increment_pos_counts(content[n]);
    n++;
  }
  content.erase(0, n);
  content.shrink_to_fit();
  return n;
}

ini_section* ini_parser::try_consume_section() {
  if (content[0] != '[') {
    return nullptr;
  }
  std::string::size_type n = 0;
  while (content[n] != ']') {
    increment_pos_counts(content[n]);
    n++;
  }

  std::string name = content.substr(1, n - 1);
  content.erase(0, n + 1);
  content.shrink_to_fit();
  return new ini_section{inifile.get(),
                         std::weak_ptr<ini_section>{current_section_}, name,
                         std::vector<std::shared_ptr<ini_entry>>{}};
}

template <typename ch>
int is_comment_char(ch c) {
  return c == static_cast<ch>(';') || c == static_cast<ch>('#');
}

template <typename ch>
int is_value_identifier_char(ch c) {
  return !is_comment_char(c) && c != '\n';
}

template <typename ch>
int is_key_identifier_char(ch c) {
  return !is_comment_char(c) && c != '\n' && c != '=';
}

bool ini_parser::try_consume_comment() {
  drop_initial_whitespace();
  // spleef whitespace will erase all the leading whitespace
  if (!is_comment_char(content[0]))
    return false;

  std::string::size_type n = 0;
  while (content[n] != '\n') {
    increment_pos_counts(content[n]);
    n++;
  }

  content.erase(0, n);
  content.shrink_to_fit();
  return true;
}

ini_entry* ini_parser::try_consume_entry() {
  auto& s = content;
  using size = std::string::size_type;
  auto locale = std::locale{};

  size ks = 0;
  // drop initial whitespace
  // might need to be removed as ini is pretty whitespace sensitive
  // all whitepsace in a key is preserved.
  //  while (std::isspace(s[ks], locale)) {
  //    increment_pos_counts(s[ks]);
  //    ks++;
  //  }

  size ke = ks;
  // after dropping initial whitespace, consume all valid key chars
  while (is_key_identifier_char(s[ke])) {
    increment_pos_counts(content[ke]);
    ke++;
  }

  std::string key = s.substr(ks, ke - ks);

  // If we reach the end of an identifier and don't find an equals, we have a
  // malformed line key with no value
  if (s[ke] != '=')
    return nullptr;

  // repeat the process for the value, but do not drop initial whitespace
  size vs = ke + 1;
  size ve = vs;
  while (is_value_identifier_char(s[ve])) {
    increment_pos_counts(s[ve]);
    ve++;
  }

  // two equal signs in a line is bad
//  if (s[ve] == '=')
//    return nullptr;

  std::string value = s.substr(vs, ve - vs);

  // cut the string to the new line so we can start fresh with the next line
  size eat = ve;
  while (eat < s.size() && s[eat] != '\n') {
    increment_pos_counts(s[eat]);
    eat++;
  }

  s.erase(0, eat);
  s.shrink_to_fit();
  return new ini_entry{current_section_.get(), key, value};
}

ini_file ini_parser::parse() {
  while (!content.empty()) {
    // clear initial whitespace
    drop_initial_whitespace();

    // look for a section
    auto sec = try_consume_section();

    if (sec != nullptr) {
      if (current_section_ != nullptr)
        inifile->sections.push_back(current_section_);

      current_section_ = std::shared_ptr<ini_section>(sec);

      // drop the rest of the line
      drop_to_newline();
      continue;
    } else {
      if (current_section_ == nullptr)
        current_section_ = std::make_shared<ini_section>(
            inifile.get(), std::weak_ptr<ini_section>{},
            std::string("<Default Section>"),
            std::vector<std::shared_ptr<ini_entry>>{});
    }

    // try to consume an entry on the next line
    auto entry = try_consume_entry();
    if (entry != nullptr) {
      current_section_->entries.push_back(std::shared_ptr<ini_entry>{entry});
      drop_to_newline();
      continue;
    }

    // if the entry fails to parse try to consume a comment
    bool i = try_consume_comment();
    if (i) {
      drop_to_newline();
      continue;
    }

    std::stringstream s;
    s << "INI Parsing of " << get_filename()
      << " has failed at line: " << current_line_
      << ", col: " << current_line_pos_ << " (" << current_pos_ << ")";
    throw std::invalid_argument(s.str());
  }

  inifile->sections.push_back(current_section_);

  return std::move(*inifile);
}

std::ostream& operator<<(std::ostream& os, ini_section const& self) {
  os << "[" << self.name << "]\n";
  for (auto const& entry : self.entries) {
    os << "\t" << *entry << "\n";
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, ini_entry const& self) {
  os << "\t" << self.key << "=" << self.value;
  return os;
}

std::ostream& operator<<(std::ostream& os, ini_file const& self) {
  os << self.name << " (sections: " << self.sections.size() << ")\n";
  for (auto const& section : self.sections) {
    os << *section << "\n";
  }
  return os;
}

}  // namespace tom

#endif
