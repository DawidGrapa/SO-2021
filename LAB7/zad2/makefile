build:
	gcc -Wall -o supplier supplier.c shared.c -lrt -lpthread
	gcc -Wall -o chef chef.c shared.c -lrt -lpthread
	gcc -Wall -o main main.c shared.c -lrt -lpthread

test:
	make build
	./main 2 2