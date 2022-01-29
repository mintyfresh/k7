#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <keyboard.h>
#include <logger.h>
#include <multiboot.h>
#include <pit.h>
#include <pmm.h>
#include <stdlib.h>
#include <terminal.h>
#include <vmm.h>

void kernel_main(uint32_t magic, multiboot_info_t* mbi)
{
    terminal_init();
    log_debug("Hello, kernel World!");

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        log_error("Kernel loaded with invalid magic number: %x", magic);
        return;
    }

    gdt_init();
    log_debug("GDT loaded.");

    idt_init();
    log_debug("IDT loaded.");
    
    irq_init();
    log_debug("IRQ handlers configured.");

    pit_init();
    log_debug("PIT IRQ configured.");

    keyboard_init();
    log_debug("Keyboard IRQ configured.");

    pmm_init(mbi);
    log_debug("Physical memory manager initialized.");

    vmm_init();

    for (;;)
    {
        asm volatile("hlt");
    }
}
