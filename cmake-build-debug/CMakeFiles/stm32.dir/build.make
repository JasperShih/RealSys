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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jaspershih/Documents/tso4260Final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jaspershih/Documents/tso4260Final/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/stm32.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/stm32.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stm32.dir/flags.make

CMakeFiles/stm32.dir/src/main.cpp.o: CMakeFiles/stm32.dir/flags.make
CMakeFiles/stm32.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jaspershih/Documents/tso4260Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/stm32.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/stm32.dir/src/main.cpp.o -c /Users/jaspershih/Documents/tso4260Final/src/main.cpp

CMakeFiles/stm32.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stm32.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jaspershih/Documents/tso4260Final/src/main.cpp > CMakeFiles/stm32.dir/src/main.cpp.i

CMakeFiles/stm32.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stm32.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jaspershih/Documents/tso4260Final/src/main.cpp -o CMakeFiles/stm32.dir/src/main.cpp.s

# Object files for target stm32
stm32_OBJECTS = \
"CMakeFiles/stm32.dir/src/main.cpp.o"

# External object files for target stm32
stm32_EXTERNAL_OBJECTS =

stm32: CMakeFiles/stm32.dir/src/main.cpp.o
stm32: CMakeFiles/stm32.dir/build.make
stm32: CMakeFiles/stm32.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jaspershih/Documents/tso4260Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stm32"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stm32.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stm32.dir/build: stm32

.PHONY : CMakeFiles/stm32.dir/build

CMakeFiles/stm32.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stm32.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stm32.dir/clean

CMakeFiles/stm32.dir/depend:
	cd /Users/jaspershih/Documents/tso4260Final/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jaspershih/Documents/tso4260Final /Users/jaspershih/Documents/tso4260Final /Users/jaspershih/Documents/tso4260Final/cmake-build-debug /Users/jaspershih/Documents/tso4260Final/cmake-build-debug /Users/jaspershih/Documents/tso4260Final/cmake-build-debug/CMakeFiles/stm32.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stm32.dir/depend

