#include <bitmap.h>
#include <extmath.h>
#include <stdlib.h>
#include <string.h>

struct Bitmap
{
    uint32_t* bitmap;
    uint32_t length;
    uint32_t total_count;
    uint32_t free_count;
    uint32_t first_free;
};

#define BITS_PER_BLOCK 32

static inline bitmap_index_t block_and_bit_to_index(uint32_t block, uint32_t bit)
{
    return block * BITS_PER_BLOCK + bit;
}

static inline uint32_t index_to_block(bitmap_index_t index)
{
    return index / BITS_PER_BLOCK;
}

static inline uint32_t index_to_bit(bitmap_index_t index)
{
    return index % BITS_PER_BLOCK;
}

#define reserve_bit(bitmap, bit) (set_bit(bitmap, bit))
#define release_bit(bitmap, bit) (clear_bit(bitmap, bit))

#define is_reserved(bitmap, bit)  (test_bit(bitmap, bit))
#define is_available(bitmap, bit) (!test_bit(bitmap, bit))

struct Bitmap* bitmap_create(uint32_t count, bool initially_reserved)
{
    uint32_t length = count / BITS_PER_BLOCK;

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

    bitmap->length      = length;
    bitmap->total_count = count;

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
    return bitmap->total_count;
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

    if (index < bitmap->first_free && bitmap->first_free < index + count)
    {
        bitmap->first_free = index + count;
    }
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

    if (index < bitmap->first_free)
    {
        bitmap->first_free = index;
    }
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

    for (; block < bitmap->length; block++, bit = 0)
    {
        if (bitmap->bitmap[block] == 0)
        {
            return false;
        }

        if (bitmap->bitmap[block] == 0xFFFFFFFF)
        {
            found += BITS_PER_BLOCK - bit;

            if (found >= count)
            {
                return true;
            }

            continue;
        }

        for (; bit < BITS_PER_BLOCK; bit++)
        {
            if (is_reserved(bitmap->bitmap[block], bit))
            {
                return false;
            }

            found++;

            if (found >= count)
            {
                return true;
            }
        }
    }

    return found >= count;
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
        bitmap->first_free = 0;
    }
    else
    {
        bitmap->free_count = bitmap->total_count;
        bitmap->first_free = bitmap->total_count;
    }
}

bool bitmap_first_available(struct Bitmap* bitmap, bitmap_index_t* index)
{
    if (bitmap->free_count == 0)
    {
        return false;
    }

    uint32_t block = index_to_block(bitmap->first_free);
    uint32_t bit   = index_to_bit(bitmap->first_free);

    for (; block < bitmap->length; block++, bit = 0)
    {
        if (bitmap->bitmap[block] == 0)
        {
            continue;
        }

        for (; bit < BITS_PER_BLOCK; bit++)
        {
            if (is_reserved(bitmap->bitmap[block], bit))
            {
                continue;
            }

            uint32_t found_index = block_and_bit_to_index(block, bit);

            *index = found_index;

            return true;
        }
    }

    return false;
}

bool bitmap_first_available_range(struct Bitmap* bitmap, uint32_t count, bitmap_index_t* index)
{
    if (bitmap->free_count < count)
    {
        return false;
    }

    uint32_t block = index_to_block(bitmap->first_free);
    uint32_t bit   = index_to_bit(bitmap->first_free);

    for (; block < bitmap->length; block++, bit = 0)
    {
        if (bitmap->bitmap[block] == 0)
        {
            continue;
        }

        for (; bit < BITS_PER_BLOCK; bit++)
        {
            if (is_reserved(bitmap->bitmap[block], bit))
            {
                continue;
            }

            bitmap_index_t found_index = block_and_bit_to_index(block, bit);

            if (bitmap_is_available_range(bitmap, found_index, count))
            {
                *index = found_index;

                return true;
            }
        }
    }

    return false;
}
