#include <stdlib.h> // Тут содержится size_t
#include <string.h> // Тут содержится memcpy

// Мы делаем #include этого файла, так как собираемся реализовать
// объявленные в нём методы
#include "image.h"

/**
 * Реализация внутреннего представления изображения.
 * Поля не должны быть доступна внешним файлам, доступ к ним
 * должен осуществляться только через специальные функции.
**/
struct image {
    // Ширина картинки пикселях
    uint32_t width;

    // Высота картинки в пикселях
    uint32_t height;

    // Одномерный массив, содержащий все пиксели изображения
    pixel* data_array;
};

image* image_create(uint32_t width, uint32_t height) {
    // Выделяем память под непосредственно структуру изображения
    image* new_image = malloc(sizeof(image));

    // Записываем в новое изображение его ширину и высоту
    new_image->width = width;
    new_image->height = height;
    
    // Вычисляем, сколько в изображении будет пикселей
    size_t pixel_count = width * height;

    // Вычисляем, сколько места займёт массив пикселей нового изображения
    size_t data_array_size = sizeof(pixel) * pixel_count;

    // Выделяем память под массив пикселей
    pixel* new_data_array = malloc(data_array_size);

    // Обнуляем весь массив пикселей
    for (size_t i = 0; i < pixel_count; i++)
        new_data_array[i] = (pixel) {0, 0, 0, 0};

    // Записываем в новое изображение указатель на его массив пикселей
    new_image->data_array = new_data_array;

    // Возвращаем новое изображение
    return new_image;
}

image* image_copy(image* image) {
    // Проверяем, не является ли аргумент нулевым указателем
    if (image == NULL)
        return NULL;

    // Получаем информацию о ширине и высоте изображения
    uint32_t width = image_get_width(image);
    uint32_t height = image_get_height(image);
    
    // Вычисляем, сколько пикселей содержит изображение
    size_t pixel_count = width * height;

    // Вычисялем, сколько места занимает массив пикселей изображения
    size_t image_data_size = sizeof(pixel) * width * height;

    // Создаём новое изображение. Память под него и его массив пикселей будет уже выделена
    // Используем struct image, так как компилятор путает тип image и переменную image
    struct image* new_image = image_create(width, height);

    // Достаём указатель на массив пикселей нового изображения
    // Сейчас этот массив заполнен прозрачными пикселями (0, 0, 0, 0).
    pixel* new_image_data = new_image->data_array;

    // Достаём указатль на массив пикселей исходного изображения
    pixel* source_image_data = image->data_array;

    // Целиком копируем память исходного массива в новый массив.
    memcpy(new_image_data, source_image_data, image_data_size);

    // Мы не можем просто скопировать указатель, потому что тогда
    // изменение одного изображения будет отражаться на другом
    // и освобождение одного из изображений сделает
    // недействительным второе

    // Возвращаем новое изображение
    return new_image;
}

void image_destroy(image* image) {
    // Проверяем, не является ли переданный аргумент нулевым указатлем
    if (image == NULL)
        return;

    // Проверяем, содержит ли изображение валидный указатель на массив пикселей
    if (image->data_array != NULL) {
        // Освобождаем память, которую занимал массив пикселей
        free(image->data_array);
    }

    // Освобождаем память, которую занимала структура изображения
    free(image);
}

uint32_t image_get_width(image* image) {
    // Не являяется ли аргумент NULL?
    if (image == NULL)
        return 0;

    // Возвращаем ширину картинки
    return image->width;
}

uint32_t image_get_height(image* image) {
    // Не являяется ли аргумент NULL?
    if (image == NULL)
        return 0;

    // Возвращаем ширину картинки
    return image->height;
}

pixel image_get_pixel(image* image, point point) {
    // Используем безопасный вариант функции получения пикселя с указанием пикселя по умолчанию
    return image_get_pixel_or_default(image, point, (pixel) {0, 0, 0, 0});
}

pixel image_get_pixel_or_default(image* image, point point, pixel default_pixel) {
    // Мы получили нулевой указатель?
    if (image == NULL)
        return default_pixel; // Возвращаем пиксель по умолчанию
    
    // Находится ли точка за пределами картинки?
    if (!image_check_bounds(image, point))
        return default_pixel; // Возвращаем пиксель по умолчанию
    
    // Вычисляем, где находится нужный пиксель
    size_t pixel_index = image_get_width(image) * point.y + point.x;

    // Возвращаем пиксель картинки
    return image->data_array[pixel_index];
}

bool image_set_pixel(image* image, point point, pixel pixel) {
    // Мы поулчили нулевой указатель?
    if (image == NULL)
        return false; // Ничего не записали, возвращаем false
    
    // Находится ли точка за пределами картинки?
    if (!image_check_bounds(image, point))
        return false; // Ничего не записали, возвращаем false
    
    // Вычисляем, где находится нужный пиксель
    size_t pixel_index = image_get_width(image) * point.y + point.x;

    // Записываем пиксель
    image->data_array[pixel_index] = pixel;
    
    // Пиксель записан, возвращаем true
    return true;
}

bool image_check_bounds(image* image, point point) {
    // Мы получили нулевой указатель?
    if (image == NULL) {
        // Изображения нет, следовательно, никакая точка
        // не может находится в её пределах.
        // Следовательно, возвращаем false.
        return false;
    }

    // Проверяем, находится ли точка в пределах изображения и сразу возвращаем результат
    return !(
        point.x < 0 ||
        point.x >= image_get_width(image) ||
        point.y < 0 ||
        point.y >= image_get_height(image)
    );
}

void image_swap_size(image* image) {
    // Мы получили нулевой указатель?
    if (image == NULL)
        return;

    uint32_t buffer = image->width;
    image->width = image->height;
    image->height = buffer;
}