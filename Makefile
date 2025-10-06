# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/lsv1.3.0.c

# Output binary
TARGET = $(BIN_DIR)/ls

# Default target
all: $(BIN_DIR) $(TARGET)

# Create binary directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile binary
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean compiled files
clean:
	rm -rf $(BIN_DIR)
