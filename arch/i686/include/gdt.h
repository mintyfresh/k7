#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

#define GDT_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define GDT_PRES(x)      ((x) << 0x07) // Present
#define GDT_SAVL(x)      ((x) << 0x0C) // Available for system use
#define GDT_LONG(x)      ((x) << 0x0D) // Long mode
#define GDT_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define GDT_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define GDT_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define GDT_DATA_RD        0x00 // Read-Only
#define GDT_DATA_RDA       0x01 // Read-Only, accessed
#define GDT_DATA_RDWR      0x02 // Read/Write
#define GDT_DATA_RDWRA     0x03 // Read/Write, accessed
#define GDT_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define GDT_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define GDT_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define GDT_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define GDT_CODE_EX        0x08 // Execute-Only
#define GDT_CODE_EXA       0x09 // Execute-Only, accessed
#define GDT_CODE_EXRD      0x0A // Execute/Read
#define GDT_CODE_EXRDA     0x0B // Execute/Read, accessed
#define GDT_CODE_EXC       0x0C // Execute-Only, conforming
#define GDT_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define GDT_CODE_EXRDC     0x0E // Execute/Read, conforming
#define GDT_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

void gdt_init(void);

#endif
