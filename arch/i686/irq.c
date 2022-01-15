#include <hal.h>
#include <pic.h>
#include <port_io.h>
#include <stdio.h>

void irq_init(void)
{
    pic_remap(0x20, 0x28);
    pic_mask_all();

    hal_enable_interrupts();
}
