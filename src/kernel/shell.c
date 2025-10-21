#include "io.h"
#include <shell.h>
#include <interrupts.h>

extern unsigned int system_ticks;
/* Forward declarations for cursor control */
/*extern unsigned short cursor_x;
/*extern unsigned short cursor_y;

/* Track cursor visibility for blinking */
static int cursor_visible = 0;

static char input_buf[256];
static int input_pos = 0;
static unsigned short cursor_x = 8, cursor_y = 2;

void print_string(const char *str, int x, int y, unsigned char color) {
    volatile unsigned char *video = (volatile unsigned char *)0xB8000 + (y * 80 + x) * 2;
    while (*str) {
        *video++ = *str++;
        *video++ = color;
    }
}

static void update_cursor(void) {
    unsigned short pos = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

static void clear_screen(void) {
    volatile unsigned char *video = (volatile unsigned char *)0xB8000 + 2 * 80 * 2; // Start at row 2
    for (int i = 0; i < 80 * 23 * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
}

static void print_prompt(void) {
    print_string("amoeba> ", 0, cursor_y, 0x07);
    cursor_x = 8;
    update_cursor();
}

void init_shell(void) {
    print_string("Hi there, I am CyberMorph's First OS - Amoeba 0.3.0", 0, 0, 0x0A);
    cursor_y = 2;
    print_prompt();
}

static void clear_cmd(void) {
    clear_screen();
    print_string("Hi there, I am CyberMorph's First OS - Amoeba 0.2.3", 0, 0, 0x0A);
    cursor_y = 2;
    print_prompt();
}

static void help_cmd(void) {
    print_string("Commands: clear, help, version, echo, halt, info", 0, cursor_y, 0x07);
    cursor_y += 1;
}

static void version_cmd(void) {
    print_string("Amoeba OS 0.2.3 - CyberMorph's First OS", 0, cursor_y, 0x07);
    cursor_y += 1;
}

static void echo_cmd(const char *arg) {
    if (*arg == '\0') {
        print_string("(empty)", 0, cursor_y, 0x07);
    } else {
        print_string(arg, 0, cursor_y, 0x07);
    }
    cursor_y += 1;
}

static void halt_cmd(void) {
    print_string("System halted.", 0, cursor_y, 0x07);
    cursor_y += 1;
    asm volatile("hlt");
}

static void info_cmd(void) {
    char buf[32];
    print_string("Amoeba OS 0.2.3", 0, cursor_y, 0x07);
    // Simple itoa for ticks
    int i = 0, ticks = system_ticks;
    if (ticks == 0) buf[i++] = '0';
    else {
        while (ticks > 0) {
            buf[i++] = '0' + (ticks % 10);
            ticks /= 10;
        }
    }
    buf[i] = '\0';
    // Reverse string
    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = temp;
    }
    print_string("Uptime ticks: ", 0, cursor_y + 1, 0x07);
    print_string(buf, 14, cursor_y + 1, 0x07);
    cursor_y += 2;
}

static void to_lowercase(char *str) {
    for (; *str; str++) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + 32;
        }
    }
}

static int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
    char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
    return c1 - c2;
}

void shell_handle_input(char c) {
    if (c == '\n') {
        input_buf[input_pos] = '\0';
        cursor_y++;  // Move to the next line for command output
        cursor_x = 0;
        update_cursor();

        char *cmd = input_buf;
        char *arg = input_buf;
        while (*arg && *arg != ' ') arg++;
        if (*arg == ' ') {
            *arg = '\0';
            arg++;
        }

        if (strcmp(cmd, "clear") == 0) clear_cmd();
        else if (strcmp(cmd, "help") == 0) help_cmd();
        else if (strcmp(cmd, "version") == 0) version_cmd();
        else if (strcmp(cmd, "echo") == 0) echo_cmd(arg);
        else if (strcmp(cmd, "halt") == 0) halt_cmd();
        else if (strcmp(cmd, "info") == 0) info_cmd();
        else if (input_buf[0]) {
            print_string("Unknown command: ", 0, cursor_y, 0x07);
            print_string(input_buf, 17, cursor_y, 0x07);
            cursor_y += 1;
        }

        input_pos = 0;
        input_buf[0] = '\0';
        if (cursor_y >= 25) {
            clear_cmd();
        } else {
            print_prompt();
        }
    } else if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            input_buf[input_pos] = '\0';
            cursor_x--;
            print_string(" ", cursor_x, cursor_y, 0x07);
            update_cursor();
        }
    } else if (input_pos < 255) {
        input_buf[input_pos++] = c;
        print_string(&c, cursor_x, cursor_y, 0x07);
        cursor_x++;
        update_cursor();
    }
}
void toggle_cursor(void) {
    volatile unsigned char *video = (volatile unsigned char *)0xB8000;
    unsigned int pos = (cursor_y * 80 + cursor_x) * 2;

    /* Read current character */
    unsigned char current_char = video[pos];

    if (cursor_visible) {
        /* Restore a blank space when cursor is "off" */
        video[pos] = ' ';
    } else {
        /* Draw an underscore when cursor is "on" */
        video[pos] = '_';
    }

    cursor_visible = !cursor_visible;
}



