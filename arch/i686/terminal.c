#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

size_t const TERMINAL_HEIGHT = 25;
size_t const TERMINAL_WIDTH  = 80;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_clear(void)
{
    for (size_t row = 0; row < TERMINAL_HEIGHT; row++)
    {
        for (size_t col = 0; col < TERMINAL_WIDTH; col++)
        {
            const size_t offset = row * TERMINAL_WIDTH + col;

            terminal_buffer[offset] = terminal_color << 8 | ' ';
        }
    }

    terminal_row = 0;
    terminal_col = 0;
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
            terminal_buffer[terminal_row * 80 + terminal_col] = (terminal_color << 8) | ch;
            terminal_col++;
            break;
    }

    if (terminal_col >= 80)
    {
        terminal_col = 0;
        terminal_row++;
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
    terminal_color  = 0x0F;
    terminal_buffer = (uint16_t*) 0xB8000;

    terminal_clear();
}
