#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include "../parseini.h"

int main() {
  tom::ini_parser parser{"test/test.ini"};

  tom::ini_file f = parser.parse();

  std::cout << f.name << std::endl;
  std::cout << f << std::endl;

  return 0;
}
