#include <interrupts.h>
#include <shell.h>
#include "pit.h"   // For PIT initialization and timer interrupt handling

/* 
 * Global system tick counter.
 * Incremented inside the PIT interrupt handler (interrupt_handler_32).
 */
unsigned int system_ticks = 0;

/*
 * Amoeba OS Kernel Entry Point
 * ----------------------------
 * The kernel_main() function is called after the bootloader loads the kernel
 * and sets up the basic environment. It initializes the Interrupt Descriptor
 * Table (IDT), the Programmable Interrupt Controller (PIC), and the 
 * Programmable Interval Timer (PIT), then launches the shell.
 */
void kernel_main(void) {
    // ------------------------------------------------------------
    // 1. Clear the VGA text screen
    // ------------------------------------------------------------
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            print_string(" ", x, y, 0x0F);  // White space on black background
        }
    }

    // ------------------------------------------------------------
    // 2. Initialize the Interrupt Descriptor Table (IDT)
    // ------------------------------------------------------------
    init_idt();

    // ------------------------------------------------------------
    // 3. Initialize the Programmable Interrupt Controller (PIC)
    // ------------------------------------------------------------
    pic_init();

    // ------------------------------------------------------------
    // 4. Initialize the Programmable Interval Timer (PIT)
    // ------------------------------------------------------------
    // Set PIT to generate 100 interrupts per second (100 Hz).
    // This drives system timing (cursor blinking, uptime counter, etc.)
    pit_init(100);

    // ------------------------------------------------------------
    // 5. Initialize the shell (text-based user interface)
    // ------------------------------------------------------------
    init_shell();

    // ------------------------------------------------------------
    // 6. Enable CPU interrupts globally
    // ------------------------------------------------------------
    __asm__ volatile ("sti");

    // ------------------------------------------------------------
    // 7. Idle loop (halt until next interrupt)
    // ------------------------------------------------------------
    // The CPU will sleep until an interrupt (like the PIT or keyboard)
    // wakes it. The PIT handler increments system_ticks.
    while (1) {
        __asm__ volatile ("hlt");
    }
}
