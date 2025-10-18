[bits 32]

global idt_load
section .text

idt_load:
    mov eax, [esp + 4]  ; Get pointer to idt_ptr
    lidt [eax]          ; Load IDT
    ret

section .note.GNU-stack
