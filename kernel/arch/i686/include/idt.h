#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

struct MachineState
{
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t interrupt;
    uint32_t error_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t ss;
};

// typedef void (*isr_t)(struct MachineState*);

void idt_set_entry(uint8_t interrupt, void* handler);

void idt_init(void);

#endif
