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
  tom::ini_parser file{"test/test.ini"};

  tom::ini_file f = file.parse();

  std::cout << f.name << std::endl;
  std::cout << f << std::endl;

  return 0;
}
