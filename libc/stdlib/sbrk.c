#include <stdlib.h>
#include <stdio.h>

extern void _kernel_end;
extern void _kernel_mapped_end;

void* sbrk(intptr_t increment)
{
    static void* heap_end = &_kernel_end;

    void* new_heap_end = heap_end + increment;

    if (new_heap_end > &_kernel_mapped_end)
    {
        printf("Heap overflow!\n");
        abort();
    }

    void* old_heap_end = heap_end;
    heap_end = new_heap_end;

    return old_heap_end;
}
