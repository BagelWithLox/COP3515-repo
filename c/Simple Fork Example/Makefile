CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -g

TARGET = fork_example
SOURCE = fork_example.c

.PHONY: all build run clean valgrind

all: build

build:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

valgrind: build
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)