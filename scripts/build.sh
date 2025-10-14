#!/bin/bash
set -e  # Exit on error

# Clean build directory
rm -rf build/*
mkdir -p build

# Assemble boot code
nasm -f elf32 src/boot/boot.s -o build/boot.o
nasm -f elf32 src/boot/idt.s -o build/idt.o

# Compile kernel
gcc -m32 -ffreestanding -I src/include -c src/kernel/kernel.c -o build/kernel.o
gcc -m32 -ffreestanding -I src/include -c src/kernel/idt.c -o build/idt_kernel.o
gcc -m32 -ffreestanding -I src/include -c src/kernel/pic.c -o build/pic.o

# Link

# Update link command
ld -m elf_i386 -T config/linker.ld \
    build/boot.o build/idt.o \
    build/kernel.o build/idt_kernel.o build/pic.o \
    -o build/kernel.bin
# Create ISO
mkdir -p iso/boot/grub
cp build/kernel.bin iso/boot/
cp config/grub/grub.cfg iso/boot/grub/
grub-mkrescue -o dist/amoeba.iso iso

echo "Build complete: dist/amoeba.iso"
