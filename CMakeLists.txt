cmake_minimum_required(VERSION 3.21)
project(ParseIni)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O0 -g")
add_library(ParseIni Source/utils.h Source/ini_entry.cpp Source/ini_entry.h Source/ini_file.cpp Source/ini_file.h Source/ini_parser.cpp Source/ini_parser.h Source/ini_section.cpp Source/ini_section.h Source/utils.cpp Source/parse_error.cpp Source/parse_error.h)

set(CMAKE_CXX_STANDARD 17)

add_executable(parsetest test/test.cpp Source/utils.h Source/ini_entry.cpp Source/ini_entry.h Source/ini_file.cpp Source/ini_file.h Source/ini_parser.cpp Source/ini_parser.h Source/ini_section.cpp Source/ini_section.h Source/utils.cpp Source/parse_error.cpp Source/parse_error.h)
target_link_libraries(parsetest ParseIni)
