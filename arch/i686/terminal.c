#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

size_t const TERMINAL_HEIGHT = 25;
size_t const TERMINAL_WIDTH  = 80;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_colour;
uint16_t* terminal_buffer;

static inline uint16_t terminal_encode_char(char ch)
{
    return (terminal_colour << 8) | (ch & 0xFF);
}

static void terminal_scroll(void)
{
    for (size_t row = 0; row < TERMINAL_HEIGHT - 1; row++)
    {
        size_t offset = row * TERMINAL_WIDTH;

        for (size_t col = 0; col < TERMINAL_WIDTH; col++)
        {
            terminal_buffer[offset + col] = terminal_buffer[offset + col + TERMINAL_WIDTH];
        }
    }

    // Clear the last row
    for (size_t col = 0; col < TERMINAL_WIDTH; col++)
    {
        terminal_buffer[(TERMINAL_HEIGHT - 1) * TERMINAL_WIDTH + col] = terminal_encode_char(' ');
    }
}

void terminal_clear(void)
{
    for (size_t row = 0; row < TERMINAL_HEIGHT; row++)
    {
        for (size_t col = 0; col < TERMINAL_WIDTH; col++)
        {
            const size_t offset = row * TERMINAL_WIDTH + col;

            terminal_buffer[offset] = terminal_encode_char(' ');
        }
    }

    terminal_row = 0;
    terminal_col = 0;
}

void terminal_get_colour(enum TerminalColour* foreground, enum TerminalColour* background)
{
    *foreground = (terminal_colour >> 4) & 0x0F;
    *background = terminal_colour & 0x0F;
}

void terminal_set_colour(enum TerminalColour foreground, enum TerminalColour background)
{
    terminal_colour = ((foreground & 0x0F) << 4) | (background & 0x0F);
}

void terminal_write_char(char ch)
{
    switch (ch)
    {
        case '\n':
            terminal_row++;
            terminal_col = 0;
            break;

        case '\t':
            terminal_col += 4;
            break;

        default:
            terminal_buffer[terminal_row * TERMINAL_WIDTH + terminal_col] = terminal_encode_char(ch);
            terminal_col++;
            break;
    }

    if (terminal_col >= TERMINAL_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }

    if (terminal_row >= TERMINAL_HEIGHT)
    {
        terminal_scroll();
        terminal_row = TERMINAL_HEIGHT - 1;
    }
}

void terminal_write_string(const char* str)
{
    while (*str)
    {
        terminal_write_char(*str++);
    }
}

void terminal_init(void)
{
    terminal_row    = 0;
    terminal_col    = 0;
    terminal_buffer = (uint16_t*) 0xB8000;

    terminal_set_colour(TERMINAL_BLACK, TERMINAL_WHITE);
    terminal_clear();
}
