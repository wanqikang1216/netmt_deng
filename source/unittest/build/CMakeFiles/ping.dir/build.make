# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/Cwork/source/unittest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/Cwork/source/unittest/build

# Include any dependencies generated for this target.
include CMakeFiles/ping.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ping.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ping.dir/flags.make

CMakeFiles/ping.dir/ping_test.cpp.o: CMakeFiles/ping.dir/flags.make
CMakeFiles/ping.dir/ping_test.cpp.o: ../ping_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/Cwork/source/unittest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ping.dir/ping_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ping.dir/ping_test.cpp.o -c /mnt/e/Cwork/source/unittest/ping_test.cpp

CMakeFiles/ping.dir/ping_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ping.dir/ping_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/Cwork/source/unittest/ping_test.cpp > CMakeFiles/ping.dir/ping_test.cpp.i

CMakeFiles/ping.dir/ping_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ping.dir/ping_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/Cwork/source/unittest/ping_test.cpp -o CMakeFiles/ping.dir/ping_test.cpp.s

# Object files for target ping
ping_OBJECTS = \
"CMakeFiles/ping.dir/ping_test.cpp.o"

# External object files for target ping
ping_EXTERNAL_OBJECTS =

ping: CMakeFiles/ping.dir/ping_test.cpp.o
ping: CMakeFiles/ping.dir/build.make
ping: ../lib/libhv.a
ping: CMakeFiles/ping.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/Cwork/source/unittest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ping"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ping.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ping.dir/build: ping

.PHONY : CMakeFiles/ping.dir/build

CMakeFiles/ping.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ping.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ping.dir/clean

CMakeFiles/ping.dir/depend:
	cd /mnt/e/Cwork/source/unittest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/Cwork/source/unittest /mnt/e/Cwork/source/unittest /mnt/e/Cwork/source/unittest/build /mnt/e/Cwork/source/unittest/build /mnt/e/Cwork/source/unittest/build/CMakeFiles/ping.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ping.dir/depend
