#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void)
{
    printf("Kernel Abort!\n");
    for (;;) { asm volatile("hlt"); }

    __builtin_unreachable();
}
