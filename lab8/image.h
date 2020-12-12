#ifndef _IMAGE_H
#define _IMAGE_H

#include <stdint.h>
#include <stdbool.h>

struct image;

typedef struct image image;

typedef struct pixel {
    uint8_t r, g, b;
} pixel;

typedef struct point {
    uint32_t x, y;
} point;

image* image_create(uint32_t width, uint32_t height);

image* image_copy(image* image);

void image_destroy(image* image);

uint32_t image_get_width(image* image);

uint32_t image_get_height(image* image);

void image_swap_size(image* image);

pixel image_get_pixel(image* image, point point);

pixel image_get_pixel_or_default(image* image, point point, pixel default_pixel);

bool image_set_pixel(image* image, point point, pixel pixel);

bool image_check_bounds(image* image, point point);

pixel image_get_pixel_by_idx(image* image, size_t idx);

void image_set_pixel_by_idx(image* image, size_t idx, pixel p);

#endif