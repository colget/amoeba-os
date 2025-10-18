#include <interrupts.h>
#include <shell.h>

unsigned int system_ticks = 0;

void kernel_main(void) {
    // Clear screen using print_string from shell.c
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            print_string(" ", x, y, 0x0F);
        }
    }

    // Initialize interrupts
    init_idt();
    pic_init();

    // Initialize shell
    init_shell();

    // Enable interrupts
    __asm__ volatile ("sti");

    while (1) {
        system_ticks++; // Increment ticks
        __asm__ volatile ("hlt");
    }
}
