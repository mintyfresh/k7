#ifndef __TERMINAL_H__
#define __TERMINAL_H__

void terminal_clear(void);
void terminal_write_char(char ch);
void terminal_write_string(const char *str);
void terminal_init(void);

#endif
