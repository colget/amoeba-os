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

void kernel_main() {
    // Clear screen (fill with spaces, black bg/white fg)
    for (int i = 0; i < 80 * 25; i++) {
        putchar(' ', i % 80, i / 80, 0x0F);  // 0x0F = white on black
    }

    // Print your message at row 10, column 10, green text (0x0A)
    print_string("Hi there, I am CyberMorph's First OS - Amoeba 0.1", 10, 10, 0x0A);
    
    // Infinite loop (kernel doesn't exit)
    while (1) {}
}
