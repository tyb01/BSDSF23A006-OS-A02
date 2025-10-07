CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = src/main.c src/lsv1.6.0.c
OUT = bin/ls

all:
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -rf bin
