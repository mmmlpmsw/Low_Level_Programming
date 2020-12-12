# all: main.o
# 	gcc -o lab6 main.o image.o image_io.o image_utils.o -lm
#
# main.o: image_io.o image_utils.o image.o main.c
# 	gcc -c -o main.o main.c
#
# image_utils.o: image.o image_utils.c
# 	gcc -c -o image_utils.o image_utils.c
#
# image_io.o: image.o image_io.c
# 	gcc -c -o image_io.o image_io.c
#
# image.o: image.c
# 	gcc -c -o image.o image.c
#
# clean:
# 	rm lab6 *.o

C=gcc
CFLAGS=-std=c11 -g

clean:
	rm *.o

all: lab7

lab7: *.asm *.c
		nasm -f elf64 sepia.asm
			$(CC) $(CFLAGS) -c *.c
				$(CC) -o lab7  *.o -no-pie
