CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude
TARGET = ibredit
SRC = $(wildcard src/*.c)

$(TARGET):
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
