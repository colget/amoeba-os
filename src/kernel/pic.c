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
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA,    0x20);
    outb(PIC2_DATA,    0x28);
    outb(PIC1_DATA,    0x04);
    outb(PIC2_DATA,    0x02);
    outb(PIC1_DATA,    0x01);
    outb(PIC2_DATA,    0x01);
    outb(PIC1_DATA,    0xFF);
    outb(PIC2_DATA,    0xFF);
    outb(PIC1_DATA,    0xFD);
}

void pic_ack(unsigned int irq) {
    if (irq >= 40) {
        outb(PIC2_COMMAND, PIC_EOI);
        outb(PIC1_COMMAND, PIC_EOI);
    } else {
        outb(PIC1_COMMAND, PIC_EOI);
    }
}
