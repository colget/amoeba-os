#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// Interrupt Descriptor Table structure
struct idt_entry {
    unsigned short base_lo;    // Low 16 bits of handler address
    unsigned short sel;        // Kernel segment selector (0x08)
    unsigned char always0;     // Always 0
    unsigned char flags;       // Flags (described below)
    unsigned short base_hi;    // High 16 bits of handler address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    unsigned short limit;      // Size of IDT - 1
    unsigned int   base;       // Address of IDT
} __attribute__((packed));

// Flags for IDT entries
#define IDT_FLAG_PRESENT     0x80    // Present in memory
#define IDT_FLAG_RING0       0x00    // Kernel mode
#define IDT_FLAG_INTGATE     0x06    // Interrupt gate
#define IDT_FLAG_FLAGS       (IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE)

// Function declarations
void init_idt(void);
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
// PIC I/O ports
#define PIC1_COMMAND  0x20
#define PIC1_DATA     0x21
#define PIC2_COMMAND  0xA0
#define PIC2_DATA     0xA1
#define PIC_EOI       0x20

void pic_init(void);
void pic_ack(unsigned int irq);

#endif
