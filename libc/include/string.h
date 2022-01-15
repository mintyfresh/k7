#ifndef __LIBC_STRING_H__
#define __LIBC_STRING_H__

#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);

char* strrev(char* str);

size_t strlen(const char* str);

#endif
