#!/bin/bash
# Compile assembly
nasm -f elf32 src/boot/boot.s -o build/boot.o
# Compile C
gcc -m32 -ffreestanding -c src/kernel/kernel.c -o build/kernel.o
# Link
ld -m elf_i386 -T config/linker.ld -o build/kernel.bin build/boot.o build/kernel.o
# Prepare ISO structure
mkdir -p iso/boot/grub
cp build/kernel.bin iso/boot/
cp config/grub/grub.cfg iso/boot/grub/
# Create ISO
grub-mkrescue -o dist/amoeba.iso iso
