#include <stdio.h>
#include <stdlib.h>

// TODO: Implement better handling for charout.
extern void terminal_write_char(char ch);
extern void terminal_write_string(const char* str);

int vprintf(const char* str, va_list args)
{
    while (*str)
    {
        if (*str == '%')
        {
            switch (*++str)
            {
                case 'c':
                    terminal_write_char((char) va_arg(args, int));
                    break;

                case 's':
                    terminal_write_string(va_arg(args, const char*));
                    break;

                case 'b':
                    {
                        char buffer[33];
                        itoa(va_arg(args, int), buffer, 2);
                        terminal_write_string(buffer);
                        break;
                    }

                case 'd':
                    {
                        char buffer[33];
                        itoa(va_arg(args, int), buffer, 10);
                        terminal_write_string(buffer);
                        break;
                    }

                case 'p':
                case 'x':
                    {
                        char buffer[33];
                        itoa(va_arg(args, int), buffer, 16);
                        terminal_write_string(buffer);
                        break;
                    }

                default:
                    terminal_write_char('%');
                    terminal_write_char(*str);
                    break;
            }

            str++;
        }
        else
        {
            terminal_write_char(*str++);
        }
    }
}
