#ifndef __LIBC_STDLIB_H__
#define __LIBC_STDLIB_H__

#include <stddef.h>
#include <stdint.h>

__attribute__((__noreturn__))
void abort(void);

char* itoa(int value, char* str, int base);

void* sbrk(intptr_t increment);

void* malloc(size_t size);
void free(void* ptr);

#endif
