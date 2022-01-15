#include "string.h"

char* strrev(char* str)
{
    size_t length = strlen(str);

    for (size_t i = 0; i < length / 2; i++)
    {
        char tmp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = tmp;
    }

    return str;
}
