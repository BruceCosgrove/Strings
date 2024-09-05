# The name of this makefile.
MAKEFILE := makefile
# The output name.
OUT_NAME := test
# The default build configuration.
# To set directly, use "make cfg=debug" or "make cfg=release".
cfg ?= debug

# The compiler to use.
COMPILER := g++-13
# The compiler flags to use.
CXXFLAGS := -std=c++23 -Wall
CXXFLAGS.debug := -g
CXXFLAGS.release := -O2
# Add the configuration-specific compiler flags.
CXXFLAGS += $(CXXFLAGS.$(cfg))

# The directory where all source files are located.
SRC_DIR := src/
# The directories where external include files are located.
INC_DIRS := ../include/
# The directory where all intermediate files should be placed.
INT_DIR_ROOT := bin-int/
INT_DIR := $(INT_DIR_ROOT)$(cfg)/
# The directory where all output files should be placed.
OUT_DIR_ROOT := bin/
OUT_DIR := $(OUT_DIR_ROOT)$(cfg)/

# GoogleTest settings.
GTEST_SRC_DIR := googletest-1.15.2/googletest/src/
GTEST_INCS := -I googletest-1.15.2/googletest/include/ -I googletest-1.15.2/googletest/
GTEST_FLAGS := -pthread
GTEST_SRCS := $(patsubst %,$(GTEST_SRC_DIR)%,gtest-all.cc gtest_main.cc)
GTEST_OBJS := $(patsubst %.cc,$(INT_DIR)%.o,$(GTEST_SRCS))

# Get all source filepaths in $(SRC_DIR) and all subdirectories of $(SRC_DIR).
SRCS := $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)**/*.cpp)
# Get all include filepaths in $(INC_DIRS).
INCS := $(patsubst %,-I %,$(INC_DIRS))
# Get each obj filepath corresponding to each source filepath.
OBJS := $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(SRCS))
# Get each dependency filepath corresponding to each source filepath.
DEPS := $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.d,$(SRCS))
# Get all directories that should be pre-created (w/ duplicates removed so mkdir doesn't warn).
DIRS := $(sort $(INT_DIR) $(OUT_DIR) $(INT_DIR)$(GTEST_SRC_DIR) $(patsubst $(SRC_DIR)%,$(INT_DIR)%,$(sort $(dir $(SRCS)))))
# Get the executable's filepath.
EXE := $(OUT_DIR)$(OUT_NAME)

# Compile the executable.
all: $(EXE) $(MAKEFILE)

# Include all source dependencies.
-include $(DEPS)

# Create necessary directories, compile, get dependencies, and make sure to recompile everything if the makefile changed.
$(INT_DIR)%.o: $(SRC_DIR)%.cpp $(MAKEFILE) | create_dirs
	$(COMPILER) $(CXXFLAGS) -MMD -c $(INCS) $(GTEST_INCS) -o $@ $<

# Compile GoogleTest
$(INT_DIR)$(GTEST_SRC_DIR)%.o: $(GTEST_SRC_DIR)%.cc $(MAKEFILE) | create_dirs
	$(COMPILER) $(CXXFLAGS) $(GTEST_FLAGS) -c $(GTEST_INCS) -o $@ $<

# Link all objs into the executable.
$(EXE): $(OBJS) $(GTEST_OBJS) $(MAKEFILE)
	$(COMPILER) -o $(EXE) $(OBJS) $(GTEST_OBJS)

# Creates all the necessary directories.
.PHONY: create_dirs
create_dirs: $(DIRS)
$(DIRS):
	mkdir -p $(DIRS)

# Runs the test.
.PHONY: run
run: $(EXE)
	./$(EXE)

# Clears the terminal.
.PHONY: clear
clear:
	clear

# Clears the terminal and compiles the necessary files.
# If successful, also runs the test.
.PHONY: crun
crun: clear run

# Clears the terminal and recompiles everything.
# If successful, also runs the test.
.PHONY: ccrun
ccrun: clean clear run

# Debugs the test.
.PHONY: rund
rund: $(EXE)
	gdb ./$(EXE)

# Removes all intermediates and the executable.
.PHONY: clean
clean:
	rm -rf $(OUT_DIR_ROOT) $(INT_DIR_ROOT)
