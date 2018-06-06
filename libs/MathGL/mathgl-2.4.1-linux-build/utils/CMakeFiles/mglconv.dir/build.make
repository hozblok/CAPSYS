# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build

# Include any dependencies generated for this target.
include utils/CMakeFiles/mglconv.dir/depend.make

# Include the progress variables for this target.
include utils/CMakeFiles/mglconv.dir/progress.make

# Include the compile flags for this target's objects.
include utils/CMakeFiles/mglconv.dir/flags.make

utils/CMakeFiles/mglconv.dir/mglconv.cpp.o: utils/CMakeFiles/mglconv.dir/flags.make
utils/CMakeFiles/mglconv.dir/mglconv.cpp.o: /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/utils/mglconv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object utils/CMakeFiles/mglconv.dir/mglconv.cpp.o"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mglconv.dir/mglconv.cpp.o -c /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/utils/mglconv.cpp

utils/CMakeFiles/mglconv.dir/mglconv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mglconv.dir/mglconv.cpp.i"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/utils/mglconv.cpp > CMakeFiles/mglconv.dir/mglconv.cpp.i

utils/CMakeFiles/mglconv.dir/mglconv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mglconv.dir/mglconv.cpp.s"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/utils/mglconv.cpp -o CMakeFiles/mglconv.dir/mglconv.cpp.s

utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.requires:

.PHONY : utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.requires

utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.provides: utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.requires
	$(MAKE) -f utils/CMakeFiles/mglconv.dir/build.make utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.provides.build
.PHONY : utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.provides

utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.provides.build: utils/CMakeFiles/mglconv.dir/mglconv.cpp.o


# Object files for target mglconv
mglconv_OBJECTS = \
"CMakeFiles/mglconv.dir/mglconv.cpp.o"

# External object files for target mglconv
mglconv_EXTERNAL_OBJECTS =

utils/mglconv: utils/CMakeFiles/mglconv.dir/mglconv.cpp.o
utils/mglconv: utils/CMakeFiles/mglconv.dir/build.make
utils/mglconv: src/libmgl.so.7.5.0
utils/mglconv: /usr/lib/x86_64-linux-gnu/libGLU.so
utils/mglconv: /usr/lib/x86_64-linux-gnu/libGL.so
utils/mglconv: /usr/lib/x86_64-linux-gnu/libgif.so
utils/mglconv: /usr/lib/x86_64-linux-gnu/libpng.so
utils/mglconv: /usr/lib/x86_64-linux-gnu/libz.so
utils/mglconv: /usr/lib/x86_64-linux-gnu/libjpeg.so
utils/mglconv: utils/CMakeFiles/mglconv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mglconv"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mglconv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
utils/CMakeFiles/mglconv.dir/build: utils/mglconv

.PHONY : utils/CMakeFiles/mglconv.dir/build

utils/CMakeFiles/mglconv.dir/requires: utils/CMakeFiles/mglconv.dir/mglconv.cpp.o.requires

.PHONY : utils/CMakeFiles/mglconv.dir/requires

utils/CMakeFiles/mglconv.dir/clean:
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils && $(CMAKE_COMMAND) -P CMakeFiles/mglconv.dir/cmake_clean.cmake
.PHONY : utils/CMakeFiles/mglconv.dir/clean

utils/CMakeFiles/mglconv.dir/depend:
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1 /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/utils /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils/CMakeFiles/mglconv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : utils/CMakeFiles/mglconv.dir/depend

