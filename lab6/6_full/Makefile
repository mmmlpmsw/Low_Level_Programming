all: main.o
	gcc -o lab6 main.o image.o image_io.o image_utils.o

main.o: image_io.o image_utils.o image.o main.c
	gcc -c -o main.o main.c

image_utils.o: image.o image_utils.c
	gcc -c -o image_utils.o image_utils.c

image_io.o: image.o image_io.c
	gcc -c -o image_io.o image_io.c

image.o: image.c
	gcc -c -o image.o image.c

clean:
	rm lab6 *.o