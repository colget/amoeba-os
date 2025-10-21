[bits 32]

; Common interrupt stub (no error code)
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli
        push 0              ; Dummy error code
        push %1             ; Interrupt number
        jmp exception_stub
%endmacro

; Common interrupt stub (with error code)
%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli
        push %1             ; Interrupt number
        jmp exception_stub
%endmacro

; Define ISRs for CPU exceptions (0-31)
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; Timer interrupt (32)
global isr32
isr32:
    cli
    push 0
    push 32
    jmp pit_stub


; Keyboard interrupt (33)
global isr33
isr33:
    cli
    push 0
    push 33
    jmp keyboard_stub

; Exception stub
exception_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x18        ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, [esp + 44] ; Get interrupt number (4*11 bytes up)
    mov ebx, [esp + 48] ; Get error code
    push ebx            ; Push error code
    push eax            ; Push interrupt number
    extern exception_handler
    call exception_handler
    add esp, 8          ; Clean parameters

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8          ; Clean interrupt number and error code
    sti
    iret

pit_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x18
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern interrupt_handler_32
    call interrupt_handler_32

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    sti
    iret


; Keyboard stub
keyboard_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x18
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern interrupt_handler_33
    call interrupt_handler_33

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    sti
    iret

section .note.GNU-stack
