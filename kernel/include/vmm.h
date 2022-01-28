#ifndef __VMM_H__
#define __VMM_H__

#include <stdbool.h>
#include <stddef.h>
#include <system.h>

bool vmm_map(vaddr_t vaddr, paddr_t paddr, unsigned flags);
bool vmm_unmap(vaddr_t vaddr);

bool vmm_get_paddr(vaddr_t vaddr, paddr_t* paddr);

void vmm_init(void);

#endif
