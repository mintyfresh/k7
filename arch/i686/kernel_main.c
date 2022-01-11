#include "gdt.h"
#include "idt.h"
#include "terminal.h"

void kernel_main(void)
{
    terminal_init();
    terminal_write_string("Hello, kernel World!\n");

    gdt_init();
    terminal_write_string("GDT loaded.\n");

    idt_init();
    terminal_write_string("IDT loaded.\n");

    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
