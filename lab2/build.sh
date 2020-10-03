#!/bin/sh

nasm -e colon.inc &&
nasm -felf64 dict.asm &&
nasm -felf64 lib.asm &&
nasm -felf64 main.asm &&
ld -o dict dict.o lib.o main.o;

rm -f *.o main

