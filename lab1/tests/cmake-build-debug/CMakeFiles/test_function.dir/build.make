# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/ed/Downloads/clion-2019.2.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ed/Downloads/clion-2019.2.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ed/CLionProjects/unum/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ed/CLionProjects/unum/tests/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test_function.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_function.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_function.dir/flags.make

CMakeFiles/test_function.dir/test_function.o: CMakeFiles/test_function.dir/flags.make
CMakeFiles/test_function.dir/test_function.o: ../test_function.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ed/CLionProjects/unum/tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_function.dir/test_function.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_function.dir/test_function.o   -c /home/ed/CLionProjects/unum/tests/test_function.c

CMakeFiles/test_function.dir/test_function.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_function.dir/test_function.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ed/CLionProjects/unum/tests/test_function.c > CMakeFiles/test_function.dir/test_function.i

CMakeFiles/test_function.dir/test_function.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_function.dir/test_function.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ed/CLionProjects/unum/tests/test_function.c -o CMakeFiles/test_function.dir/test_function.s

# Object files for target test_function
test_function_OBJECTS = \
"CMakeFiles/test_function.dir/test_function.o"

# External object files for target test_function
test_function_EXTERNAL_OBJECTS =

test_function: CMakeFiles/test_function.dir/test_function.o
test_function: CMakeFiles/test_function.dir/build.make
test_function: CMakeFiles/test_function.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ed/CLionProjects/unum/tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_function"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_function.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_function.dir/build: test_function

.PHONY : CMakeFiles/test_function.dir/build

CMakeFiles/test_function.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_function.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_function.dir/clean

CMakeFiles/test_function.dir/depend:
	cd /home/ed/CLionProjects/unum/tests/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ed/CLionProjects/unum/tests /home/ed/CLionProjects/unum/tests /home/ed/CLionProjects/unum/tests/cmake-build-debug /home/ed/CLionProjects/unum/tests/cmake-build-debug /home/ed/CLionProjects/unum/tests/cmake-build-debug/CMakeFiles/test_function.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_function.dir/depend
