#ifndef __IRQ_H__
#define __IRQ_H__

#include <idt.h>
#include <stdint.h>

typedef void (*irq_handler_t)(struct MachineState*);

irq_handler_t irq_get_handler(uint8_t irq);
void irq_set_handler(uint8_t irq, irq_handler_t handler);

void irq_init(void);

#endif
