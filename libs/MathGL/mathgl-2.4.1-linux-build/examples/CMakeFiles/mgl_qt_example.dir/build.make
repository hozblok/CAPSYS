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
include examples/CMakeFiles/mgl_qt_example.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/mgl_qt_example.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/mgl_qt_example.dir/flags.make

examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o: examples/CMakeFiles/mgl_qt_example.dir/flags.make
examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o: /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/wnd_samples.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o -c /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/wnd_samples.cpp

examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.i"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/wnd_samples.cpp > CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.i

examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.s"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/wnd_samples.cpp -o CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.s

examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.requires:

.PHONY : examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.requires

examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.provides: examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/mgl_qt_example.dir/build.make examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.provides.build
.PHONY : examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.provides

examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.provides.build: examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o


examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o: examples/CMakeFiles/mgl_qt_example.dir/flags.make
examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o: /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/qt_example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o -c /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/qt_example.cpp

examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mgl_qt_example.dir/qt_example.cpp.i"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/qt_example.cpp > CMakeFiles/mgl_qt_example.dir/qt_example.cpp.i

examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mgl_qt_example.dir/qt_example.cpp.s"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples/qt_example.cpp -o CMakeFiles/mgl_qt_example.dir/qt_example.cpp.s

examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.requires:

.PHONY : examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.requires

examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.provides: examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/mgl_qt_example.dir/build.make examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.provides.build
.PHONY : examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.provides

examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.provides.build: examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o


# Object files for target mgl_qt_example
mgl_qt_example_OBJECTS = \
"CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o" \
"CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o"

# External object files for target mgl_qt_example
mgl_qt_example_EXTERNAL_OBJECTS =

examples/mgl_qt_example: examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o
examples/mgl_qt_example: examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o
examples/mgl_qt_example: examples/CMakeFiles/mgl_qt_example.dir/build.make
examples/mgl_qt_example: widgets/qt5/libmgl-qt5.so.7.5.0
examples/mgl_qt_example: src/libmgl.so.7.5.0
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libGL.so
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libgif.so
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libpng.so
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libz.so
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libjpeg.so
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libQt5PrintSupport.so.5.7.1
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libQt5OpenGL.so.5.7.1
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.7.1
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.7.1
examples/mgl_qt_example: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.7.1
examples/mgl_qt_example: examples/CMakeFiles/mgl_qt_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable mgl_qt_example"
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mgl_qt_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/mgl_qt_example.dir/build: examples/mgl_qt_example

.PHONY : examples/CMakeFiles/mgl_qt_example.dir/build

examples/CMakeFiles/mgl_qt_example.dir/requires: examples/CMakeFiles/mgl_qt_example.dir/wnd_samples.cpp.o.requires
examples/CMakeFiles/mgl_qt_example.dir/requires: examples/CMakeFiles/mgl_qt_example.dir/qt_example.cpp.o.requires

.PHONY : examples/CMakeFiles/mgl_qt_example.dir/requires

examples/CMakeFiles/mgl_qt_example.dir/clean:
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples && $(CMAKE_COMMAND) -P CMakeFiles/mgl_qt_example.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/mgl_qt_example.dir/clean

examples/CMakeFiles/mgl_qt_example.dir/depend:
	cd /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1 /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1/examples /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples/CMakeFiles/mgl_qt_example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/mgl_qt_example.dir/depend

