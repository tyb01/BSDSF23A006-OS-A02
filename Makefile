CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = src/main.c src/lsv1.1.0.c
BIN = bin/ls

all: $(BIN)

$(BIN): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -rf bin
