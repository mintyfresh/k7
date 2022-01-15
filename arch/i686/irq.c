#include "pic.h"
#include "terminal.h"

void irq_init(void)
{
    pic_remap(0x20, 0x28);
    pic_clear_mask(0);
    // pic_set_mask(1);

    asm volatile("sti");
}

void isr_32_handler(void* _)
{
    terminal_write_string("timer");
    pic_send_eoi(0);
}
