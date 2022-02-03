#include <ring_buffer.h>

#include <stdlib.h>

struct RingBuffer
{
    size_t capacity;
    size_t start;
    size_t end;
    uintptr_t* buffer;
};

ring_buffer_t* ring_buffer_create(size_t capacity)
{
    if (capacity == 0)
    {
        return NULL;
    }

    ring_buffer_t* buffer = malloc(sizeof(ring_buffer_t));

    if (buffer == NULL)
    {
        return NULL;
    }

    buffer->capacity = capacity;
    buffer->start    = 0;
    buffer->end      = 0;
    buffer->buffer   = malloc(sizeof(uintptr_t) * capacity);

    if (buffer->buffer == NULL)
    {
        free(buffer);

        return NULL;
    }

    return buffer;
}

void ring_buffer_destroy(ring_buffer_t* buffer)
{
    if (buffer == NULL)
    {
        return;
    }

    free(buffer->buffer);
    free(buffer);
}

size_t ring_buffer_capacity(ring_buffer_t* buffer)
{
    return buffer->capacity;
}

size_t ring_buffer_count(ring_buffer_t* buffer)
{
    if (buffer->start <= buffer->end)
    {
        return buffer->end - buffer->start;
    }
    else
    {
        return buffer->capacity - buffer->start + buffer->end;
    }
}

inline bool ring_buffer_full(ring_buffer_t* buffer)
{
    return ring_buffer_count(buffer) == buffer->capacity;
}

inline bool ring_buffer_empty(ring_buffer_t* buffer)
{
    return ring_buffer_count(buffer) == 0;
}

bool ring_buffer_push(ring_buffer_t* buffer, uintptr_t value)
{
    if (ring_buffer_full(buffer))
    {
        return false;
    }

    buffer->buffer[buffer->end++] = value;
    buffer->end %= buffer->capacity;

    return true;
}

bool ring_buffer_push_range(ring_buffer_t* buffer, size_t count, uintptr_t* values)
{
    if (ring_buffer_count(buffer) + count > buffer->capacity)
    {
        return false;
    }

    for (size_t i = 0; i < count; i++)
    {
        ring_buffer_push(buffer, values[i]);
    }

    return true;
}

bool ring_buffer_pop_head(ring_buffer_t* buffer, uintptr_t* value)
{
    if (ring_buffer_empty(buffer))
    {
        return false;
    }
    
    *value = buffer->buffer[buffer->start++];
    buffer->start %= buffer->capacity;

    return true;
}

bool ring_buffer_pop_head_range(ring_buffer_t* buffer, size_t count, uintptr_t* values)
{
    if (ring_buffer_count(buffer) < count)
    {
        return false;
    }

    for (size_t i = 0; i < count; i++)
    {
        ring_buffer_pop_head(buffer, &values[i]);
    }

    return true;
}

bool ring_buffer_pop_tail(ring_buffer_t* buffer, uintptr_t* value)
{
    if (ring_buffer_empty(buffer))
    {
        return false;
    }

    if (buffer->end == 0)
    {
        buffer->end = buffer->capacity;
    }

    *value = buffer->buffer[--buffer->end];

    return true;
}

bool ring_buffer_pop_tail_range(ring_buffer_t* buffer, size_t count, uintptr_t* values)
{
    if (ring_buffer_count(buffer) < count)
    {
        return false;
    }

    for (size_t i = 0; i < count; i++)
    {
        ring_buffer_pop_tail(buffer, &values[i]);
    }

    return true;
}

void ring_buffer_clear(ring_buffer_t* buffer)
{
    buffer->start = 0;
    buffer->end   = 0;
}
