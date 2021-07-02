main: main.c loader.o
	gcc -o main main.c loader.o

loader.o: header/loader.h loader.c
	gcc -c -Wall header/loader.h loader.c
