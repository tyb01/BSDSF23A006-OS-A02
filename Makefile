CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = src/main.c src/lsv1.0.0.c src/lsv1.1.0.c
OUT = bin/ls

all:
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -rf bin
