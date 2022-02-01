#ifndef __PMM_H__
#define __PMM_H__

#include <multiboot.h>
#include <stdbool.h>
#include <stdint.h>
#include <system.h>

bool pmm_allocate_page(paddr_t* paddr);
bool pmm_allocate_page_range(paddr_t* paddr, uint32_t count);

uint32_t pmm_free_pages_count(void);
uint32_t pmm_total_pages_count(void);

void pmm_init(multiboot_info_t *mbi);
void pmm_display_stats(void);

#endif
