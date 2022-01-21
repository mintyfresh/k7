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

void idt_set_entry(uint8_t interrupt, void* isr)
{
    idt[interrupt] = idt_encode_entry(isr);
}

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

void idt_init(void)
{
    idt_set_entry(0, isr0);
    idt_set_entry(1, isr1);
    idt_set_entry(2, isr2);
    idt_set_entry(3, isr3);
    idt_set_entry(4, isr4);
    idt_set_entry(5, isr5);
    idt_set_entry(6, isr6);
    idt_set_entry(7, isr7);
    idt_set_entry(8, isr8);
    idt_set_entry(9, isr9);
    idt_set_entry(10, isr10);
    idt_set_entry(11, isr11);
    idt_set_entry(12, isr12);
    idt_set_entry(13, isr13);
    idt_set_entry(14, isr14);
    idt_set_entry(15, isr15);
    idt_set_entry(16, isr16);
    idt_set_entry(17, isr17);
    idt_set_entry(18, isr18);
    idt_set_entry(19, isr19);
    idt_set_entry(20, isr20);
    idt_set_entry(21, isr21);
    idt_set_entry(22, isr22);
    idt_set_entry(23, isr23);
    idt_set_entry(24, isr24);
    idt_set_entry(25, isr25);
    idt_set_entry(26, isr26);
    idt_set_entry(27, isr27);
    idt_set_entry(28, isr28);
    idt_set_entry(29, isr29);
    idt_set_entry(30, isr30);
    idt_set_entry(31, isr31);

    idt_load_descriptor();
}

void isr_handler(struct MachineState* state)
{
    printf("Exception: %d\n", state->interrupt);
    abort();
}
