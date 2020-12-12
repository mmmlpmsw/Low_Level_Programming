#include <stdlib.h>
#include <math.h>

// Мы собираемся реализовать в этом файле функции из этого h-файла
#include "image_utils.h"

/**
 * Поворачивает картинку на 90 градусов по часовой или против часовой стрелки.
 * Для работы с изображением используются функции из image.h.
**/
void image_rotate(image* image, double degrees) {
    // Переводим аргумент в радианы
    double radians = degrees*M_PI/180;

    // Математический фокус. Думаю, разберёшься
    bool size_swap_needed = fmod(abs(degrees) + 45, 180) > 90;

    // Получаем дилну картинки, для удобства
    uint32_t width = image_get_width(image);

    // Получаем ширину картинки, для удобства
    uint32_t height = image_get_height(image);

    // Нам потребуется старое изображение,
    // потому что мы собираемся модифицировать уже имеющееся
    struct image* old_image = image_copy(image);

    // Получаем координаты центра изображения
    double center_x = (double)width/2.0;
    double center_y = (double)height/2.0;

    double cosr = cos(-radians);
    double sinr = sin(-radians);

    // Обратная матрица трансформации изображения (объясню её отдельно)
    double transform_matrix3[] = {
        cos(-radians), -sin(-radians), - center_x*cos(-radians) + center_y*sin(-radians) + center_x,
        sin(-radians),  cos(-radians), - center_x*sin(-radians) - center_y*cos(-radians) + center_y,
        0, 0, 1
    };

    // Надо ли менять местами ширину и высоту?
    if (size_swap_needed) {
        image_swap_size(image); // Меняем...
        // Размер картинки изменился, обновляем переменные
        width = image_get_width(image);
        height = image_get_height(image);

        // Корректируем матрицу
        transform_matrix3[2] = - center_y*cos(-radians) + center_x*sin(-radians) + center_x;
        transform_matrix3[5] = - center_y*sin(-radians) - center_x*cos(-radians) + center_y;
    }

    // Двойной цикл, перемещающий каждый пиксель картинки
    // в его новую позицию
    // Внешний цикл движется по строчкам сверху вниз
    for (uint32_t current_y = 0; current_y < height; current_y++) {
        // Внутренний цикл движется по столбикам слева направо
        for (uint32_t current_x = 0; current_x < width; current_x++) {
            // Координаты пикселя-приёмника
            point dst_point = { current_x, current_y };

            // Вычисление координат пикселя-источника
            double src_x = dst_point.x;
            double src_y = dst_point.y;

            // Математический фокус, объясню отдельно
            src_x += 0.5;
            src_y += 0.5;

            // Умножение обратной матрицы на вектор смещённых координат пикселя-приёмника,
            // получение итоговых координат пикселя-источника
            point src_point = {
                floor(transform_matrix3[0]*src_x + transform_matrix3[1]*src_y + transform_matrix3[2]),
                floor(transform_matrix3[3]*src_x + transform_matrix3[4]*src_y + transform_matrix3[5])
            };

            // Получаем пиксель-источник. Читаем из сохранённого старого изображение,
            // поскольку текущее изображение сейчас модифицируется
            pixel src_pixel = image_get_pixel(old_image, src_point);

            // Устанавливаем новую точку
            image_set_pixel(image, dst_point, src_pixel);
        }
    }

    // Нам больше не нужно старое изображение, освобождаем его
    image_destroy(old_image);
}