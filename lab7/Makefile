all: main.o
	gcc -o lab6 main.o mem.o mem_debug.o

main.o: mem.o mem_debug.o main.c tests.c
	gcc -c -o main.o main.c

mem.o: mem.c
	gcc -c -o mem.o mem.c

mem_debug.o: mem_debug.c
	gcc -c -o mem_debug.o mem_debug.c

clean:
	rm lab6 *.o