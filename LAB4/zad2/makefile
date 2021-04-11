CC = gcc
CFLAGS = -Wall -Wextra -O0 -std=c11 -g

test:
	$(CC) $(CFLAGS) main.c -D_POSIX_C_SOURCE=199309l -o main
	./main