#include <bitmap.h>
#include <extmath.h>
#include <logger.h>
#include <multiboot.h>
#include <ring_buffer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

static struct Bitmap* bitmap;
static ring_buffer_t* page_cache;

static inline paddr_t index_to_paddr(bitmap_index_t index)
{
    return index * PAGE_SIZE;
}

static inline bitmap_index_t paddr_to_index(paddr_t paddr)
{
    return paddr / PAGE_SIZE;
}

static void pmm_init_bitmap(uint32_t total_memory)
{
    bitmap = bitmap_create(total_memory / PAGE_SIZE, true);
}

static void pmm_init_page_cache(void)
{
    page_cache = ring_buffer_create(50);
}

static void pmm_load_memory_map(multiboot_info_t* mbi)
{
    vaddr_t mmap_addr = paddr_to_vaddr(mbi->mmap_addr);
    vaddr_t mmap_end  = mmap_addr + mbi->mmap_length;

    while (mmap_addr < mmap_end)
    {
        multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mmap_addr;

        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            if (mmap->addr > UINT32_MAX || mmap->addr + mmap->len > UINT32_MAX)
            {
                log_error("Memory map entry exceeds 32-bit address space.");
                break;
            }

            if ((paddr_t)(mmap->addr + mmap->len) > bitmap_total_count(bitmap) * PAGE_SIZE)
            {
                log_error("Memory map entry exceeds bitmap size.");
                break;
            }

            bitmap_index_t start = paddr_to_index(mmap->addr);
            uint32_t       count = (uint32_t)(mmap->len / PAGE_SIZE);

            // Map any pages that have spillover.
            if (mmap->len % PAGE_SIZE > 0)
            {
                count++;
            }

            // Mark all pages in range as free.
            bitmap_release_range(bitmap, start, count);
        }

        mmap_addr += mmap->size + sizeof(mmap->size);
    }
}

extern void _kernel_start;
extern void _kernel_mapped_end;

static void pmm_reserve_kernel_memory(void)
{
    paddr_t kernel_start = vaddr_to_paddr((vaddr_t) &_kernel_start);
    paddr_t kernel_end   = vaddr_to_paddr((vaddr_t) &_kernel_mapped_end);

    bitmap_index_t start = paddr_to_index(kernel_start);
    uint32_t       count = (kernel_end - kernel_start) / PAGE_SIZE;

    if (kernel_end % PAGE_SIZE > 0)
    {
        count++;
    }

    bitmap_reserve_range(bitmap, start, count);
}

bool pmm_allocate_page(paddr_t* paddr)
{
    if (ring_buffer_pop_tail(page_cache, paddr))
    {
        return true;
    }

    bitmap_index_t index = 0;

    if (bitmap_first_available(bitmap, &index))
    {
        bitmap_reserve(bitmap, index);

        *paddr = index_to_paddr(index);

        return true;
    }

    return false;
}

bool pmm_allocate_page_range(paddr_t* paddr, uint32_t count)
{
    bitmap_index_t index = 0;

    if (bitmap_first_available_range(bitmap, count, &index))
    {
        bitmap_reserve_range(bitmap, index, count);

        *paddr = index_to_paddr(index);

        return true;
    }

    return false;
}

bool pmm_release_page(paddr_t paddr)
{
    if (ring_buffer_push(page_cache, paddr))
    {
        return true;
    }

    bitmap_release(bitmap, paddr_to_index(paddr));

    return true;
}

bool pmm_release_range(paddr_t paddr, uint32_t count)
{
    bitmap_release_range(bitmap, paddr_to_index(paddr), count);

    return true;
}

uint32_t pmm_free_pages_count(void)
{
    return bitmap_available_count(bitmap) + ring_buffer_count(page_cache);
}

uint32_t pmm_total_pages_count(void)
{
    return bitmap_total_count(bitmap);
}

void pmm_init(multiboot_info_t* mbi)
{
    // TODO: Figure out what to do with lower memory.
    uint32_t total_memory = mbi->mem_upper * 1024 + (1024 * 1024);

    pmm_init_bitmap(total_memory);
    pmm_init_page_cache();

    pmm_load_memory_map(mbi);
    pmm_reserve_kernel_memory();
}

#define MEGABYTES(x) ((x) / (1024 * 1024))

void pmm_display_stats(void)
{
    log_debug("Total memory: %d MiB", MEGABYTES(pmm_total_pages_count() * PAGE_SIZE));
    log_debug("Total pages: %d", pmm_total_pages_count());

    log_debug("Available memory: %d MiB", MEGABYTES(pmm_free_pages_count() * PAGE_SIZE));
    log_debug("Available pages: %d", pmm_free_pages_count());
}
