#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include "../parseini.h"

int main() {
  tom::ini_parser parser{"test/test.ini"};

  tom::ini_file f = parser.parse();

  std::cout << f.name << std::endl;
  std::cout << f << std::endl;

  auto section = f.get_section("FTP");

  assert(section->name == "FTP");

  auto port = section->get_entry("FTPPort");

  assert(port->key == "FTPPort");
  assert(port->value == "21");

  std::cout << "In FTP Section: " << port->key << "=" << port->value
            << std::endl;

  std::cout << "FTPDir is equal to \""
            << std::get<0>(section->get_value("FTPDir")) << "\"\n";

  auto entry = f.get_entry("PrimaryIP");
  assert(entry->key == "PrimaryIP");
  assert(entry->value == "192.168.0.13");
  assert(entry->parent.lock()->name == "BACKUP_SERVERS");

  std::cout << "Setting \"" << entry->key << "\" has value \"" << entry->value
            << "\" in section \"" << entry->parent.lock()->name << "\"\n";

  return 0;
}
