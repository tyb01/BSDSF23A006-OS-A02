# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/lsv1.4.0.c
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/lsv1.4.0.o
TARGET = $(BIN_DIR)/ls

# Default target
all: $(TARGET)

# Create binary
$(TARGET): $(SRCS) | $(BIN_DIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

# Directories
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean compiled files
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Phony targets
.PHONY: all clean
