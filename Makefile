CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = ibredit

$(TARGET):
	$(CC) *.c -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
