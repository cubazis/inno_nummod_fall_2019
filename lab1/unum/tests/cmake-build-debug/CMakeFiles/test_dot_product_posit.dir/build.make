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
CMAKE_COMMAND = /snap/clion/82/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/82/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test_dot_product_posit.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_dot_product_posit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_dot_product_posit.dir/flags.make

CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.o: CMakeFiles/test_dot_product_posit.dir/flags.make
CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.o: ../test_dot_product_posit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.o   -c /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/test_dot_product_posit.c

CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/test_dot_product_posit.c > CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.i

CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/test_dot_product_posit.c -o CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.s

# Object files for target test_dot_product_posit
test_dot_product_posit_OBJECTS = \
"CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.o"

# External object files for target test_dot_product_posit
test_dot_product_posit_EXTERNAL_OBJECTS =

test_dot_product_posit: CMakeFiles/test_dot_product_posit.dir/test_dot_product_posit.o
test_dot_product_posit: CMakeFiles/test_dot_product_posit.dir/build.make
test_dot_product_posit: CMakeFiles/test_dot_product_posit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_dot_product_posit"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_dot_product_posit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_dot_product_posit.dir/build: test_dot_product_posit

.PHONY : CMakeFiles/test_dot_product_posit.dir/build

CMakeFiles/test_dot_product_posit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_dot_product_posit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_dot_product_posit.dir/clean

CMakeFiles/test_dot_product_posit.dir/depend:
	cd /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug /home/alec/CLionProjects/NumModelingHW1/tmp/inno_nummod_fall_2019/lab1/unum/tests/cmake-build-debug/CMakeFiles/test_dot_product_posit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_dot_product_posit.dir/depend

