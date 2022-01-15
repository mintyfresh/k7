#include <irq.h>
#include <keyboard.h>
#include <port_io.h>
#include <stddef.h>
#include <stdio.h>

#define KEYBOARD_IRQ 0x01

static inline uint8_t keyboard_scan_code(void)
{
    return inb(0x60);
}

void isr_33_handler(void* _)
{
    size_t i;
    uint8_t buffer[8];

    for (i = 0; i < 8; i++)
    {
        uint8_t scan_code = keyboard_scan_code();

        if (i > 0 && scan_code == buffer[8 - i])
        {
            break;
        }

        buffer[8 - i - 1] = scan_code;
    }
    for (; i < 8; i++)
    {
        buffer[8 - i - 1] = 0;
    }

    keyboard_key_event(*((scan_code_t*) &buffer));
    irq_ack(KEYBOARD_IRQ);
}
