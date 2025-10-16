#include "interrupts.h"

extern void putchar(char c, int x, int y, char color);
extern void print_string(const char* str, int start_x, int start_y, char color);

static int cursor_x = 10;
static int cursor_y = 12;

char scancode_to_ascii(unsigned char scancode) {
    static char scancode_map[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x0E = backspace
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    if (scancode < sizeof(scancode_map) && !(scancode & 0x80)) {
        return scancode_map[scancode];
    }
    return 0;
}

void interrupt_handler_33(void) {
    unsigned char scancode = inb(0x60);

    // Debug: Print scancode in hex at top-left
    char buf[4];
    buf[0] = (scancode / 16) > 9 ? 'A' + (scancode / 16) - 10 : '0' + (scancode / 16);
    buf[1] = (scancode % 16) > 9 ? 'A' + (scancode % 16) - 10 : '0' + (scancode % 16);
    buf[2] = '\0';
    print_string("Scancode: 0x", 0, 1, 0x0E);
    print_string(buf, 12, 1, 0x0E);

    char c = scancode_to_ascii(scancode);
    if (c) {
        if (c == '\n') {
            cursor_x = 10;
            cursor_y++;
            if (cursor_y >= 25) {
                cursor_y = 12;
                // Clear input area (rows 12-24)
                for (int y = 12; y < 25; y++) {
                    for (int x = 0; x < 80; x++) {
                        putchar(' ', x, y, 0x0F);
                    }
                }
            }
        } else if (c == '\b') {
            if (cursor_x > 10) {
                cursor_x--;
                putchar(' ', cursor_x, cursor_y, 0x0F);
            }
        } else {
            putchar(c, cursor_x, cursor_y, 0x0F);
            cursor_x++;
            if (cursor_x >= 80) {
                cursor_x = 10;
                cursor_y++;
                if (cursor_y >= 25) {
                    cursor_y = 12;
                    // Clear input area (rows 12-24)
                    for (int y = 12; y < 25; y++) {
                        for (int x = 0; x < 80; x++) {
                            putchar(' ', x, y, 0x0F);
                        }
                    }
                }
            }
        }
    }
    pic_ack(1);
}
