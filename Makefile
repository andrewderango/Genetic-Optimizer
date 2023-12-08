CC = gcc
CFLAGS = -Wall -g -O3

all: GA

GA: GA.c OF.c functions.c functions.h
	$(CC) $(CFLAGS) -o GA GA.c OF.c functions.c -lm

clean:
	rm -f GA
	rm -rf GA.dSYM
