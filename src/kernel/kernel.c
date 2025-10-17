#include "interrupts.h"
#include "shell.h"

void putchar(char c, int x, int y, char color) {
    unsigned short* vga = (unsigned short*)0xB8000;
    vga[y * 80 + x] = (unsigned short)c | ((unsigned short)color << 8);
}

void print_string(const char* str, int start_x, int start_y, char color) {
    int x = start_x;
    int y = start_y;
    while (*str) {
        if (*str == '\n') {
            y++;
            x = 0;
        } else {
            putchar(*str, x, y, color);
            x++;
            if (x >= 80) {
                x = 0;
                y++;
            }
        }
        str++;
    }
}

unsigned int system_ticks = 0;

void kernel_main(void) {
    // Clear screen
    for (int i = 0; i < 80 * 25; i++) {
        putchar(' ', i % 80, i / 80, 0x0F);
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
