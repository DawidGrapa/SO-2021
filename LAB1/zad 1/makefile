CC = gcc 
CFLAGS = -Wall -O0 -std=c11

static:
	$(CC) $(CFLAGS) -c library.c -o library.o
	ar rcs library.a library.o

shared:
	$(CC) $(CFLAGS) -fPIC -shared library.c -o library_shared.so