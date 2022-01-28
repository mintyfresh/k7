#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFF000

#define KERNEL_BASE 0xC0000000

static inline paddr_t vaddr_to_paddr(vaddr_t vaddr)
{
    return vaddr - KERNEL_BASE;
}

static inline vaddr_t paddr_to_vaddr(paddr_t paddr)
{
    return paddr + KERNEL_BASE;
}

static inline void hal_enable_interrupts(void)
{
    asm volatile("sti");
}

static inline void hal_disable_interrupts(void)
{
    asm volatile("cli");
}

static inline void get_msr(uint32_t msr, uint32_t* low, uint32_t* high)
{
    __asm__ __volatile__("rdmsr" : "=a"(*low), "=d"(*high) : "c"(msr));
}

static inline void set_msr(uint32_t msr, uint32_t low, uint32_t high)
{
    __asm__ __volatile__("wrmsr" : : "a"(low), "d"(high), "c"(msr));
}

#endif
