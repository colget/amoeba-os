#ifndef PIT_H
#define PIT_H

#include "interrupts.h"

/* Initialize the PIT with a given frequency (Hz) */
void pit_init(unsigned int frequency);

/* Timer interrupt handler (IRQ0, vector 32) */
void interrupt_handler_32(void);

#endif
