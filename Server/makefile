# Shell program used.
SHELL = /bin/sh

# Binaries directory.
BIN_DIR := bin
# Executable file (target).
BIN_FILE := $(BIN_DIR)/tdsv

# Other directories.
SRC_DIR := src
HDR_DIR := include
OBJ_DIR := build
TEST_SRC_DIR := test

# File extensions.
HDR_EXT := hpp
SRC_EXT := cpp
OBJ_EXT := o

# Compiler and options.
CC = g++
CDEBUG = -g
OS := $(shell uname)
ifeq ($(OS), Linux)
    INC := -I$(HDR_DIR)
    LIBS := -L/usr/lib/x86_64-linux-gnu/ -lSDL2 -lSDL2_ttf
else ifeq ($(OS), Darwin)
    INC := -I$(HDR_DIR) -I/Library/Frameworks/SDL2.framework/Headers/\
           -I/Library/Frameworks/SDL2_ttf.framework/Headers -F/Library/Frameworks/
    LIBS := -F /Library/Frameworks -framework SDL2 -framework SDL2_ttf
endif
CFLAGS = -std=c++11 -Wall -O $(CDEBUG) $(INC)
LDFLAGS = -g

# Find all source files names.
SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
# Generate object file names from source files names.
OBJ_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.$(OBJ_EXT), $(SRC_FILES))
# Find all test source files names.
TEST_SRC_FILES := $(wildcard $(TEST_SRC_DIR)/*.$(SRC_EXT))
# Generate test object file names from source files names.
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.$(OBJ_EXT), $(TEST_SRC_FILES))
# Generate test binary file names from source files names.
TEST_BIN_FILES := $(patsubst $(TEST_SRC_DIR)/%.$(SRC_EXT), $(BIN_DIR)/%, $(TEST_SRC_FILES))
TESTS := $(patsubst $(TEST_SRC_DIR)/%.$(SRC_EXT), %, $(TEST_SRC_FILES))

# Create executable file plus tests.
.PHONY: all
all: $(BIN_FILE) testall

# Main rule: create executable file.
$(BIN_FILE): $(OBJ_DIR)/main.o $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@ 

$(OBJ_FILES): $(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

# Test rules.
.PHONY: testall
testall: $(TESTS)

$(TESTS): %: $(BIN_DIR)/%

$(TEST_BIN_FILES): $(BIN_DIR)/%: $(OBJ_DIR)/%.$(OBJ_EXT)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(TEST_OBJ_FILES): $(OBJ_DIR)/%.$(OBJ_EXT): $(TEST_SRC_DIR)/%.$(SRC_EXT) 
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

# Cleaning.
.PHONY: clean
clean:
	$(RM) $(BIN_FILE)
	$(RM) $(OBJ_FILES)
	$(RM) $(TEST_BIN_FILES)
	$(RM) $(TEST_OBJ_FILES)
