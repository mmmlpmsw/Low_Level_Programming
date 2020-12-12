#include <stdlib.h>
#include <string.h>

#include "image.h"

struct image {
    uint32_t width;

    uint32_t height;

    pixel* data_array;
};

image* image_create(uint32_t width, uint32_t height) {
    image* new_image = malloc(sizeof(image));

    new_image->width = width;
    new_image->height = height;

    size_t pixel_count = width * height;

    size_t data_array_size = sizeof(pixel) * pixel_count;

    pixel* new_data_array = malloc(data_array_size);

    for (size_t i = 0; i < pixel_count; i++)
        new_data_array[i] = (pixel) {0, 0, 0};

    new_image->data_array = new_data_array;

    return new_image;
}

image* image_copy(image* image) {
    if (image == NULL)
        return NULL;

    uint32_t width = image_get_width(image);
    uint32_t height = image_get_height(image);

    size_t image_data_size = sizeof(pixel) * width * height;

    struct image* new_image = image_create(width, height);

    pixel* new_image_data = new_image->data_array;

    pixel* source_image_data = image->data_array;

    memcpy(new_image_data, source_image_data, image_data_size);


    return new_image;
}

void image_destroy(image* image) {
    if (image == NULL)
        return;

    if (image->data_array != NULL) {
        free(image->data_array);
    }

    free(image);
}

uint32_t image_get_width(image* image) {
    if (image == NULL)
        return 0;

    return image->width;
}

uint32_t image_get_height(image* image) {
    if (image == NULL)
        return 0;

    return image->height;
}

pixel image_get_pixel(image* image, point point) {
    return image_get_pixel_or_default(image, point, (pixel) {0, 0, 0});
}

pixel image_get_pixel_or_default(image* image, point point, pixel default_pixel) {
    if (image == NULL)
        return default_pixel;

    if (!image_check_bounds(image, point))
        return default_pixel;

    size_t pixel_index = image_get_width(image) * point.y + point.x;

    return image->data_array[pixel_index];
}

bool image_set_pixel(image* image, point point, pixel pixel) {
    if (image == NULL)
        return false;

    if (!image_check_bounds(image, point))
        return false;

    size_t pixel_index = image_get_width(image) * point.y + point.x;

    image->data_array[pixel_index] = pixel;

    return true;
}

bool image_check_bounds(image* image, point point) {
    if (image == NULL) {
        return false;
    }

    return !(
        point.x < 0 ||
        point.x >= image_get_width(image) ||
        point.y < 0 ||
        point.y >= image_get_height(image)
    );
}

void image_swap_size(image* image) {
    if (image == NULL)
        return;

    uint32_t buffer = image->width;
    image->width = image->height;
    image->height = buffer;
}

pixel image_get_pixel_by_idx(image* image, size_t idx) {
    return image->data_array[idx];
}

void image_set_pixel_by_idx(image* image, size_t idx, pixel p) {
    image->data_array[idx] = p;
}