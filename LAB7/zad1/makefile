build:
	touch bake
	touch table
	touch pizzeria
	gcc -o supplier supplier.c shared.c
	gcc -o chef chef.c shared.c
	gcc -o main main.c shared.c

test:
	make build
	./main 2 5