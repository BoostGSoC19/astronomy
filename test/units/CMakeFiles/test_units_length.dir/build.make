# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/rohit/astronomy/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rohit/astronomy/test

# Include any dependencies generated for this target.
include units/CMakeFiles/test_units_length.dir/depend.make

# Include the progress variables for this target.
include units/CMakeFiles/test_units_length.dir/progress.make

# Include the compile flags for this target's objects.
include units/CMakeFiles/test_units_length.dir/flags.make

units/CMakeFiles/test_units_length.dir/length.o: units/CMakeFiles/test_units_length.dir/flags.make
units/CMakeFiles/test_units_length.dir/length.o: units/length.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rohit/astronomy/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object units/CMakeFiles/test_units_length.dir/length.o"
	cd /home/rohit/astronomy/test/units && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_units_length.dir/length.o -c /home/rohit/astronomy/test/units/length.cpp

units/CMakeFiles/test_units_length.dir/length.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_units_length.dir/length.i"
	cd /home/rohit/astronomy/test/units && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rohit/astronomy/test/units/length.cpp > CMakeFiles/test_units_length.dir/length.i

units/CMakeFiles/test_units_length.dir/length.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_units_length.dir/length.s"
	cd /home/rohit/astronomy/test/units && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rohit/astronomy/test/units/length.cpp -o CMakeFiles/test_units_length.dir/length.s

units/CMakeFiles/test_units_length.dir/length.o.requires:

.PHONY : units/CMakeFiles/test_units_length.dir/length.o.requires

units/CMakeFiles/test_units_length.dir/length.o.provides: units/CMakeFiles/test_units_length.dir/length.o.requires
	$(MAKE) -f units/CMakeFiles/test_units_length.dir/build.make units/CMakeFiles/test_units_length.dir/length.o.provides.build
.PHONY : units/CMakeFiles/test_units_length.dir/length.o.provides

units/CMakeFiles/test_units_length.dir/length.o.provides.build: units/CMakeFiles/test_units_length.dir/length.o


# Object files for target test_units_length
test_units_length_OBJECTS = \
"CMakeFiles/test_units_length.dir/length.o"

# External object files for target test_units_length
test_units_length_EXTERNAL_OBJECTS =

units/test_units_length: units/CMakeFiles/test_units_length.dir/length.o
units/test_units_length: units/CMakeFiles/test_units_length.dir/build.make
units/test_units_length: units/CMakeFiles/test_units_length.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rohit/astronomy/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_units_length"
	cd /home/rohit/astronomy/test/units && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_units_length.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
units/CMakeFiles/test_units_length.dir/build: units/test_units_length

.PHONY : units/CMakeFiles/test_units_length.dir/build

units/CMakeFiles/test_units_length.dir/requires: units/CMakeFiles/test_units_length.dir/length.o.requires

.PHONY : units/CMakeFiles/test_units_length.dir/requires

units/CMakeFiles/test_units_length.dir/clean:
	cd /home/rohit/astronomy/test/units && $(CMAKE_COMMAND) -P CMakeFiles/test_units_length.dir/cmake_clean.cmake
.PHONY : units/CMakeFiles/test_units_length.dir/clean

units/CMakeFiles/test_units_length.dir/depend:
	cd /home/rohit/astronomy/test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rohit/astronomy/test /home/rohit/astronomy/test/units /home/rohit/astronomy/test /home/rohit/astronomy/test/units /home/rohit/astronomy/test/units/CMakeFiles/test_units_length.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : units/CMakeFiles/test_units_length.dir/depend

