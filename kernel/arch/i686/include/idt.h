#ifndef __IDT_H__
#define __IDT_H__

void idt_set_entry(unsigned interrupt, void (*isr)(void*));

void idt_init(void);

#endif
