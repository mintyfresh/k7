#include <stdint.h>

#include "gdt.h"

#define GDT_NULL        0
#define GDT_KERNEL_CODE GDT_DESCTYPE(1) | GDT_PRES(1) | GDT_SAVL(0) | \
                        GDT_LONG(0)     | GDT_SIZE(1) | GDT_GRAN(1) | \
                        GDT_PRIV(0)     | GDT_CODE_EXRD
#define GDT_KERNEL_DATA GDT_DESCTYPE(1) | GDT_PRES(1) | GDT_SAVL(0) | \
                        GDT_LONG(0)     | GDT_SIZE(1) | GDT_GRAN(1) | \
                        GDT_PRIV(0)     | GDT_DATA_RDWR
#define GDT_USER_CODE   GDT_DESCTYPE(1) | GDT_PRES(1) | GDT_SAVL(0) | \
                        GDT_LONG(0)     | GDT_SIZE(1) | GDT_GRAN(1) | \
                        GDT_PRIV(3)     | GDT_CODE_EXRD
#define GDT_USER_DATA   GDT_DESCTYPE(1) | GDT_PRES(1) | GDT_SAVL(0) | \
                        GDT_LONG(0)     | GDT_SIZE(1) | GDT_GRAN(1) | \
                        GDT_PRIV(3)     | GDT_DATA_RDWR

#pragma pack(push, 1)
struct GDTEntry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high;
};

struct GDTDescriptor
{
    uint16_t size;
    uint32_t offset;
};
#pragma pack(pop)

struct GDTDescriptor gdtr;
struct GDTEntry gdt[5];

static struct GDTEntry gdt_encode_entry(uint32_t base, uint32_t offset, uint16_t type)
{
    struct GDTEntry entry;

    entry.base_low    = (base)       & 0xFFFF;
    entry.base_middle = (base >> 16) & 0x00FF;
    entry.base_high   = (base >> 24) & 0x00FF;

    entry.limit_low  = (offset)       & 0xFFFF;
    entry.limit_high = (offset >> 16) & 0x000F;

    entry.flags  = (type >> 12) & 0x0F;
    entry.access = (type)       & 0xFF;

    return entry;
}

static void gdt_load_descriptor(void)
{
    gdtr.size   = sizeof(gdt);
    gdtr.offset = (uint32_t) gdt;

    asm volatile("lgdt (gdtr)");
}

static void gdt_flush(void)
{
    asm volatile
    (
        "ljmp $0x08, $.gdt_flush_i686\n"
    ".gdt_flush_i686:\n"
        "mov $0x10, %ax\n"
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
    );
}

void gdt_init(void)
{
    gdt[0] = gdt_encode_entry(0, 0, GDT_NULL);
    gdt[1] = gdt_encode_entry(0, 0xFFFFFFFF, GDT_KERNEL_CODE);
    gdt[2] = gdt_encode_entry(0, 0xFFFFFFFF, GDT_KERNEL_DATA);
    gdt[3] = gdt_encode_entry(0, 0xBFFFFFFF, GDT_USER_CODE);
    gdt[4] = gdt_encode_entry(0, 0xBFFFFFFF, GDT_USER_DATA);

    gdt_load_descriptor();
    gdt_flush();
}
