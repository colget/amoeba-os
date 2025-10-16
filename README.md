# Amoeba OS
A minimal operating system project, starting with a bootable kernel that prints "Hi there, I am CyberMorph's First OS - Amoeba 0.1".

## Build Instructions
1. Install NASM, GCC, Binutils, GRUB tools, and QEMU.
2. Run `./scripts/build.sh` to build `dist/amoeba.iso`.
3. Test with `qemu-system-i386 -cdrom dist/amoeba.iso` or VirtualBox.

## Directory Structure
- `src/`: Source code (bootloader, kernel).
- `config/`: Linker and GRUB configs.
- `scripts/`: Build scripts.
- `build/`: Generated object files and binaries (ignored).
- `dist/`: Bootable ISO (ignored).
Amoeba 0.2: Fixed Multiboot header, added backspace, and resolved screen overwrite
Amoeba 0.2: Fixed Multiboot, backspace, and screen overwrite
