#include <stdint.h>

#include "idt.h"

#pragma pack(push, 1)
struct IDTDescriptor
{
    uint16_t size;
    uint32_t offset;
};

struct IDTEntry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
};
#pragma pack(pop)

static struct IDTDescriptor idtr;
static struct IDTEntry idt[256];

static void idt_load_descriptor(void)
{
    idtr.size   = sizeof(idt) - 1;
    idtr.offset = (uint32_t) idt;

    asm volatile("lidt %0" : : "m"(idtr));
}

static struct IDTEntry idt_encode_entry(void* isr)
{
    struct IDTEntry entry;

    entry.offset_low  = ((uintptr_t) isr)       & 0xFFFF;
    entry.offset_high = ((uintptr_t) isr >> 16) & 0xFFFF;

    entry.selector    = 0x08;
    entry.zero        = 0x00;
    entry.type_attr   = 0x8E;

    return entry;
}

void idt_set_entry(unsigned interrupt, void (*isr)(void*))
{
    idt[interrupt] = idt_encode_entry(isr);
}

void idt_init(void)
{
    extern void isr_install_handlers(void);

    isr_install_handlers();
    idt_load_descriptor();
}

#include "terminal.h"

void isr_3_handler(void* _)
{
    terminal_write_string("Custom interrupt!\n");
}
