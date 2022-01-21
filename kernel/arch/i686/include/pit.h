#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

uint16_t pit_get_counter(void);
void pit_set_counter(uint16_t counter);

uint32_t pit_get_frequency(void);
void pit_set_frequency(uint32_t frequency);

void pit_init(void);

#endif
