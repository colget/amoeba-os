#!/bin/bash
set -e

rm -rf build/*
mkdir -p build dist iso/boot/grub

# Assemble
nasm -f elf32 src/boot/boot.s -o build/boot.o
nasm -f elf32 src/boot/idt.s -o build/idt.o
nasm -f elf32 src/boot/interrupt.s -o build/interrupt.o

# Compile
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/kernel.c -o build/kernel.o
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/idt.c -o build/idt_kernel.o
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/exception.c -o build/exception.o
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/keyboard.c -o build/keyboard.o
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/pic.c -o build/pic.o
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/shell.c -o build/shell.o
gcc -m32 -ffreestanding -fno-pic -fno-plt -I src/include -c src/kernel/pit.c -o build/pit.o

# Link
ld -m elf_i386 -T config/linker.ld \
    build/boot.o build/idt.o build/interrupt.o \
    build/kernel.o build/idt_kernel.o build/exception.o \
    build/keyboard.o build/pic.o build/shell.o build/pit.o \
    -o build/kernel.bin

# Create ISO
cp build/kernel.bin iso/boot/
cp config/grub/grub.cfg iso/boot/grub/
grub-mkrescue -o dist/amoeba.iso iso

echo "Build complete: dist/amoeba.iso"
