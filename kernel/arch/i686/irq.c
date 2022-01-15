#include <hal.h>
#include <pic.h>
#include <port_io.h>
#include <stdio.h>

void irq_ack(uint8_t irq)
{
    pic_send_eoi(irq);
}

void irq_init(void)
{
    pic_remap(0x20, 0x28);
    pic_mask_all();
    pic_clear_mask(0x01);

    hal_enable_interrupts();
}
