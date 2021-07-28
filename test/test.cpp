#include <array>
#include <cassert>
#include <iostream>
#include <istream>
#include <stdexcept>
#include "../Source/utils.h"
#include "../Source/ini_entry.h"
#include "../Source/ini_parser.h"
#include "../Source/ini_file.h"
#include "../Source/ini_parser.h"

namespace {
inline std::string quote(std::string const& s) {
    return "\"" + s + "\"";
}
} //namespace

int main() {
    tom::ini_parser parser{ "test/test.ini" };

    tom::ini_file f = parser.parse();

    std::cout << f.name << std::endl;
    std::cout << f << std::endl;

    auto section = f.get_section("FTP");

    assert(section->name == "FTP");

    auto port = section->get_entry("FTPPort");

    assert(port->key() == "FTPPort");
    assert(port->value() == "21");

    std::cout << "In FTP Section: " << port->key() << "=" << port->value() << std::endl;

    std::cout << "FTPDir is equal to \"" << std::get<0>(section->get_value("FTPDir")) << "\"\n";

    auto entry = f.get_entry("PrimaryIP");
    assert(entry->key() == "PrimaryIP");
    assert(entry->value() == "192.168.0.13");
    assert(entry->parent.lock()->name == "BACKUP_SERVERS");

    std::cout << "Setting " << quote(entry->key()) << " has value() " << quote(entry->value()) << " in section "
              << quote(entry->parent.lock()->name) << "\n";

    std::cout << "Value = " << port->adapt_value<int>() << std::endl;

    return 0;
}
