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
include CMakeFiles/test_dot_product.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_dot_product.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_dot_product.dir/flags.make

CMakeFiles/test_dot_product.dir/test_dot_product.o: CMakeFiles/test_dot_product.dir/flags.make
CMakeFiles/test_dot_product.dir/test_dot_product.o: ../test_dot_product.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ed/CLionProjects/unum/tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_dot_product.dir/test_dot_product.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_dot_product.dir/test_dot_product.o   -c /home/ed/CLionProjects/unum/tests/test_dot_product.c

CMakeFiles/test_dot_product.dir/test_dot_product.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_dot_product.dir/test_dot_product.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ed/CLionProjects/unum/tests/test_dot_product.c > CMakeFiles/test_dot_product.dir/test_dot_product.i

CMakeFiles/test_dot_product.dir/test_dot_product.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_dot_product.dir/test_dot_product.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ed/CLionProjects/unum/tests/test_dot_product.c -o CMakeFiles/test_dot_product.dir/test_dot_product.s

# Object files for target test_dot_product
test_dot_product_OBJECTS = \
"CMakeFiles/test_dot_product.dir/test_dot_product.o"

# External object files for target test_dot_product
test_dot_product_EXTERNAL_OBJECTS =

test_dot_product: CMakeFiles/test_dot_product.dir/test_dot_product.o
test_dot_product: CMakeFiles/test_dot_product.dir/build.make
test_dot_product: CMakeFiles/test_dot_product.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ed/CLionProjects/unum/tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_dot_product"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_dot_product.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_dot_product.dir/build: test_dot_product

.PHONY : CMakeFiles/test_dot_product.dir/build

CMakeFiles/test_dot_product.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_dot_product.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_dot_product.dir/clean

CMakeFiles/test_dot_product.dir/depend:
	cd /home/ed/CLionProjects/unum/tests/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ed/CLionProjects/unum/tests /home/ed/CLionProjects/unum/tests /home/ed/CLionProjects/unum/tests/cmake-build-debug /home/ed/CLionProjects/unum/tests/cmake-build-debug /home/ed/CLionProjects/unum/tests/cmake-build-debug/CMakeFiles/test_dot_product.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_dot_product.dir/depend

