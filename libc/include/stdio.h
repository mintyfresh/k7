#ifndef __LIBC_STDIO_H__
#define __LIBC_STDIO_H__

#include <stdarg.h>

int printf(const char *format, ...);
int vprintf(const char *format, va_list args);

#endif
