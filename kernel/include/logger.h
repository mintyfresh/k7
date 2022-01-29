#ifndef __KERNEL_LOGGER_H__
#define __KERNEL_LOGGER_H__

#include <stdarg.h>
#include <stdio.h>
#include <terminal.h>

#define log_debug(fmt, ...) \
    print_log("DEBUG", __func__, fmt, ##__VA_ARGS__)

#define log_info(fmt, ...) \
    print_log(" INFO", __func__, fmt, ##__VA_ARGS__)

#define log_warn(fmt, ...) \
    print_log(" WARN", __func__, fmt, ##__VA_ARGS__)

#define log_error(fmt, ...) \
    print_colour_log(TERMINAL_RED, TERMINAL_WHITE, "ERROR", __func__, fmt, ##__VA_ARGS__)

static inline void print_log(const char* level, const char* func, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    printf("[%s] %s() ", level, func);
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
}

static inline void print_colour_log(enum TerminalColour foreground, enum TerminalColour background, const char* level, const char* func, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    enum TerminalColour og_foreground, og_background;
    terminal_get_colour(&og_foreground, &og_background);

    terminal_set_colour(foreground, background);

    printf("[%s] %s() ", level, func);
    vprintf(fmt, args);
    printf("\n");

    terminal_set_colour(og_foreground, og_background);

    va_end(args);
}

#endif
