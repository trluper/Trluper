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
include CMakeFiles/coroutine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/coroutine.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/coroutine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/coroutine.dir/flags.make

CMakeFiles/coroutine.dir/test/test_coroutine.cc.o: CMakeFiles/coroutine.dir/flags.make
CMakeFiles/coroutine.dir/test/test_coroutine.cc.o: ../test/test_coroutine.cc
CMakeFiles/coroutine.dir/test/test_coroutine.cc.o: CMakeFiles/coroutine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/coroutine.dir/test/test_coroutine.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/coroutine.dir/test/test_coroutine.cc.o -MF CMakeFiles/coroutine.dir/test/test_coroutine.cc.o.d -o CMakeFiles/coroutine.dir/test/test_coroutine.cc.o -c /home/project/Trluper/test/test_coroutine.cc

CMakeFiles/coroutine.dir/test/test_coroutine.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/coroutine.dir/test/test_coroutine.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/test/test_coroutine.cc > CMakeFiles/coroutine.dir/test/test_coroutine.cc.i

CMakeFiles/coroutine.dir/test/test_coroutine.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/coroutine.dir/test/test_coroutine.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/test/test_coroutine.cc -o CMakeFiles/coroutine.dir/test/test_coroutine.cc.s

# Object files for target coroutine
coroutine_OBJECTS = \
"CMakeFiles/coroutine.dir/test/test_coroutine.cc.o"

# External object files for target coroutine
coroutine_EXTERNAL_OBJECTS =

coroutine: CMakeFiles/coroutine.dir/test/test_coroutine.cc.o
coroutine: CMakeFiles/coroutine.dir/build.make
coroutine: libTrluper.so
coroutine: /usr/lib/x86_64-linux-gnu/libyaml-cpp.so.0.7.0
coroutine: CMakeFiles/coroutine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable coroutine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/coroutine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/coroutine.dir/build: coroutine
.PHONY : CMakeFiles/coroutine.dir/build

CMakeFiles/coroutine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/coroutine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/coroutine.dir/clean

CMakeFiles/coroutine.dir/depend:
	cd /home/project/Trluper/TEST && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/project/Trluper /home/project/Trluper /home/project/Trluper/TEST /home/project/Trluper/TEST /home/project/Trluper/TEST/CMakeFiles/coroutine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/coroutine.dir/depend
