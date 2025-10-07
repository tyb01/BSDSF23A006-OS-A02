CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = src/main.c src/lsv1.5.0.c
TARGET = bin/ls

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) | bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

bin:
	mkdir -p bin

clean:
	rm -rf bin

