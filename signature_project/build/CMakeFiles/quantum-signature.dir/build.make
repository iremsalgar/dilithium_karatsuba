# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_SOURCE_DIR = /home/isalgar/karatsuba/signature_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/isalgar/karatsuba/signature_project/build

# Include any dependencies generated for this target.
include CMakeFiles/quantum-signature.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/quantum-signature.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/quantum-signature.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/quantum-signature.dir/flags.make

CMakeFiles/quantum-signature.dir/src/test.c.o: CMakeFiles/quantum-signature.dir/flags.make
CMakeFiles/quantum-signature.dir/src/test.c.o: /home/isalgar/karatsuba/signature_project/src/test.c
CMakeFiles/quantum-signature.dir/src/test.c.o: CMakeFiles/quantum-signature.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/isalgar/karatsuba/signature_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/quantum-signature.dir/src/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/quantum-signature.dir/src/test.c.o -MF CMakeFiles/quantum-signature.dir/src/test.c.o.d -o CMakeFiles/quantum-signature.dir/src/test.c.o -c /home/isalgar/karatsuba/signature_project/src/test.c

CMakeFiles/quantum-signature.dir/src/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/quantum-signature.dir/src/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/isalgar/karatsuba/signature_project/src/test.c > CMakeFiles/quantum-signature.dir/src/test.c.i

CMakeFiles/quantum-signature.dir/src/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/quantum-signature.dir/src/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/isalgar/karatsuba/signature_project/src/test.c -o CMakeFiles/quantum-signature.dir/src/test.c.s

# Object files for target quantum-signature
quantum__signature_OBJECTS = \
"CMakeFiles/quantum-signature.dir/src/test.c.o"

# External object files for target quantum-signature
quantum__signature_EXTERNAL_OBJECTS =

quantum-signature: CMakeFiles/quantum-signature.dir/src/test.c.o
quantum-signature: CMakeFiles/quantum-signature.dir/build.make
quantum-signature: /usr/lib/aarch64-linux-gnu/libssl.so
quantum-signature: /usr/lib/aarch64-linux-gnu/libcrypto.so
quantum-signature: CMakeFiles/quantum-signature.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/isalgar/karatsuba/signature_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable quantum-signature"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/quantum-signature.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/quantum-signature.dir/build: quantum-signature
.PHONY : CMakeFiles/quantum-signature.dir/build

CMakeFiles/quantum-signature.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/quantum-signature.dir/cmake_clean.cmake
.PHONY : CMakeFiles/quantum-signature.dir/clean

CMakeFiles/quantum-signature.dir/depend:
	cd /home/isalgar/karatsuba/signature_project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/isalgar/karatsuba/signature_project /home/isalgar/karatsuba/signature_project /home/isalgar/karatsuba/signature_project/build /home/isalgar/karatsuba/signature_project/build /home/isalgar/karatsuba/signature_project/build/CMakeFiles/quantum-signature.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/quantum-signature.dir/depend

