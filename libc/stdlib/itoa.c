#include "stdlib.h"
#include "string.h"

char* itoa(int value, char* str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr = str;
    int sign;
    unsigned int v;

    if (base < 2 || base > 35)
    {
        *wstr = '\0';
        return str;
    }

    sign = (value < 0 && base == 10);
    if (sign)
        v = -value;
    else
        v = (unsigned int)value;

    do
    {
        *wstr++ = num[v % base];
        v /= base;
    } while (v);

    if (sign)
        *wstr++ = '-';

    *wstr = '\0';

    strrev(str);

    return str;
}
