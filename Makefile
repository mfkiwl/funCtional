CC = gcc
CFLAGS = -Wall -g

example: example.c functional.c
	$(CC) $(CFLAGS) -o example.o example.c functional.c && ./example.o

.PHONY: format
format:
	clang-format -i -style=file *.{c,h}
