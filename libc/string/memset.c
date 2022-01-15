#include <string.h>

void* memset(void* dest, int c, size_t n)
{
    char* d = dest;
    while (n--)
        *d++ = c;
    return dest;
}
