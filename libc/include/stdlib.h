#ifndef __LIBC_STDLIB_H__
#define __LIBC_STDLIB_H__

#include <stdint.h>

char* itoa(int value, char* str, int base);

void* sbrk(intptr_t increment);

#endif
