# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/ing/Bureau/DM_Compression_formats_d-images

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ing/Bureau/DM_Compression_formats_d-images/build

# Include any dependencies generated for this target.
include CMakeFiles/DM_Compression_formats_d-images.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DM_Compression_formats_d-images.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DM_Compression_formats_d-images.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DM_Compression_formats_d-images.dir/flags.make

CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o: CMakeFiles/DM_Compression_formats_d-images.dir/flags.make
CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o: /home/ing/Bureau/DM_Compression_formats_d-images/src/test.cpp
CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o: CMakeFiles/DM_Compression_formats_d-images.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ing/Bureau/DM_Compression_formats_d-images/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o -MF CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o.d -o CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o -c /home/ing/Bureau/DM_Compression_formats_d-images/src/test.cpp

CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ing/Bureau/DM_Compression_formats_d-images/src/test.cpp > CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.i

CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ing/Bureau/DM_Compression_formats_d-images/src/test.cpp -o CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.s

# Object files for target DM_Compression_formats_d-images
DM_Compression_formats_d__images_OBJECTS = \
"CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o"

# External object files for target DM_Compression_formats_d-images
DM_Compression_formats_d__images_EXTERNAL_OBJECTS =

DM_Compression_formats_d-images: CMakeFiles/DM_Compression_formats_d-images.dir/src/test.cpp.o
DM_Compression_formats_d-images: CMakeFiles/DM_Compression_formats_d-images.dir/build.make
DM_Compression_formats_d-images: CMakeFiles/DM_Compression_formats_d-images.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ing/Bureau/DM_Compression_formats_d-images/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DM_Compression_formats_d-images"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DM_Compression_formats_d-images.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DM_Compression_formats_d-images.dir/build: DM_Compression_formats_d-images
.PHONY : CMakeFiles/DM_Compression_formats_d-images.dir/build

CMakeFiles/DM_Compression_formats_d-images.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DM_Compression_formats_d-images.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DM_Compression_formats_d-images.dir/clean

CMakeFiles/DM_Compression_formats_d-images.dir/depend:
	cd /home/ing/Bureau/DM_Compression_formats_d-images/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ing/Bureau/DM_Compression_formats_d-images /home/ing/Bureau/DM_Compression_formats_d-images /home/ing/Bureau/DM_Compression_formats_d-images/build /home/ing/Bureau/DM_Compression_formats_d-images/build /home/ing/Bureau/DM_Compression_formats_d-images/build/CMakeFiles/DM_Compression_formats_d-images.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DM_Compression_formats_d-images.dir/depend

