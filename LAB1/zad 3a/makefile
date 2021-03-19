CC = gcc 
CFLAGS = -Wall -O0 -std=c11 #-00 wylaczenie debugowania kodu, -std=c11 wersja standardu jezyka

#-L. pokazuje folder gdzie sa pliki
#-shared - kompilowanie biblioteki dzielonej
#-static -"- bliblioteki statycznej
#-Wall wlacza wszystkie mozliwe ostrzezenia podczas kompilacji
#-fPIC - Position Independent Code, potrzebne przybibiliotekach
#-Wl,-rpath=. - oznacza szukanie biblioteki w bieżącym katalogu w momencieuruchomienia programu

zad3: static
	$(CC) $(CFLAGS) -c main.c -o program.o
	$(CC) $(CFLAGS) -static program.o -L. library.a -o program 
	make start
	make clean
	

zad3_Test:	clean static
	$(CC) $(CFLAGS) -c main.c -o Test.o
	$(CC) $(CFLAGS) -static Test.o -L. library.a -o Test
	./Test Tests > raport2.txt
	./Test Tests
	make clean

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *Tests*
	rm -f *test*
	rm -f *Merged*

static:
	$(CC) $(CFLAGS) -c library.c -o library.o
	ar rcs library.a library.o

shared:
	$(CC) $(CFLAGS) -fPIC -shared library.c -o library_shared.so

start: 
	./program create_table 2 merge_files SMALL.txt:LARGE.txt remove_block 0


static_zad3: clean static
	$(CC) $(CFLAGS) -c main.c -o Test.o
	$(CC) $(CFLAGS) -static Test.o -L. library.a -o Test
	./Test Tests 
	make clean	

shared_zad3: clean shared
	$(CC) $(CFLAGS) -o Test main.c library_shared.so -L. -Wl,-rpath=. 
	./Test Tests
	make clean

dynamic_zad3: clean shared
	$(CC) $(CFLAGS) -o Test main_dynamic.c -ldl
	./Test Tests
	make clean

zad3a: clean
	@echo "\nSTATIC\n" >> results3a.txt
	make static
	$(CC) $(CFLAGS) -c main.c -o Test.o
	$(CC) $(CFLAGS) -static Test.o -L. library.a -o Test
	./Test Tests >> results3a.txt
	make clean
	
	@echo "\nSHARED\n" >> results3a.txt
	make shared
	$(CC) $(CFLAGS) -o Test main.c library_shared.so -L. -Wl,-rpath=.
	./Test Tests >> results3a.txt
	make clean

	@echo "\nDYNAMIC\n" >> results3a.txt
	make shared
	$(CC) $(CFLAGS) -o Test main_dynamic.c -ldl
	./Test Tests >> results3a.txt
	make clean

zad3b: clean
	@echo "\nSTATIC\n" >> results3b.txt

	@echo "\nWITH -O1\n" >> results3b.txt

	make static
	$(CC) -Wall -O1 -std=c11 -c main.c -o Test.o
	$(CC) -Wall -O1 -std=c11 -static Test.o -L. library.a -o Test
	./Test Tests >> results3b.txt
	make clean	

	@echo "\nWITH -Os\n" >> results3b.txt

	make static
	$(CC) -Wall -Os -std=c11 -c main.c -o Test.o
	$(CC) -Wall -Os -std=c11 -static Test.o -L. library.a -o Test
	./Test Tests >> results3b.txt
	make clean

	@echo "\nWITH -O3\n" >> results3b.txt

	make static
	$(CC) -Wall -O3 -std=c11 -c main.c -o Test.o
	$(CC) -Wall -O3 -std=c11 -static Test.o -L. library.a -o Test
	./Test Tests >> results3b.txt
	make clean

	
	@echo "\nSHARED\n" >> results3b.txt

	@echo "\nWITH -O1\n" >> results3b.txt

	make shared
	$(CC) -Wall -O1 -std=c11 -o Test main.c library_shared.so -L. -Wl,-rpath=.
	./Test Tests >> results3b.txt
	make clean

	@echo "\nWITH -Os\n" >> results3b.txt

	make shared
	$(CC) -Wall -Os -std=c11 -o Test main.c library_shared.so -L. -Wl,-rpath=.
	./Test Tests >> results3b.txt
	make clean

	@echo "\nWITH -O3\n" >> results3b.txt

	make shared
	$(CC) -Wall -O3 -std=c11 -o Test main.c library_shared.so -L. -Wl,-rpath=.
	./Test Tests >> results3b.txt
	make clean

	@echo "\nWITH -O1\n" >> results3b.txt

	make shared
	@echo "\nDYNAMIC\n" >> results3b.txt
	make shared
	$(CC) -Wall -O1 -std=c11 -o Test main_dynamic.c -ldl
	./Test Tests >> results3b.txt
	make clean

	@echo "\nWITH -Os\n" >> results3b.txt

	make shared
	$(CC) -Wall -Os -std=c11 -o Test main_dynamic.c -ldl
	./Test Tests >> results3b.txt
	make clean

	@echo "\nWITH -O3\n" >> results3b.txt

	make shared
	$(CC) -Wall -O3 -std=c11 -o Test main_dynamic.c -ldl
	./Test Tests >> results3b.txt
	make clean


