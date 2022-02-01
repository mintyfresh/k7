#include <extmath.h>
#include <logger.h>
#include <pmm.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vmm.h>

typedef uint32_t page_directory_entry_t;
typedef uint32_t page_table_entry_t;

typedef struct
{
    page_directory_entry_t entries[1024];
} page_directory_t;

typedef struct
{
    page_table_entry_t entries[1024];
} page_table_t;

#define PAGE_DIRECTORY_MAPPING 0xFFFFF000
#define PAGE_TABLE_ADDRESS     0xFFC00000

#define SMALL_PAGE_MASK 0xFFFFF000
#define LARGE_PAGE_MASK 0xFFC00000

#define PAGE_DIRECTORY_PRESENT        (1 << 0)
#define PAGE_DIRECTORY_READ_WRITE     (1 << 1)
#define PAGE_DIRECTORY_USER           (1 << 2)
#define PAGE_DIRECTORY_WRITE_THROUGH  (1 << 3)
#define PAGE_DIRECTORY_CACHE_DISABLED (1 << 4)
#define PAGE_DIRECTORY_ACCESSED       (1 << 5)
#define PAGE_DIRECTORY_DIRTY          (1 << 6)
#define PAGE_DIRECTORY_LARGE          (1 << 7)
#define PAGE_DIRECTORY_GLOBAL         (1 << 8)

#define PAGE_TABLE_PRESENT        (1 << 0)
#define PAGE_TABLE_READ_WRITE     (1 << 1)
#define PAGE_TABLE_USER           (1 << 2)
#define PAGE_TABLE_WRITE_THROUGH  (1 << 3)
#define PAGE_TABLE_CACHE_DISABLED (1 << 4)
#define PAGE_TABLE_ACCESSED       (1 << 5)
#define PAGE_TABLE_DIRTY          (1 << 6)
#define PAGE_TABLE_PAT            (1 << 7)
#define PAGE_TABLE_GLOBAL         (1 << 8)

#define is_present(entry)        ((entry) & PAGE_DIRECTORY_PRESENT)
#define is_read_write(entry)     ((entry) & PAGE_DIRECTORY_READ_WRITE)
#define is_user(entry)           ((entry) & PAGE_DIRECTORY_USER)
#define is_system(entry)         (!is_user(entry))
#define is_write_through(entry)  ((entry) & PAGE_DIRECTORY_WRITE_THROUGH)
#define is_cache_disabled(entry) ((entry) & PAGE_DIRECTORY_CACHE_DISABLED)
#define is_accessed(entry)       ((entry) & PAGE_DIRECTORY_ACCESSED)
#define is_dirty(entry)          ((entry) & PAGE_DIRECTORY_DIRTY)
#define is_large_page(entry)     ((entry) & PAGE_DIRECTORY_LARGE)
#define is_global(entry)         ((entry) & PAGE_DIRECTORY_GLOBAL)

#define small_page_address(entry) ((entry) & SMALL_PAGE_MASK)
#define large_page_address(entry) ((entry) & LARGE_PAGE_MASK)

static inline uint32_t page_directory_index(vaddr_t vaddr)
{
    return (vaddr >> 22) & 0x3FF;
}

static inline uint32_t page_table_index(vaddr_t vaddr)
{
    return (vaddr >> 12) & 0x3FF;
}

static inline page_directory_t* get_page_directory(void)
{
    return (page_directory_t*) PAGE_DIRECTORY_MAPPING;
}

static inline page_directory_entry_t* get_page_directory_entry(vaddr_t vaddr)
{
    uint32_t pd_index = page_directory_index(vaddr);

    return &get_page_directory()->entries[pd_index];
}

static inline page_table_t* get_page_table(uint32_t index)
{
    return &((page_table_t*) PAGE_TABLE_ADDRESS)[index];
}

static inline page_table_entry_t* get_page_table_entry(vaddr_t vaddr)
{
    uint32_t pd_index = page_directory_index(vaddr);
    uint32_t pt_index = page_table_index(vaddr);

    return &get_page_table(pd_index)->entries[pt_index];
}

static inline void flush_tlb(void)
{
    asm volatile("mov %%cr3, %%eax; mov %%eax, %%cr3" : : : "eax");
}

static inline void flush_tlb_single(vaddr_t vaddr)
{
    asm volatile("invlpg (%0)" : : "r" (vaddr) : "memory");
}

static bool create_page_table(page_directory_entry_t* pd_entry, unsigned flags)
{
    paddr_t page_table_address = 0;

    if (!pmm_allocate_page(&page_table_address))
    {
        log_error("Failed to allocate page table");

        return false;
    }

    // TODO: Add better support for flags on page table creation.
    *pd_entry = page_table_address | PAGE_DIRECTORY_PRESENT | flags;

    return true;
}

bool vmm_map(vaddr_t vaddr, paddr_t paddr, unsigned flags)
{
    page_directory_entry_t* pd_entry = get_page_directory_entry(vaddr);

    if (!is_present(*pd_entry))
    {
        if (create_page_table(pd_entry, flags))
        {
            uint32_t pd_index = page_directory_index(vaddr);

            flush_tlb_single((vaddr_t) get_page_table(pd_index));
            memset(get_page_table(pd_index), 0, PAGE_SIZE);
        }
        else
        {
            return false;
        }
    }

    page_table_entry_t* pt_entry = get_page_table_entry(vaddr);

    if (is_present(*pt_entry))
    {
        return false;
    }

    *pt_entry = (paddr & PAGE_MASK) | PAGE_TABLE_PRESENT | flags;

    flush_tlb_single(vaddr);

    return true;
}

bool vmm_unmap(vaddr_t vaddr)
{
    page_directory_entry_t* pd_entry = get_page_directory_entry(vaddr);

    if (!is_present(*pd_entry))
    {
        return false;
    }

    if (is_large_page(*pd_entry))
    {
        clear_flag(*pd_entry, PAGE_DIRECTORY_PRESENT);

        flush_tlb_single(vaddr);

        return true;
    }

    page_table_entry_t* pt_entry = get_page_table_entry(vaddr);

    if (!is_present(*pt_entry))
    {
        return false;
    }

    clear_flag(*pt_entry, PAGE_TABLE_PRESENT);

    flush_tlb_single(vaddr);

    return true;
}

inline bool vmm_get_paddr(vaddr_t vaddr, paddr_t* paddr)
{
    page_directory_entry_t* pd_entry = get_page_directory_entry(vaddr);

    if (!is_present(*pd_entry))
    {
        return false;
    }

    if (is_large_page(*pd_entry))
    {
        *paddr = large_page_address(*pd_entry) | (vaddr & ~LARGE_PAGE_MASK);

        return true;
    }

    page_table_entry_t* pt_entry = get_page_table_entry(vaddr);

    if (!is_present(*pt_entry))
    {
        return false;
    }

    *paddr = small_page_address(*pt_entry) | (vaddr & ~SMALL_PAGE_MASK);

    return true;
}

extern void _kernel_start;

static inline void vmm_identity_map_lower_mem(void)
{
    // Unmap the first 4MB of memory.
    if (!vmm_unmap(0x00000000))
    {
        log_error("Failed to unmap 0");
        abort();
    }

    // Identity map the first 1MB of memory.
    for (paddr_t paddr = 0; paddr < 0x100000; paddr += PAGE_SIZE)
    {
        if (!vmm_map((vaddr_t) paddr, paddr, PAGE_TABLE_PRESENT | PAGE_TABLE_READ_WRITE))
        {
            log_error("Failed to map: %x", paddr);
            abort();
        }
    }
}

void vmm_init(void)
{
    vmm_identity_map_lower_mem();
}
