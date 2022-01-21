#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>

typedef uint64_t scan_code_t;

void keyboard_key_event(scan_code_t scan_code);

void keyboard_init(void);

#endif
