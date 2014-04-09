CC = gcc
Compile = $(CC)

all: search search.o

search: search.o
	$(CC) -o search search.o

search.o: search.h search.c
	$(CC) -c -g search.c

clean:
	rm -rf search.o search
