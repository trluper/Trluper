# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/project/Trluper

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/project/Trluper/TEST

# Include any dependencies generated for this target.
include CMakeFiles/mysql.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mysql.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mysql.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mysql.dir/flags.make

CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o: CMakeFiles/mysql.dir/flags.make
CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o: ../test/test_mysqlpool.cc
CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o: CMakeFiles/mysql.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o -MF CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o.d -o CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o -c /home/project/Trluper/test/test_mysqlpool.cc

CMakeFiles/mysql.dir/test/test_mysqlpool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mysql.dir/test/test_mysqlpool.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/test/test_mysqlpool.cc > CMakeFiles/mysql.dir/test/test_mysqlpool.cc.i

CMakeFiles/mysql.dir/test/test_mysqlpool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mysql.dir/test/test_mysqlpool.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/test/test_mysqlpool.cc -o CMakeFiles/mysql.dir/test/test_mysqlpool.cc.s

# Object files for target mysql
mysql_OBJECTS = \
"CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o"

# External object files for target mysql
mysql_EXTERNAL_OBJECTS =

mysql: CMakeFiles/mysql.dir/test/test_mysqlpool.cc.o
mysql: CMakeFiles/mysql.dir/build.make
mysql: libTrluper.so
mysql: /usr/lib/x86_64-linux-gnu/libyaml-cpp.so.0.7.0
mysql: CMakeFiles/mysql.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mysql"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mysql.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mysql.dir/build: mysql
.PHONY : CMakeFiles/mysql.dir/build

CMakeFiles/mysql.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mysql.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mysql.dir/clean

CMakeFiles/mysql.dir/depend:
	cd /home/project/Trluper/TEST && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/project/Trluper /home/project/Trluper /home/project/Trluper/TEST /home/project/Trluper/TEST /home/project/Trluper/TEST/CMakeFiles/mysql.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mysql.dir/depend
