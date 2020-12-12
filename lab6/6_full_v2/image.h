#ifndef _IMAGE_H
#define _IMAGE_H

/**
 * Этот файл содержит определения, необходимые для
 * непосредственных (!) операций над изображением,
 * таких как:
 * 1. Создание/удаление изображения
 * 2. Получение ширины/высоты изображения
 * 3. Получение и установка отдельных пикселей
 * 
**/

#include <stdint.h>
#include <stdbool.h>

/**
 * Внутреннее представление изображения.
 * Поля структуры должны быть скрыты, доступ к
 * полям осуществляется только через функции, определённые ниже.
**/
struct image;

/**
 * Определение типа 'image', который равен 'struct image'.
 * Используется для удобства.
**/
typedef struct image image;

/**
 * Внутреннее представление пикселя.
 * Альфа-канал в этой лабе не нужен, используется для
 * потенциальной совместисомсти с форматами, имеющими
 * прозрачность (например, png).
**/
typedef struct pixel {
    uint8_t r, g, b, a;
} pixel;

/**
 * Точка для обозначения координат на изображении, для удобства.
**/
typedef struct point {
    uint32_t x, y;
} point;

/**
 * Создаёт изображение, выделяя под него память.
 * Изображение должно создаваться только через эту функцию
 * (либо через функцию, использующую эту функцию).
 * Изображение должно быть инициализированово и готово к работе.
 * По умолчанию изображение заполнено черными прозрачными
 * пикселями (r = 0, g = 0, b = 0, a = 0).
 * Изображение после использования следует разрушить,
 * используя функцию image_destroy.
**/
image* image_create(uint32_t width, uint32_t height);

/**
 * Создаёт копию изображения в памяти
 * Возвращает ссылку на новое изображение
**/
image* image_copy(image* image);

/**
 * Освобождает память, используемую изображением.
 * Ссылка, переданная в эту функцию, станет недействительной.
**/
void image_destroy(image* image);

/**
 * Возвращает ширину изображения в пикселях.
**/
uint32_t image_get_width(image* image);

/**
 * Возвращает высоту изображения в пикселях.
**/
uint32_t image_get_height(image* image);

/**
 * Меняет местами ширину и высоту изображения.
 * Целостность пиксельных данных после вызова этой функции не гарантируется.
**/
void image_swap_size(image* image);

/**
 * Возвращает пиксель изображения, находящийся в указанных координатах.
 * Если точка находится за пределами изображения, возвращается
 * прозрачный черный пиксель (r = 0, g = 0, b = 0, a = 0).
**/
pixel image_get_pixel(image* image, point point);

/**
 * Возвращает пиксель изображения, находящийся в указанных координатах.
 * Если точка находится за пределами изображения, возвращается
 * пиксель по умолчанию (третий аргумент).
**/
pixel image_get_pixel_or_default(image* image, point point, pixel default_pixel);

/**
 * Устанавливает указанный писель указанному изображению в указанное место.
 * Если пиксель установлен, возвращает true.
 * Если точка находится за пределами изображения, возвращает false.
**/
bool image_set_pixel(image* image, point point, pixel pixel);

/**
 * Проверяем, находится ли точка на изображении, или за её пределами.
 * Если точка находится в пределах изображения, возвращает true.
 * Иначе, возвращает falsse.
**/
bool image_check_bounds(image* image, point point);

#endif