#ifndef __PIC_H__
#define __PIC_H__

#include <stdint.h>

void pic_remap(uint8_t master_offset, uint8_t slave_offset);
void pic_send_eoi(uint8_t irq);

void pic_enable_all(void);
void pic_disable_all(void);

void pic_enable_irq(uint8_t irq);
void pic_disable_irq(uint8_t irq);

uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

#endif
