CC=gcc
CFLAGS=-Wall -std=c11 -Wextra

all: main

# Compiling the ULine lib.
png.o: png.c png.h
	$(CC) $(CFLAGS) -c png.c

# Linking all.
main: main.c png.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del *.o
