ASM=nasm
FLAGS=-f elf64 -g

all: main

dict.o: dict.asm
	$(ASM) $(FLAGS) -o dict.o dict.asm

lib.o: lib.asm
	$(ASM) $(FLAGS) -o lib.o lib.asm

main.o: main.asm colon.inc words.inc
	$(ASM) $(FLAGS) -o main.o main.asm

main: main.o dict.o lib.o
	ld -o main main.o dict.o lib.o

clean:
	rm -f *.o