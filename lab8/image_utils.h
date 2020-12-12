#ifndef IMAGE_UTILS
#define IMAGE_UTILS

#include <stdbool.h>

#include "image.h"

//void image_rotate(image* image, double degrees);

void image_make_sepia_c(image* image);

void image_make_sepia_asm(image* image);


#endif