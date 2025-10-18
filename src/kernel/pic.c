#include "interrupts.h"

void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void pic_init(void) {
    // Initialize PICs
    outb(PIC1_COMMAND, 0x11); // ICW1: Initialize
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);    // ICW2: Vector offset (32)
    outb(PIC2_DATA, 0x28);    // ICW2: Vector offset (40)
    outb(PIC1_DATA, 0x04);    // ICW3: Master has slave at IRQ2
    outb(PIC2_DATA, 0x02);    // ICW3: Slave ID
    outb(PIC1_DATA, 0x01);    // ICW4: 8086 mode
    outb(PIC2_DATA, 0x01);

    // Mask all IRQs except IRQ1 (keyboard)
    outb(PIC1_DATA, 0xFD); // 11111101 (IRQ1 enabled)
    outb(PIC2_DATA, 0xFF); // All slave IRQs disabled
}

void pic_ack(unsigned int irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}
