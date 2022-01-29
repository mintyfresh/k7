#ifndef __EXTMATH_H__
#define __EXTMATH_H__

#include <stdint.h>

#define test_bit(value, bit)  ((value) &   (1 << (bit)))
#define set_bit(value, bit)   ((value) |=  (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

#define test_flag(value, flag)  ((value) &   (flag))
#define set_flag(value, flag)   ((value) |=  (flag))
#define clear_flag(value, flag) ((value) &= ~(flag))

#endif
