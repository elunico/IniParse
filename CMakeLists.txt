cmake_minimum_required(VERSION 3.21)
project(ParseIni)


set(CMAKE_CXX_STANDARD 17)

add_library(ParseIni parseini.cpp parseini.h)


add_executable(parsetest test/test.cpp parseini.h)
target_link_libraries(parsetest ParseIni)
