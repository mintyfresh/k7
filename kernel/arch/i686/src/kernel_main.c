#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <stdlib.h>
#include <stdio.h>
#include <terminal.h>
#include <keyboard.h>
#include <pit.h>

void kernel_main(void)
{
    terminal_init();
    printf("Hello, kernel World!\n");

    gdt_init();
    printf("GDT loaded.\n");

    idt_init();
    printf("IDT loaded.\n");
    
    irq_init();
    printf("IRQ handlers configured.\n");

    pit_init();
    printf("PIT IRQ configured.\n");

    keyboard_init();
    printf("Keyboard IRQ configured.\n");

    for (;;)
    {
        asm volatile("hlt");
    }
}
