# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.21.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.21.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/thomaspovinelli/CLionProjects/ParseIni

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/thomaspovinelli/CLionProjects/ParseIni

# Include any dependencies generated for this target.
include CMakeFiles/parsetest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/parsetest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/parsetest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parsetest.dir/flags.make

CMakeFiles/parsetest.dir/test/test.cpp.o: CMakeFiles/parsetest.dir/flags.make
CMakeFiles/parsetest.dir/test/test.cpp.o: test/test.cpp
CMakeFiles/parsetest.dir/test/test.cpp.o: CMakeFiles/parsetest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomaspovinelli/CLionProjects/ParseIni/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/parsetest.dir/test/test.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/parsetest.dir/test/test.cpp.o -MF CMakeFiles/parsetest.dir/test/test.cpp.o.d -o CMakeFiles/parsetest.dir/test/test.cpp.o -c /Users/thomaspovinelli/CLionProjects/ParseIni/test/test.cpp

CMakeFiles/parsetest.dir/test/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parsetest.dir/test/test.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/thomaspovinelli/CLionProjects/ParseIni/test/test.cpp > CMakeFiles/parsetest.dir/test/test.cpp.i

CMakeFiles/parsetest.dir/test/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parsetest.dir/test/test.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/thomaspovinelli/CLionProjects/ParseIni/test/test.cpp -o CMakeFiles/parsetest.dir/test/test.cpp.s

# Object files for target parsetest
parsetest_OBJECTS = \
"CMakeFiles/parsetest.dir/test/test.cpp.o"

# External object files for target parsetest
parsetest_EXTERNAL_OBJECTS =

parsetest: CMakeFiles/parsetest.dir/test/test.cpp.o
parsetest: CMakeFiles/parsetest.dir/build.make
parsetest: libParseIni.a
parsetest: CMakeFiles/parsetest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/thomaspovinelli/CLionProjects/ParseIni/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable parsetest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parsetest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parsetest.dir/build: parsetest
.PHONY : CMakeFiles/parsetest.dir/build

CMakeFiles/parsetest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parsetest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parsetest.dir/clean

CMakeFiles/parsetest.dir/depend:
	cd /Users/thomaspovinelli/CLionProjects/ParseIni && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thomaspovinelli/CLionProjects/ParseIni /Users/thomaspovinelli/CLionProjects/ParseIni /Users/thomaspovinelli/CLionProjects/ParseIni /Users/thomaspovinelli/CLionProjects/ParseIni /Users/thomaspovinelli/CLionProjects/ParseIni/CMakeFiles/parsetest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parsetest.dir/depend

