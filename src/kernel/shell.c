#include "shell.h"
#include "interrupts.h"

extern void putchar(char c, int x, int y, char color);
extern void print_string(const char* str, int start_x, int start_y, char color);

// Global uptime ticks (incremented in kernel.c)
extern unsigned int system_ticks;

static int cursor_x = 8;  // After "amoeba> "
static int cursor_y = 2;  // Prompt row
static char input_buffer[256];
static int buffer_pos = 0;

static void print_prompt(void) {
    print_string("amoeba> ", 0, cursor_y, 0x0F);
    cursor_x = 8; // After "amoeba> "
}

static int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

static void clear_cmd(void) {
    for (int i = 0; i < 80 * 25; i++) {
        putchar(' ', i % 80, i / 80, 0x0F);
    }
    cursor_x = 8;
    cursor_y = 2;
    print_string("Hi there, I am CyberMorph's First OS - Amoeba 0.2.2", 0, 0, 0x0A);
    print_prompt();
}

static void help_cmd(void) {
    cursor_y++;
    print_string("Available commands:", 0, cursor_y++, 0x0F);
    print_string("  clear - Clear the screen", 0, cursor_y++, 0x0F);
    print_string("  help - Show this help", 0, cursor_y++, 0x0F);
    print_string("  version - Show version info", 0, cursor_y++, 0x0F);
    print_string("  echo <text> - Echo text back", 0, cursor_y++, 0x0F);
    print_string("  halt - Stop the OS", 0, cursor_y++, 0x0F);
    print_string("  info - Show system info", 0, cursor_y++, 0x0F);
    print_prompt();
}

static void version_cmd(void) {
    cursor_y++;
    print_string("Amoeba OS 0.2.2 - CyberMorph's First OS", 0, cursor_y++, 0x0F);
    print_prompt();
}

static void echo_cmd(const char* args) {
    cursor_y++;
    if (args && *args) {
        print_string(args, 0, cursor_y++, 0x0F);
    } else {
        print_string("(empty)", 0, cursor_y++, 0x0F);
    }
    print_prompt();
}

static void halt_cmd(void) {
    cursor_y++;
    print_string("System halted.", 0, cursor_y++, 0x0C);
    while (1) {
        __asm__ volatile ("cli; hlt");
    }
}

static void info_cmd(void) {
    cursor_y++;
    print_string("Amoeba OS 0.2.2", 0, cursor_y++, 0x0F);
    // Convert ticks to string
    char buf[16];
    int i = 0;
    unsigned int ticks = system_ticks;
    if (ticks == 0) {
        buf[i++] = '0';
    } else {
        while (ticks > 0) {
            buf[i++] = '0' + (ticks % 10);
            ticks /= 10;
        }
    }
    buf[i] = '\0';
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buf[j];
        buf[j] = buf[k];
        buf[k] = temp;
    }
    print_string("Uptime ticks: ", 0, cursor_y, 0x0F);
    print_string(buf, 14, cursor_y++, 0x0F);
    print_prompt();
}

static void process_command(void) {
    input_buffer[buffer_pos] = '\0';
    char* cmd = input_buffer;
    char* args = 0;

    // Find arguments (skip command)
    for (int i = 0; input_buffer[i]; i++) {
        if (input_buffer[i] == ' ') {
            input_buffer[i] = '\0';
            args = &input_buffer[i + 1];
            break;
        }
    }

    // Execute command
    if (strcmp(cmd, "clear") == 0) {
        clear_cmd();
    } else if (strcmp(cmd, "help") == 0) {
        help_cmd();
    } else if (strcmp(cmd, "version") == 0) {
        version_cmd();
    } else if (strcmp(cmd, "echo") == 0) {
        echo_cmd(args);
    } else if (strcmp(cmd, "halt") == 0) {
        halt_cmd();
    } else if (strcmp(cmd, "info") == 0) {
        info_cmd();
    } else if (buffer_pos > 0) {
        cursor_y++;
        print_string("Unknown command: ", 0, cursor_y, 0x0C);
        print_string(cmd, 17, cursor_y++, 0x0C);
        print_prompt();
    } else {
        cursor_y++;
        print_prompt();
    }

    buffer_pos = 0;
}

void init_shell(void) {
    print_string("Hi there, I am CyberMorph's First OS - Amoeba 0.2.2", 0, 0, 0x0A);
    print_prompt();
}

void shell_handle_input(char c) {
    if (c == '\n') {
        process_command();
    } else if (c == '\b') {
        if (buffer_pos > 0) {
            buffer_pos--;
            cursor_x--;
            putchar(' ', cursor_x, cursor_y, 0x0F);
        }
    } else {
        if (buffer_pos < 255) {
            input_buffer[buffer_pos++] = c;
            putchar(c, cursor_x, cursor_y, 0x0F);
            cursor_x++;
            if (cursor_x >= 80) {
                cursor_x = 8;
                cursor_y++;
                if (cursor_y >= 25) {
                    cursor_y = 2;
                    for (int y = 2; y < 25; y++) {
                        for (int x = 0; x < 80; x++) {
                            putchar(' ', x, y, 0x0F);
                        }
                    }
                    print_prompt();
                }
            }
        }
    }
}
