CC = gcc
CFLAGS = -Wall -Wextra -O0 -std=c11 -g

static:
	$(CC) $(CFLAGS) -c library.c -o library.o
	ar rcs library.a library.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *Merged*

test: clean static
	$(CC) $(CFLAGS) -c main.c -o prog.o
	$(CC) $(CFLAGS) -static prog.o -L. library.a -o prog
	./prog Tests > raport_zad_2.txt
	make clean


zad2: static
	$(CC) $(CFLAGS) -c main.c -o program.o
	$(CC) $(CFLAGS) -static program.o -L. library.a -o program 
	make start
	make clean

start: 
	./program create_table 2 merge_files a.txt:b.txt pierwszy.txt:drugi.txt		