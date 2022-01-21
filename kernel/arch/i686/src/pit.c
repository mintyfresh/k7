#include <irq.h>
#include <pit.h>
#include <port_io.h>

#define PIT_IRQ 0x00
#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND_REGISTER 0x43
#define PIT_INPUT_FREQUENCY 1193182

uint16_t pit_get_counter(void)
{
    uint16_t counter = 0;

    outb(PIT_COMMAND_REGISTER, 0b00000000);

    counter |= inb(PIT_CHANNEL_0);
    counter |= inb(PIT_CHANNEL_0) << 8;

    return counter;
}

void pit_set_counter(uint16_t counter)
{
    outb(PIT_CHANNEL_0, (counter >> 0) & 0xFF);
    outb(PIT_CHANNEL_0, (counter >> 8) & 0xFF);
}

void pit_set_frequency(uint32_t frequency)
{
    uint16_t counter = PIT_INPUT_FREQUENCY / frequency;

    pit_set_counter(counter);
}

static void pit_irq_handler(struct MachineState* _)
{
    // Nothing to do here.
    printf("PIT IRQ\n");
}

void pit_init(void)
{
    pit_set_frequency(100);
    irq_set_handler(PIT_IRQ, pit_irq_handler);
}
