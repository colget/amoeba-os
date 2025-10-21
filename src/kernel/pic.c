#include "io.h"
#include <interrupts.h>

/*
 * PIC (Programmable Interrupt Controller) - 8259A
 * ------------------------------------------------
 * The x86 platform has two cascaded PICs:
 *  - Master PIC: handles IRQ0–7
 *  - Slave  PIC: handles IRQ8–15
 *
 * IRQs are hardware interrupts (timer, keyboard, etc.)
 * The PIC needs to be remapped because the default
 * IRQ vectors (0x08–0x0F) overlap with CPU exceptions.
 *
 * We remap them to 0x20–0x2F:
 *  - Master handles 0x20–0x27 (IRQ0–7)
 *  - Slave  handles 0x28–0x2F (IRQ8–15)
 */

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define PIC_EOI      0x20  /* End of Interrupt command */

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

/* Remap the PICs so that IRQ0–15 are mapped to IDT entries 0x20–0x2F */
void pic_init(void) {
    unsigned char master_mask, slave_mask;

    /* Save current interrupt masks */
    master_mask = inb(PIC1_DATA);
    slave_mask  = inb(PIC2_DATA);

    /* Start the initialization sequence (cascade mode) */
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /* Remap offsets */
    outb(PIC1_DATA, 0x20);  // Master offset: 0x20 (32)
    outb(PIC2_DATA, 0x28);  // Slave offset:  0x28 (40)

    /* Tell Master PIC that there is a slave at IRQ2 (0000 0100) */
    outb(PIC1_DATA, 4);

    /* Tell Slave PIC its cascade identity (0000 0010) */
    outb(PIC2_DATA, 2);

    /* Set both PICs to operate in 8086/88 mode */
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /* Restore saved masks (re-enable previously masked interrupts) */
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}

/*
 * Acknowledge the end of an interrupt (EOI)
 * -----------------------------------------
 * Every IRQ must send an EOI to the PIC to let it
 * know that the interrupt has been handled, otherwise
 * it won’t trigger further interrupts.
 *
 * If the IRQ came from the slave PIC (IRQ8–15), we must
 * acknowledge both the slave *and* the master.
 */
void pic_ack(unsigned int irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);  // Acknowledge slave PIC
    }
    outb(PIC1_COMMAND, PIC_EOI);      // Always acknowledge master PIC
}
