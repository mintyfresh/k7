#ifndef __ARCH_HAL_H__
#define __ARCH_HAL_H__

static inline void hal_enable_interrupts(void)
{
    asm volatile("sti");
}

static inline void hal_disable_interrupts(void)
{
    asm volatile("cli");
}

#endif
