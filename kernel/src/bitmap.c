#include <bitmap.h>
#include <extmath.h>
#include <stdlib.h>
#include <string.h>

struct Bitmap
{
    uint32_t* bitmap;
    uint32_t length;
    uint32_t count;
    uint32_t free_count;
    uint32_t next_free;
};

#define ELEMENTS_PER_BLOCK 32

static inline bitmap_index_t block_bit_to_index(uint32_t block, uint32_t bit)
{
    return block * ELEMENTS_PER_BLOCK + bit;
}

static inline uint32_t index_to_block(bitmap_index_t index)
{
    return index / ELEMENTS_PER_BLOCK;
}

static inline uint32_t index_to_bit(bitmap_index_t index)
{
    return index % ELEMENTS_PER_BLOCK;
}

#define reserve_bit(bitmap, bit) (set_bit(bitmap, bit))
#define release_bit(bitmap, bit) (clear_bit(bitmap, bit))

#define is_reserved(bitmap, bit)  (test_bit(bitmap, bit))
#define is_available(bitmap, bit) (!test_bit(bitmap, bit))

struct Bitmap* bitmap_create(uint32_t count, bool initially_reserved)
{
    uint32_t length = count / ELEMENTS_PER_BLOCK;

    struct Bitmap* bitmap = malloc(sizeof(struct Bitmap));

    if (bitmap == NULL)
    {
        return NULL;
    }

    bitmap->bitmap = malloc(length * sizeof(uint32_t));

    if (bitmap->bitmap == NULL)
    {
        free(bitmap);

        return NULL;
    }

    bitmap->length = length;
    bitmap->count  = count;

    bitmap_set_all(bitmap, initially_reserved);

    return bitmap;
}

void bitmap_destroy(struct Bitmap* bitmap)
{
    free(bitmap->bitmap);
    free(bitmap);
}

uint32_t bitmap_total_count(struct Bitmap* bitmap)
{
    return bitmap->count;
}

uint32_t bitmap_available_count(struct Bitmap* bitmap)
{
    return bitmap->free_count;
}

inline void bitmap_reserve(struct Bitmap* bitmap, bitmap_index_t index)
{
    bitmap_reserve_range(bitmap, index, 1);
}

void bitmap_reserve_range(struct Bitmap* bitmap, bitmap_index_t index, uint32_t count)
{
    for (uint32_t i = index; i < index + count; i++)
    {
        uint32_t block = index_to_block(i);
        uint32_t bit   = index_to_bit(i);

        reserve_bit(bitmap->bitmap[block], bit);
    }

    bitmap->free_count -= count;
    bitmap->next_free   = index + count;
}

inline void bitmap_release(struct Bitmap* bitmap, bitmap_index_t index)
{
    bitmap_release_range(bitmap, index, 1);
}

void bitmap_release_range(struct Bitmap* bitmap, bitmap_index_t index, uint32_t count)
{
    for (uint32_t i = index; i < index + count; i++)
    {
        uint32_t block = index_to_block(i);
        uint32_t bit   = index_to_bit(i);

        release_bit(bitmap->bitmap[block], bit);
    }

    bitmap->free_count += count;
    bitmap->next_free   = index;
}

inline bool bitmap_is_available(struct Bitmap* bitmap, bitmap_index_t index)
{
    uint32_t block = index_to_block(index);
    uint32_t bit   = index_to_bit(index);

    return is_available(bitmap->bitmap[block], bit);
}

bool bitmap_is_available_range(struct Bitmap* bitmap, bitmap_index_t index, uint32_t count)
{
    uint32_t found = 0;
    uint32_t block = index_to_block(index);
    uint32_t bit   = index_to_bit(index);

    for (; block < bitmap->length; block++)
    {
        if (bitmap->bitmap[block] == 0)
        {
            return false;
        }

        for (; bit < ELEMENTS_PER_BLOCK; bit++)
        {
            if (is_reserved(bitmap->bitmap[block], bit))
            {
                return false;
            }

            found++;

            if (found == count)
            {
                return true;
            }
        }

        bit = 0;
    }

    return found == count;
}

void bitmap_set_all(struct Bitmap* bitmap, bool reserved)
{
    for (uint32_t i = 0; i < bitmap->length; i++)
    {
        bitmap->bitmap[i] = reserved ? 0xFFFFFFFF : 0;
    }

    if (reserved)
    {
        bitmap->free_count = 0;
        bitmap->next_free  = 0;
    }
    else
    {
        bitmap->free_count = bitmap->count;
        bitmap->next_free  = bitmap->count;
    }
}

bool bitmap_first_available(struct Bitmap* bitmap, bitmap_index_t* index)
{
    if (bitmap->free_count == 0)
    {
        return false;
    }

    uint32_t block = index_to_block(bitmap->next_free);
    uint32_t bit   = index_to_bit(bitmap->next_free);

    for (; block < bitmap->length; block++)
    {
        if (bitmap->bitmap[block] == 0)
        {
            continue;
        }

        for (; bit < ELEMENTS_PER_BLOCK; bit++)
        {
            if (is_reserved(bitmap->bitmap[block], bit))
            {
                continue;
            }

            uint32_t found_index = block_bit_to_index(block, bit);

            *index = found_index;
            bitmap_reserve(bitmap, found_index);

            return true;
        }

        bit = 0;
    }

    return false;
}

bool bitmap_first_available_range(struct Bitmap* bitmap, uint32_t count, bitmap_index_t* index)
{
    if (bitmap->free_count < count)
    {
        return false;
    }

    uint32_t block = index_to_block(bitmap->next_free);
    uint32_t bit   = index_to_bit(bitmap->next_free);

    for (; block < bitmap->length; block++)
    {
        if (bitmap->bitmap[block] == 0)
        {
            continue;
        }

        for (; bit < ELEMENTS_PER_BLOCK; bit++)
        {
            if (is_reserved(bitmap->bitmap[block], bit))
            {
                continue;
            }

            bitmap_index_t found_index = block_bit_to_index(block, bit);

            if (bitmap_is_available_range(bitmap, found_index, count))
            {
                *index = found_index;
                bitmap_reserve_range(bitmap, found_index, count);

                return true;
            }
        }

        bit = 0;
    }

    return false;
}
