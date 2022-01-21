#include <hal.h>
#include <pic.h>
#include <port_io.h>
#include <stdio.h>
#include <stddef.h>
#include <idt.h>
#include <irq.h>

#define IRQ_BASE 0x20

void irq_ack(uint8_t irq)
{
    pic_send_eoi(irq);
}

static irq_handler_t irq_handlers[16] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

irq_handler_t irq_get_handler(uint8_t irq)
{
    return irq_handlers[irq];
}

void irq_set_handler(uint8_t irq, irq_handler_t handler)
{
    irq_handlers[irq] = handler;
}

void irq_handler(struct MachineState* state)
{
    uint8_t irq = state->interrupt - IRQ_BASE;
    irq_handler_t handler = irq_get_handler(irq);

    if (handler)
    {
        handler(state);
    }

    irq_ack(irq);
}

extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);

void irq_init(void)
{
    pic_disable_all();
    pic_remap(IRQ_BASE, IRQ_BASE + 8);

    pic_enable_irq(0);
    pic_enable_irq(1);

    idt_set_entry(IRQ_BASE + 0, isr32);
    idt_set_entry(IRQ_BASE + 1, isr33);
    idt_set_entry(IRQ_BASE + 2, isr34);
    idt_set_entry(IRQ_BASE + 3, isr35);
    idt_set_entry(IRQ_BASE + 4, isr36);
    idt_set_entry(IRQ_BASE + 5, isr37);
    idt_set_entry(IRQ_BASE + 6, isr38);
    idt_set_entry(IRQ_BASE + 7, isr39);
    idt_set_entry(IRQ_BASE + 8, isr40);
    idt_set_entry(IRQ_BASE + 9, isr41);
    idt_set_entry(IRQ_BASE + 10, isr42);
    idt_set_entry(IRQ_BASE + 11, isr43);
    idt_set_entry(IRQ_BASE + 12, isr44);
    idt_set_entry(IRQ_BASE + 13, isr45);
    idt_set_entry(IRQ_BASE + 14, isr46);
    idt_set_entry(IRQ_BASE + 15, isr47);

    hal_enable_interrupts();
}
