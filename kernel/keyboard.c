#include <keyboard.h>
#include <stdio.h>

void keyboard_key_event(scan_code_t scan_code)
{
    printf("Scan code: %x%x\n", (uint32_t)(scan_code >> 32), (uint32_t) scan_code);
}
