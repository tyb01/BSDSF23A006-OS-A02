# Makefile for lsv1.0.0

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Directories
SRC_DIR = src
BIN_DIR = bin

# Files
SRC = $(SRC_DIR)/lsv1.0.0.c
OUT = $(BIN_DIR)/lsv1.0.0

# Default target
all: $(OUT)

# Build rule
$(OUT): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Clean rule
clean:
	rm -rf $(BIN_DIR)

# Run rule (optional)
run: $(OUT)
	$(OUT)

# Example: run with argument
run-home: $(OUT)
	$(OUT) /home

.PHONY: all clean run run-home
