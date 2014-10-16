#!/bin/sh

rm kernel *.o
nasm -f elf32 kernel.asm -o kasm.o
gcc -m32 -c kernel.c -o kc.o
gcc -m32 -c shell.c -o shell.o
gcc -m32 -std=c99 -c func.c -o func.o
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o shell.o func.o
qemu-system-i386 -kernel kernel
