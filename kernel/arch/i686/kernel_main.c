#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <terminal.h>

void kernel_main(void)
{
    terminal_init();
    terminal_write_string("Hello, kernel World!\n");

    gdt_init();
    terminal_write_string("GDT loaded.\n");

    idt_init();
    terminal_write_string("IDT loaded.\n");
    
    irq_init();
    terminal_write_string("IRQ handlers configured.\n");

    for (;;)
    {
        asm volatile("hlt");
    }
}