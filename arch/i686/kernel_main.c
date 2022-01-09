#include "terminal.h"

void kernel_main(void)
{
    terminal_init();
    terminal_write_string("Hello, kernel World!\n");
}
