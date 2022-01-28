#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t bitmap_index_t;

struct Bitmap;

struct Bitmap* bitmap_create(uint32_t count, bool initially_reserved);
void bitmap_destroy(struct Bitmap* bitmap);

uint32_t bitmap_total_count(struct Bitmap* bitmap);
uint32_t bitmap_available_count(struct Bitmap* bitmap);

void bitmap_reserve(struct Bitmap* bitmap, bitmap_index_t index);
void bitmap_reserve_range(struct Bitmap* bitmap, bitmap_index_t index, uint32_t count);

void bitmap_release(struct Bitmap* bitmap, bitmap_index_t index);
void bitmap_release_range(struct Bitmap* bitmap, bitmap_index_t index, uint32_t count);

bool bitmap_is_available(struct Bitmap* bitmap, bitmap_index_t index);
bool bitmap_is_available_range(struct Bitmap* bitmap, bitmap_index_t index, uint32_t count);

void bitmap_set_all(struct Bitmap* bitmap, bool reserved);

bool bitmap_first_available(struct Bitmap* bitmap, bitmap_index_t* index);
bool bitmap_first_available_range(struct Bitmap* bitmap, uint32_t count, bitmap_index_t* index);

#endif
