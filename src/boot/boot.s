[bits 32]

; Multiboot header constants
MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS  equ 0x0
MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

extern kernel_main
global _start

section .multiboot
align 4
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

section .text
_start:
    ; Set up stack
    mov esp, 0x20000
    mov ebp, esp

    ; Set segment registers
    mov ax, 0x18
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Hide BIOS cursor
    mov dx, 0x3D4
    mov al, 0x0A
    out dx, al
    inc dx
    mov al, 0x20
    out dx, al

    ; Call kernel
    call kernel_main

    ; Halt
    cli
    hlt

section .note.GNU-stack
