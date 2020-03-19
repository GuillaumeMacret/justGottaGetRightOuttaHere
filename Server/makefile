# Shell program used.
SHELL = /bin/sh

# Binaries directory.
BIN_DIR := bin
# Executable file (target).
BIN_FILE := $(BIN_DIR)/app

# Other directories.
SRC_DIR := src
HDR_DIR := include
OBJ_DIR := obj

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
    LIBS := -L/usr/lib/x86_64-linux-gnu/ -lpthread
else ifeq ($(OS), Darwin)
    INC := -I$(HDR_DIR) 
    LIBS := -F /Library/Frameworks -lpthread
endif
CFLAGS = -std=c++11 -Wall -O $(CDEBUG) $(INC)
LDFLAGS = -g

# Find all source files names.
SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
# Generate object file names from source files names.
OBJ_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.$(OBJ_EXT), $(SRC_FILES))

# Create executable file plus tests.
.PHONY: all
all: $(BIN_FILE)

# Main rule: create executable file.
$(BIN_FILE): $(OBJ_DIR)/main.o $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@ 

$(OBJ_FILES): $(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

# Cleaning.
.PHONY: clean
clean:
	$(RM) $(BIN_FILE)
	$(RM) $(OBJ_FILES)
