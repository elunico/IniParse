#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include "../parseini.h"

std::string readfile(std::string const& filename) {
  std::string line;
  std::ifstream myfile(filename);
  std::stringstream text{};
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      text << line << '\n';
    }
    myfile.close();
  } else {
    abort();
  }
  return text.str();
}

int main() {
  std::string text =
      readfile("/Users/thomaspovinelli/CLionProjects/ParseIni/test/test.ini");
  std::cout << text << std::endl;

  auto file = tom::ini_parser{
      "/Users/thomaspovinelli/CLionProjects/ParseIni/test/test.ini"};

  tom::ini_file f = file.parse();

  std::cout << f.name << std::endl;
  for (auto const& a : f.sections) {
    std::cout << a << ", ";
  }

  return 0;
}
