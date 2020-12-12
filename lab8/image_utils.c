#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "image_utils.h"
#include "sepia.h"

//void image_rotate(image* image, double degrees) {
//    double radians = degrees*M_PI/180;
//    bool size_swap_needed = fmod(fabs(degrees) + 45, 180) > 90;
//    uint32_t width = image_get_width(image);
//    uint32_t height = image_get_height(image);
//    struct image* old_image = image_copy(image);
//    double center_x = (double)width/2.0;
//    double center_y = (double)height/2.0;
//
//    double cosr = cos(-radians);
//    double sinr = sin(-radians);
//
//    double transform_matrix3[] = {
//        cos(-radians), -sin(-radians), - center_x*cos(-radians) + center_y*sin(-radians) + center_x,
//        sin(-radians),  cos(-radians), - center_x*sin(-radians) - center_y*cos(-radians) + center_y,
//        0, 0, 1
//    };
//
//    if (size_swap_needed) {
//        image_swap_size(image);
//        width = image_get_width(image);
//        height = image_get_height(image);
//
//        transform_matrix3[2] = - center_y*cos(-radians) + center_x*sin(-radians) + center_x;
//        transform_matrix3[5] = - center_y*sin(-radians) - center_x*cos(-radians) + center_y;
//    }
//
//    for (uint32_t current_y = 0; current_y < height; current_y++) {
//        for (uint32_t current_x = 0; current_x < width; current_x++) {
//            point dst_point = { current_x, current_y };
//
//            double src_x = dst_point.x;
//            double src_y = dst_point.y;
//
//            src_x += 0.5;
//            src_y += 0.5;
//
//            point src_point = {
//                floor(transform_matrix3[0]*src_x + transform_matrix3[1]*src_y + transform_matrix3[2]),
//                floor(transform_matrix3[3]*src_x + transform_matrix3[4]*src_y + transform_matrix3[5])
//            };
//
//            pixel src_pixel = image_get_pixel(old_image, src_point);
//
//            image_set_pixel(image, dst_point, src_pixel);
//        }
//    }
//
//    image_destroy(old_image);
//}

unsigned char sat( uint64_t x) {
    if (x < 256)
        return x;
    return 255;
}

void sepia_one_pixel(image* old_image, image* image, uint32_t x, uint32_t y) {
    pixel old = image_get_pixel(old_image, (point){x, y});
    pixel new = image_get_pixel(image, (point){x, y});

    new.r = sat((float)old.r * .393f + (float)old.g * .769f + (float)old.b * .189f);
    new.g = sat((float)old.r * .349f + (float)old.g * .686f + (float)old.b * .168f);
    new.b = sat((float)old.r * .272f + (float)old.g * .543f + (float)old.b * .131f);

    image_set_pixel(image, (point){x, y}, new);
}

void sepia_one_pixel_by_idx(image* old_image, image* image, size_t i) {
    pixel old = image_get_pixel_by_idx(old_image, i);
    pixel new = image_get_pixel_by_idx(image, i);

    new.r = sat((float)old.r * .393f + (float)old.g * .769f + (float)old.b * .189f);
    new.g = sat((float)old.r * .349f + (float)old.g * .686f + (float)old.b * .168f);
    new.b = sat((float)old.r * .272f + (float)old.g * .543f + (float)old.b * .131f);

    image_set_pixel_by_idx(image, i, new);
}

void image_make_sepia_c(image* image) {
    uint32_t width = image_get_width(image);
    uint32_t height = image_get_height(image);
    struct image* old_image = image_copy(image);

    for (uint32_t i = 0; i < width; i ++) {
        for (uint32_t j = 0; j < height; j ++) {
            sepia_one_pixel(old_image, image, i, j);
        }
    }
    image_destroy(old_image);
}

void image_make_sepia_asm(image* image) {
    uint32_t width = image_get_width(image);
    uint32_t height = image_get_height(image);
    struct image* old_image = image_copy(image);

    if (height * width < 4) {
        for (uint32_t i = 0; i < width; i ++) {
            for (uint32_t j = 0; j < height; j++) {
                sepia_one_pixel(old_image, image, i, j);
            }
        }
        return;
    }

    for (size_t i = 0; i < (height * width)/4*4 ; i+=4) {
        float src[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        pixel p0 = image_get_pixel_by_idx(old_image, i);
        pixel p1 = image_get_pixel_by_idx(old_image, i + 1);
        pixel p2 = image_get_pixel_by_idx(old_image, i + 2);
        pixel p3 = image_get_pixel_by_idx(old_image, i + 3);

        src[0] = p0.r; src[1] = p0.g; src[2] = p0.b;
        src[3] = p1.r; src[4] = p1.g; src[5] = p1.b;
        src[6] = p2.r; src[7] = p2.g; src[8] = p2.b;
        src[9] = p3.r; src[10] = p3.g; src[11] = p3.b;
        float dst[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        sepia_sse(src, dst);
        image_set_pixel_by_idx(image, i, (pixel){dst[0], dst[1], dst[2]});
        image_set_pixel_by_idx(image, i + 1, (pixel){dst[3], dst[4], dst[5]});
        image_set_pixel_by_idx(image, i + 2, (pixel){dst[6], dst[7], dst[8]});
        image_set_pixel_by_idx(image, i + 3, (pixel){dst[9], dst[10], dst[11]});
    }

    for (size_t i  = height * width - (height * width) % 4; i < height * width; ++i) {
        sepia_one_pixel_by_idx(old_image, image, i);
    }
}
