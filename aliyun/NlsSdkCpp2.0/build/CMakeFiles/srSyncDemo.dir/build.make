# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/fish/speech/aliyun/NlsSdkCpp2.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fish/speech/aliyun/NlsSdkCpp2.0/build

# Include any dependencies generated for this target.
include CMakeFiles/srSyncDemo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/srSyncDemo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/srSyncDemo.dir/flags.make

CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o: CMakeFiles/srSyncDemo.dir/flags.make
CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o: ../demo/speechRecognizerSyncDemo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fish/speech/aliyun/NlsSdkCpp2.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o -c /home/fish/speech/aliyun/NlsSdkCpp2.0/demo/speechRecognizerSyncDemo.cpp

CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fish/speech/aliyun/NlsSdkCpp2.0/demo/speechRecognizerSyncDemo.cpp > CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.i

CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fish/speech/aliyun/NlsSdkCpp2.0/demo/speechRecognizerSyncDemo.cpp -o CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.s

CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.requires:

.PHONY : CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.requires

CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.provides: CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.requires
	$(MAKE) -f CMakeFiles/srSyncDemo.dir/build.make CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.provides.build
.PHONY : CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.provides

CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.provides.build: CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o


# Object files for target srSyncDemo
srSyncDemo_OBJECTS = \
"CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o"

# External object files for target srSyncDemo
srSyncDemo_EXTERNAL_OBJECTS =

../demo/srSyncDemo: CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o
../demo/srSyncDemo: CMakeFiles/srSyncDemo.dir/build.make
../demo/srSyncDemo: CMakeFiles/srSyncDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fish/speech/aliyun/NlsSdkCpp2.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../demo/srSyncDemo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/srSyncDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/srSyncDemo.dir/build: ../demo/srSyncDemo

.PHONY : CMakeFiles/srSyncDemo.dir/build

CMakeFiles/srSyncDemo.dir/requires: CMakeFiles/srSyncDemo.dir/demo/speechRecognizerSyncDemo.cpp.o.requires

.PHONY : CMakeFiles/srSyncDemo.dir/requires

CMakeFiles/srSyncDemo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/srSyncDemo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/srSyncDemo.dir/clean

CMakeFiles/srSyncDemo.dir/depend:
	cd /home/fish/speech/aliyun/NlsSdkCpp2.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fish/speech/aliyun/NlsSdkCpp2.0 /home/fish/speech/aliyun/NlsSdkCpp2.0 /home/fish/speech/aliyun/NlsSdkCpp2.0/build /home/fish/speech/aliyun/NlsSdkCpp2.0/build /home/fish/speech/aliyun/NlsSdkCpp2.0/build/CMakeFiles/srSyncDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/srSyncDemo.dir/depend

