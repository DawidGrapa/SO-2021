CC = gcc
CFLAGS = -Wall -Wextra -O0 -std=c11 -g

clean:
	rm -f main

test:
	$(CC) $(CFLAGS) main.c -o main -lm
	./main d a.txt

