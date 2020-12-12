#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "image_io.h"

static const uint16_t BMP_MAGIC_NUMBER = 0x4D42;

struct __attribute__((packed)) bmp_file_header {
    uint16_t type;
    uint32_t file_size; 
    uint32_t reserved;
    uint32_t data_start_addr;

    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes_num;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;
    uint32_t colors_num;
    uint32_t important_colors_num;
};

struct __attribute__((packed)) bmp_pixel_bgr {
    uint8_t b, g, r;
};

image_load_status image_read_from_bmp(char* bmp_file_name, image** dst) {
    FILE* file = fopen(bmp_file_name, "rb");

    if (file == NULL) {
        if (errno == ENOENT)
            return IMAGE_LOAD_FILE_NOT_EXIST;
        return IMAGE_LOAD_READ_FAIL;
    }

    struct bmp_file_header header;
    /*size_t have_read = */fread(&header, sizeof(header), 1, file);

    if (ferror(file))
        return IMAGE_LOAD_READ_FAIL;

    if (header.type != BMP_MAGIC_NUMBER)
        return IMAGE_LOAD_TYPE_UNSUPPORTED;

    if (header.bits_per_pixel != 24)
        return IMAGE_LOAD_BPP_NOT_SUPPORTED;

    if (header.compression_method != 0)
        return IMAGE_LOAD_COMPRESSION_NOT_SUPPORTED;

    uint32_t pixel_data_start = header.data_start_addr;

    uint32_t width_bytes = header.width*3;

    uint32_t width_bytes_padding = (4 - width_bytes%4)%4;

    fseek(file, pixel_data_start, SEEK_SET);

    image* result = image_create(header.width, header.height);

    struct bmp_pixel_bgr* bmp_pixel_data = (struct bmp_pixel_bgr*)malloc(sizeof(struct bmp_pixel_bgr) * width_bytes);

    for (uint32_t pixel_row_index = 0; pixel_row_index < header.height; pixel_row_index++) {
        fread(bmp_pixel_data, width_bytes, 1, file);

        for (uint32_t j = 0; j < image_get_width(result); j++) {
//            struct bmp_pixel_bgr bmp_specific_pixel = bmp_pixel_data[j];

            pixel generic_pixel = (pixel) {
                bmp_pixel_data[j].r,
                bmp_pixel_data[j].g,
                bmp_pixel_data[j].b
            };

            point dest_point = (point) {
                j,
                image_get_height(result) - pixel_row_index - 1
            };

            image_set_pixel(result, dest_point, generic_pixel);
        }

        fseek(file, width_bytes_padding, SEEK_CUR);
    }

    free(bmp_pixel_data);
    fclose(file);

    *dst = result;

    return IMAGE_LOAD_OK;
}

image_save_status image_save_to_bmp(char* bmp_file_name, image* src) {
    FILE* file = fopen(bmp_file_name, "wb");

    if (file == NULL) {
        if (errno == EACCES)
            return IMAGE_SAVE_NO_ACCESS;
        return IMAGE_SAVE_OPEN_FAIL;
    }

    uint32_t width_bytes = image_get_width(src)*3;

    uint32_t width_bytes_padded = width_bytes % 4 == 0 ? width_bytes : width_bytes - width_bytes%4 + 4;

    struct bmp_file_header header;

    uint32_t file_size = sizeof(header) + width_bytes_padded * image_get_height(src);

    header.type = BMP_MAGIC_NUMBER;
    header.file_size = file_size;
    header.reserved = 0;

    header.data_start_addr = sizeof(header);
    header.header_size = 40;
    header.width = image_get_width(src);
    header.height = image_get_height(src);
    header.color_planes_num = 1;
    header.bits_per_pixel = 24;
    header.compression_method = 0;
    header.image_size = 0;
    header.x_pixels_per_meter = 1;
    header.y_pixels_per_meter = 1;
    header.colors_num = 256*256*256;
    header.important_colors_num = 0;

    fwrite(&header, sizeof(header), 1, file);

    if (ferror(file))
        return IMAGE_SAVE_WRITE_FAIL;

    struct bmp_pixel_bgr* pixel_data = (struct bmp_pixel_bgr*)malloc(width_bytes_padded);

    for (uint32_t row = 0; row < image_get_height(src); row++) {

        for (uint32_t pixelIdx = 0; pixelIdx < image_get_width(src); pixelIdx++) {
            pixel px = image_get_pixel(src, (point) { pixelIdx, image_get_height(src) - row - 1 });
            pixel_data[pixelIdx] = (struct bmp_pixel_bgr) {
                px.b, px.g, px.r
            };
        }
        fwrite(pixel_data, width_bytes_padded, 1, file);

        if (ferror(file))
            return IMAGE_SAVE_WRITE_FAIL;
    }

    free(pixel_data);

    fclose(file);

    return IMAGE_SAVE_OK;
}