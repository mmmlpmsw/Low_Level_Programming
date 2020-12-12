// Мы собираемся реализовать в этом файле функции из этого h-файла
#include "image_utils.h"

/**
 * Поворачивает картинку на 90 градусов по часовой или против часовой стрелки.
 * Для работы с изображением используются функции из image.h.
**/
void image_rotate(image* image, bool clockwise) {
    // Получаем дилну картинки, для удобства
    uint32_t width = image_get_width(image);

    // Получаем ширину картинки, для удобства
    uint32_t height = image_get_height(image);

    // Нам потребуется старое изображение,
    // потому что мы собираемся модифицировать уже имеющееся
    struct image* old_image = image_copy(image);

    // Изображение повёрнуто, поэтому меняем местами ширину и высоту
    image_swap_size(image);

    // Двойной цикл, перемещающий каждый пиксель картинки
    // в его новую позицию
    // Внешний цикл движется по строчкам сверху вниз
    for (uint32_t current_y = 0; current_y < height; current_y++) {
        // Внутренний цикл движется по столбикам слева направо
        for (uint32_t current_x = 0; current_x < width; current_x++) {
            // Координаты пикселя-источника
            point src_point = { current_x, current_y };

            // Получаем пиксель-источник. Читаем из сохранённого старого изображение,
            // поскольку текущее изображение сейчас модифицируется
            pixel src_pixel = image_get_pixel(old_image, src_point);

            // Объявляем координаты пикселя-приёмника
            point new_point;

            // Поворот по часовой стрелке?
            if (clockwise)
                new_point = (point) { height - current_y - 1, current_x };
            else
                new_point = (point) { current_y, width - current_x - 1 };
            
            // Устанавливаем новую точку
            image_set_pixel(image, new_point, src_pixel);
        }
    }

    // Нам больше не нужно старое изображение, освобождаем его
    image_destroy(old_image);
}