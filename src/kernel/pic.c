#include "interrupts.h"

void outb(unsigned short port, unsigned char val) {
    // Write byte to I/O port
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    // Read byte from I/O port
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void pic_init(void) {
    // ICW1: Initialize PICs (start configuration)
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // ICW2: Map IRQs to interrupt vectors
    outb(PIC1_DATA, 0x20);  // IRQ0-7 → interrupts 32-39
    outb(PIC2_DATA, 0x28);  // IRQ8-15 → interrupts 40-47

    // ICW3: Configure master/slave wiring
    outb(PIC1_DATA, 0x04);  // Master PIC: IRQ2 connected to slave
    outb(PIC2_DATA, 0x02);  // Slave PIC: connected to master's IRQ2

    // ICW4: Set 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // OCW1: Disable all interrupts except IRQ1 (keyboard)
    outb(PIC1_DATA, 0xFD);  // 0xFD = 11111101, enables IRQ1
    outb(PIC2_DATA, 0xFF);  // Disable all slave interrupts
}

void pic_ack(unsigned int irq) {
    // Send End-of-Interrupt to PIC
    if (irq >= 40) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}
