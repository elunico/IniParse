//
// Created by Thomas Povinelli on 7/28/21.
//

#include "ini_section.h"
#include "ini_entry.h"

namespace tom {

std::shared_ptr<ini_entry> ini_section::get_entry(
    std::string const& key) const {
  return get_or_nullptr(emap, key);
}

std::pair<std::string, bool> ini_section::get_value(
    std::string const& key) const {
  auto entry = get_entry(key);
  if (entry == nullptr) {
    return std::pair<std::string, bool>{"", false};
  } else {
    return std::pair<std::string, bool>{entry->value(), true};
  }
}

bool ini_section::add_entry(std::string const& key, std::string const& value) {
  auto entry = std::make_shared<ini_entry>(this->weak_from_this(), key, value);
  return add_entry(entry);
}

bool ini_section::add_entry(std::shared_ptr<ini_entry> const& entry) {
  auto const& key = entry->key();
  auto result = false;
  if (emap.find(key) != emap.end())
    result = true;

  emap[key] = entry;
  return result;
}

bool ini_section::remove_entry(std::string const& key) {
  auto v = (emap.erase(key) >= 1);
  return v;
}

std::string const& ini_section::operator[](const std::string& key) {
  auto entry = get_or_nullptr(emap, key);
  if (entry == nullptr) {
    add_entry(key, "");
    entry = get_or_nullptr(emap, key);
  }
  return entry->value();
}

std::unique_ptr<std::vector<std::shared_ptr<ini_entry>>> ini_section::entries()
    const noexcept {
  auto v = new std::vector<std::shared_ptr<ini_entry>>{};
  std::for_each(std::begin(emap), std::end(emap),
                [&v](auto a) { v->push_back(std::get<1>(a)); });

  return std::unique_ptr<std::vector<std::shared_ptr<ini_entry>>>{v};
}

ini_section::~ini_section() = default;

}  // namespace tom
