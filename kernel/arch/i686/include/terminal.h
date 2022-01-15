#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <stdint.h>

enum TerminalColour
{
    TERMINAL_BLACK = 0,
    TERMINAL_BLUE = 1,
    TERMINAL_GREEN = 2,
    TERMINAL_CYAN = 3,
    TERMINAL_RED = 4,
    TERMINAL_MAGENTA = 5,
    TERMINAL_BROWN = 6,
    TERMINAL_LIGHT_GREY = 7,
    TERMINAL_DARK_GREY = 8,
    TERMINAL_LIGHT_BLUE = 9,
    TERMINAL_LIGHT_GREEN = 10,
    TERMINAL_LIGHT_CYAN = 11,
    TERMINAL_LIGHT_RED = 12,
    TERMINAL_LIGHT_MAGENTA = 13,
    TERMINAL_LIGHT_BROWN = 14,
    TERMINAL_WHITE = 15
};

void terminal_clear(void);

void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void terminal_disable_cursor(void);

void terminal_get_colour(enum TerminalColour* foreground, enum TerminalColour* background);
void terminal_set_colour(enum TerminalColour foreground, enum TerminalColour background);

void terminal_write_char(char ch);
void terminal_write_string(const char *str);

void terminal_init(void);

#endif
