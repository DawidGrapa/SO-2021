CC = gcc 
CFLAGS = -Wall -O0 -std=c11 #-00 wylaczenie debugowania kodu, -std=c11 wersja standardu jezyka

#-L. pokazuje folder gdzie sa pliki
#-shared - kompilowanie biblioteki dzielonej
#-static -"- bliblioteki statycznej
#-Wall wlacza wszystkie mozliwe ostrzezenia podczas kompilacji
#-fPIC - Position Independent Code, potrzebne przybibiliotekach

zad2: static
	$(CC) $(CFLAGS) -c main.c -o program.o
	$(CC) $(CFLAGS) -static program.o -L. library.a -o program 
	make start
	make clean
	

zad2_Test:	clean static
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
	rm -f *Merged*
	rm -f *test*

static:
	$(CC) $(CFLAGS) -c library.c -o library.o
	ar rcs library.a library.o

shared:
	$(CC) $(CFLAGS) -fPIC -shared library.c -o library_shared.so

start: 
	./program create_table 2 merge_files a.txt:b.txt pierwszy.txt:drugi.txt