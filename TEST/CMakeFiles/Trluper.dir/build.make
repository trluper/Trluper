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
include CMakeFiles/Trluper.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Trluper.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Trluper.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Trluper.dir/flags.make

CMakeFiles/Trluper.dir/src/message.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/message.cc.o: ../src/message.cc
CMakeFiles/Trluper.dir/src/message.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Trluper.dir/src/message.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/message.cc.o -MF CMakeFiles/Trluper.dir/src/message.cc.o.d -o CMakeFiles/Trluper.dir/src/message.cc.o -c /home/project/Trluper/src/message.cc

CMakeFiles/Trluper.dir/src/message.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/message.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/message.cc > CMakeFiles/Trluper.dir/src/message.cc.i

CMakeFiles/Trluper.dir/src/message.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/message.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/message.cc -o CMakeFiles/Trluper.dir/src/message.cc.s

CMakeFiles/Trluper.dir/src/server.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/server.cc.o: ../src/server.cc
CMakeFiles/Trluper.dir/src/server.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Trluper.dir/src/server.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/server.cc.o -MF CMakeFiles/Trluper.dir/src/server.cc.o.d -o CMakeFiles/Trluper.dir/src/server.cc.o -c /home/project/Trluper/src/server.cc

CMakeFiles/Trluper.dir/src/server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/server.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/server.cc > CMakeFiles/Trluper.dir/src/server.cc.i

CMakeFiles/Trluper.dir/src/server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/server.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/server.cc -o CMakeFiles/Trluper.dir/src/server.cc.s

CMakeFiles/Trluper.dir/src/abstractHandle.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/abstractHandle.cc.o: ../src/abstractHandle.cc
CMakeFiles/Trluper.dir/src/abstractHandle.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Trluper.dir/src/abstractHandle.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/abstractHandle.cc.o -MF CMakeFiles/Trluper.dir/src/abstractHandle.cc.o.d -o CMakeFiles/Trluper.dir/src/abstractHandle.cc.o -c /home/project/Trluper/src/abstractHandle.cc

CMakeFiles/Trluper.dir/src/abstractHandle.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/abstractHandle.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/abstractHandle.cc > CMakeFiles/Trluper.dir/src/abstractHandle.cc.i

CMakeFiles/Trluper.dir/src/abstractHandle.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/abstractHandle.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/abstractHandle.cc -o CMakeFiles/Trluper.dir/src/abstractHandle.cc.s

CMakeFiles/Trluper.dir/src/connections.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/connections.cc.o: ../src/connections.cc
CMakeFiles/Trluper.dir/src/connections.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Trluper.dir/src/connections.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/connections.cc.o -MF CMakeFiles/Trluper.dir/src/connections.cc.o.d -o CMakeFiles/Trluper.dir/src/connections.cc.o -c /home/project/Trluper/src/connections.cc

CMakeFiles/Trluper.dir/src/connections.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/connections.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/connections.cc > CMakeFiles/Trluper.dir/src/connections.cc.i

CMakeFiles/Trluper.dir/src/connections.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/connections.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/connections.cc -o CMakeFiles/Trluper.dir/src/connections.cc.s

CMakeFiles/Trluper.dir/src/dataProcess.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/dataProcess.cc.o: ../src/dataProcess.cc
CMakeFiles/Trluper.dir/src/dataProcess.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Trluper.dir/src/dataProcess.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/dataProcess.cc.o -MF CMakeFiles/Trluper.dir/src/dataProcess.cc.o.d -o CMakeFiles/Trluper.dir/src/dataProcess.cc.o -c /home/project/Trluper/src/dataProcess.cc

CMakeFiles/Trluper.dir/src/dataProcess.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/dataProcess.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/dataProcess.cc > CMakeFiles/Trluper.dir/src/dataProcess.cc.i

CMakeFiles/Trluper.dir/src/dataProcess.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/dataProcess.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/dataProcess.cc -o CMakeFiles/Trluper.dir/src/dataProcess.cc.s

CMakeFiles/Trluper.dir/src/fapplycations.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/fapplycations.cc.o: ../src/fapplycations.cc
CMakeFiles/Trluper.dir/src/fapplycations.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Trluper.dir/src/fapplycations.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/fapplycations.cc.o -MF CMakeFiles/Trluper.dir/src/fapplycations.cc.o.d -o CMakeFiles/Trluper.dir/src/fapplycations.cc.o -c /home/project/Trluper/src/fapplycations.cc

CMakeFiles/Trluper.dir/src/fapplycations.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/fapplycations.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/fapplycations.cc > CMakeFiles/Trluper.dir/src/fapplycations.cc.i

CMakeFiles/Trluper.dir/src/fapplycations.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/fapplycations.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/fapplycations.cc -o CMakeFiles/Trluper.dir/src/fapplycations.cc.s

CMakeFiles/Trluper.dir/src/thread.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/thread.cc.o: ../src/thread.cc
CMakeFiles/Trluper.dir/src/thread.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Trluper.dir/src/thread.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/thread.cc.o -MF CMakeFiles/Trluper.dir/src/thread.cc.o.d -o CMakeFiles/Trluper.dir/src/thread.cc.o -c /home/project/Trluper/src/thread.cc

CMakeFiles/Trluper.dir/src/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/thread.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/thread.cc > CMakeFiles/Trluper.dir/src/thread.cc.i

CMakeFiles/Trluper.dir/src/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/thread.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/thread.cc -o CMakeFiles/Trluper.dir/src/thread.cc.s

CMakeFiles/Trluper.dir/src/threadPool.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/threadPool.cc.o: ../src/threadPool.cc
CMakeFiles/Trluper.dir/src/threadPool.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Trluper.dir/src/threadPool.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/threadPool.cc.o -MF CMakeFiles/Trluper.dir/src/threadPool.cc.o.d -o CMakeFiles/Trluper.dir/src/threadPool.cc.o -c /home/project/Trluper/src/threadPool.cc

CMakeFiles/Trluper.dir/src/threadPool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/threadPool.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/threadPool.cc > CMakeFiles/Trluper.dir/src/threadPool.cc.i

CMakeFiles/Trluper.dir/src/threadPool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/threadPool.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/threadPool.cc -o CMakeFiles/Trluper.dir/src/threadPool.cc.s

CMakeFiles/Trluper.dir/src/timer.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/timer.cc.o: ../src/timer.cc
CMakeFiles/Trluper.dir/src/timer.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Trluper.dir/src/timer.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/timer.cc.o -MF CMakeFiles/Trluper.dir/src/timer.cc.o.d -o CMakeFiles/Trluper.dir/src/timer.cc.o -c /home/project/Trluper/src/timer.cc

CMakeFiles/Trluper.dir/src/timer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/timer.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/timer.cc > CMakeFiles/Trluper.dir/src/timer.cc.i

CMakeFiles/Trluper.dir/src/timer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/timer.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/timer.cc -o CMakeFiles/Trluper.dir/src/timer.cc.s

CMakeFiles/Trluper.dir/src/util.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/util.cc.o: ../src/util.cc
CMakeFiles/Trluper.dir/src/util.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Trluper.dir/src/util.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/util.cc.o -MF CMakeFiles/Trluper.dir/src/util.cc.o.d -o CMakeFiles/Trluper.dir/src/util.cc.o -c /home/project/Trluper/src/util.cc

CMakeFiles/Trluper.dir/src/util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/util.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/util.cc > CMakeFiles/Trluper.dir/src/util.cc.i

CMakeFiles/Trluper.dir/src/util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/util.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/util.cc -o CMakeFiles/Trluper.dir/src/util.cc.s

CMakeFiles/Trluper.dir/src/log.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/log.cc.o: ../src/log.cc
CMakeFiles/Trluper.dir/src/log.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Trluper.dir/src/log.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/log.cc.o -MF CMakeFiles/Trluper.dir/src/log.cc.o.d -o CMakeFiles/Trluper.dir/src/log.cc.o -c /home/project/Trluper/src/log.cc

CMakeFiles/Trluper.dir/src/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/log.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/log.cc > CMakeFiles/Trluper.dir/src/log.cc.i

CMakeFiles/Trluper.dir/src/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/log.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/log.cc -o CMakeFiles/Trluper.dir/src/log.cc.s

CMakeFiles/Trluper.dir/src/mysqlpool.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/src/mysqlpool.cc.o: ../src/mysqlpool.cc
CMakeFiles/Trluper.dir/src/mysqlpool.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Trluper.dir/src/mysqlpool.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/src/mysqlpool.cc.o -MF CMakeFiles/Trluper.dir/src/mysqlpool.cc.o.d -o CMakeFiles/Trluper.dir/src/mysqlpool.cc.o -c /home/project/Trluper/src/mysqlpool.cc

CMakeFiles/Trluper.dir/src/mysqlpool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/src/mysqlpool.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/src/mysqlpool.cc > CMakeFiles/Trluper.dir/src/mysqlpool.cc.i

CMakeFiles/Trluper.dir/src/mysqlpool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/src/mysqlpool.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/src/mysqlpool.cc -o CMakeFiles/Trluper.dir/src/mysqlpool.cc.s

CMakeFiles/Trluper.dir/http/http.cc.o: CMakeFiles/Trluper.dir/flags.make
CMakeFiles/Trluper.dir/http/http.cc.o: ../http/http.cc
CMakeFiles/Trluper.dir/http/http.cc.o: CMakeFiles/Trluper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Trluper.dir/http/http.cc.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Trluper.dir/http/http.cc.o -MF CMakeFiles/Trluper.dir/http/http.cc.o.d -o CMakeFiles/Trluper.dir/http/http.cc.o -c /home/project/Trluper/http/http.cc

CMakeFiles/Trluper.dir/http/http.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Trluper.dir/http/http.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/project/Trluper/http/http.cc > CMakeFiles/Trluper.dir/http/http.cc.i

CMakeFiles/Trluper.dir/http/http.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Trluper.dir/http/http.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/project/Trluper/http/http.cc -o CMakeFiles/Trluper.dir/http/http.cc.s

# Object files for target Trluper
Trluper_OBJECTS = \
"CMakeFiles/Trluper.dir/src/message.cc.o" \
"CMakeFiles/Trluper.dir/src/server.cc.o" \
"CMakeFiles/Trluper.dir/src/abstractHandle.cc.o" \
"CMakeFiles/Trluper.dir/src/connections.cc.o" \
"CMakeFiles/Trluper.dir/src/dataProcess.cc.o" \
"CMakeFiles/Trluper.dir/src/fapplycations.cc.o" \
"CMakeFiles/Trluper.dir/src/thread.cc.o" \
"CMakeFiles/Trluper.dir/src/threadPool.cc.o" \
"CMakeFiles/Trluper.dir/src/timer.cc.o" \
"CMakeFiles/Trluper.dir/src/util.cc.o" \
"CMakeFiles/Trluper.dir/src/log.cc.o" \
"CMakeFiles/Trluper.dir/src/mysqlpool.cc.o" \
"CMakeFiles/Trluper.dir/http/http.cc.o"

# External object files for target Trluper
Trluper_EXTERNAL_OBJECTS =

libTrluper.so: CMakeFiles/Trluper.dir/src/message.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/server.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/abstractHandle.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/connections.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/dataProcess.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/fapplycations.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/thread.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/threadPool.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/timer.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/util.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/log.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/src/mysqlpool.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/http/http.cc.o
libTrluper.so: CMakeFiles/Trluper.dir/build.make
libTrluper.so: CMakeFiles/Trluper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/project/Trluper/TEST/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX shared library libTrluper.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Trluper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Trluper.dir/build: libTrluper.so
.PHONY : CMakeFiles/Trluper.dir/build

CMakeFiles/Trluper.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Trluper.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Trluper.dir/clean

CMakeFiles/Trluper.dir/depend:
	cd /home/project/Trluper/TEST && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/project/Trluper /home/project/Trluper /home/project/Trluper/TEST /home/project/Trluper/TEST /home/project/Trluper/TEST/CMakeFiles/Trluper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Trluper.dir/depend
