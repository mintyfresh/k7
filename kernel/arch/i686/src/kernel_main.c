#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <stdlib.h>
#include <stdio.h>
#include <terminal.h>
#include <keyboard.h>
#include <pit.h>
#include <multiboot.h>

void kernel_main(uint32_t magic, multiboot_info_t* memory_map)
{
    terminal_init();
    printf("Hello, kernel World!\n");

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printf("Kernel loaded with invalid magic number: %x\n", magic);
        return;
    }

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
