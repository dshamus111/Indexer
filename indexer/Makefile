CC = gcc
Compile = $(CC)

all: index tokenizer.o sorted-list.o

index: tokenizer.o sorted-list.o
	$(CC) -o index tokenizer.o sorted-list.o

tokenizer.o: sorted-list.h tokenizer.c
	$(CC) -c -g tokenizer.c

sorted-list.o: sorted-list.h sorted-list.c
	$(CC) -c -g sorted-list.c

clean:
	rm -rf tokenizer.o sorted-list.o index
