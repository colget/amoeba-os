
# Amoeba OS
An ongoing and evolving Operating System project. 

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
Amoeba 0.2.1: Added simple shell with commands: clear, help, version, echo <text>
Amoeba 0.2.1: Added simple shell with commands: clear, help, version, echo <text>
Amoeba 0.2.2: Moved welcome message to top-left, added halt and info commands, improved echo handling
Amoeba 0.2.3: Added Shift key support for uppercase letters and symbols, case-insensitive commands
Amoeba OS v0.3.0 — Blinking Cursor Build

Summary:
This version introduces real hardware interrupt handling through the PIT (Programmable Interval Timer) and PIC (Programmable Interrupt Controller).
The cursor now blinks smoothly every 0.5 seconds, powered by periodic timer interrupts.

Key Changes:

Added PIT (IRQ0) timer support running at 100 Hz (pit.c, pit.h).

Added PIC initialization (pic.c, pic.h) and proper interrupt remapping (IRQs 0–15 → 0x20–0x2F).

New low-level port I/O header io.h providing inb() and outb().

Updated shell to use a hardware-driven blinking cursor.

Build script (scripts/build.sh) updated to include new source files.

Build & Run:

./scripts/build.sh
qemu-system-i386 -cdrom dist/amoeba.iso -m 512M
When booted:

The blinking cursor is now powered by real hardware interrupts.

Next Goals:
Add uptime display using system ticks.
Implement command history and improved shell input.
