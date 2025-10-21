#include "io.h"
#include "pit.h"
#include "pic.h"
#include "shell.h"

/* PIT I/O ports */
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_BASE_HZ  1193182U   /* PIT input clock frequency */

static volatile unsigned long pit_ticks = 0;
extern unsigned int system_ticks;  /* defined in kernel.c */

/*
 * Initialize the PIT to a given frequency (Hz)
 * Example: pit_init(100) → 100 interrupts per second (10ms period)
 */
void pit_init(unsigned int frequency) {
    unsigned int divisor = PIT_BASE_HZ / frequency;

    /* Channel 0, access mode = lobyte/hibyte, mode 2 (rate generator) */
    outb(PIT_COMMAND, 0x34);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, divisor >> 8);
}

/*
 * IRQ0 handler (PIT timer)
 * ------------------------------------------------------------
 * Triggered ~every 10ms if frequency = 100Hz.
 * Increments global tick counters and toggles cursor every 50 ticks (~0.5s).
 */
void interrupt_handler_32(void) {
    pit_ticks++;
    system_ticks++;

    /* Toggle cursor every 50 ticks (~0.5s at 100Hz) */
    if (pit_ticks % 50 == 0) {
        toggle_cursor();
    }

    /* IMPORTANT: Do NOT print directly here — it can freeze or corrupt state.
       Use this interrupt only to update counters or flags. */

    /* Acknowledge interrupt to PIC so next IRQ0 can arrive */
    pic_ack(0);
}
