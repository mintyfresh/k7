#ifndef __IRQ_H__
#define __IRQ_H__

#include <stdint.h>

void irq_ack(uint8_t irq);
void irq_init(void);

#endif
