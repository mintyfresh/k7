#include <keyboard.h>
#include <logger.h>

void keyboard_key_event(scan_code_t scan_code)
{
    log_debug("Scan code: %x%x\n", (uint32_t)(scan_code >> 32), (uint32_t) scan_code);
}
