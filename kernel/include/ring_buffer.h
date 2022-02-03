#ifndef __KERNEL_RING_BUFFER_H__
#define __KERNEL_RING_BUFFER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct RingBuffer ring_buffer_t;

ring_buffer_t* ring_buffer_create(size_t capacity);
void ring_buffer_destroy(ring_buffer_t* buffer);

size_t ring_buffer_capacity(ring_buffer_t* buffer);
size_t ring_buffer_count(ring_buffer_t* buffer);

bool ring_buffer_full(ring_buffer_t* buffer);
bool ring_buffer_empty(ring_buffer_t* buffer);

bool ring_buffer_push(ring_buffer_t* buffer, uintptr_t value);
bool ring_buffer_push_range(ring_buffer_t* buffer, size_t count, uintptr_t* values);

bool ring_buffer_pop_head(ring_buffer_t* buffer, uintptr_t* value);
bool ring_buffer_pop_head_range(ring_buffer_t* buffer, size_t count, uintptr_t* values);

bool ring_buffer_pop_tail(ring_buffer_t* buffer, uintptr_t* value);
bool ring_buffer_pop_tail_range(ring_buffer_t* buffer, size_t count, uintptr_t* values);

void ring_buffer_clear(ring_buffer_t* buffer);

#endif
