#include <stdlib.h> // Тут есть size_t
#include <stdio.h> // Тут есть fopen, fwrite, и так далее
#include <errno.h> // Тут есть коды ошибок

// В этом файле мы собираемся реализовать функции, объявленные тут
#include "image_io.h"

// Число, на которое должен начинаться любой bmp-файл (в little-endian)
static const uint16_t BMP_MAGIC_NUMBER = 0x4D42;

// Запакованная структура, повторяющая заголовок bmp-файлов
struct __attribute__((packed)) bmp_file_header {
    // Signature (14 bytes)
    uint16_t type;
    uint32_t file_size; 
    uint32_t reserved;
    uint32_t data_start_addr;

    // Info header (40 bytes)
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

// Запакованная структура, повторяющая один пиксель в bmp-файле.
// В bmp-файлах цвета расположены в следующем порядке: синий, зелёный, красный.
struct __attribute__((packed)) bmp_pixel_bgr {
    uint8_t b, g, r;
};

image_load_status image_read_from_bmp(char* bmp_file_name, image** dst) {
    // Открываем файл в режиме бинарного (b) чтения (r)
    FILE* file = fopen(bmp_file_name, "rb");

    // Файл не открылся?
    if (file == NULL) {
        // Это потому что файла нет?
        if (errno == ENOENT)
            return IMAGE_LOAD_FILE_NOT_EXIST; // Возвращаем код о том, что файла нет
        return IMAGE_LOAD_READ_FAIL; // Код для всех остальных ошибок открытия файла
    }

    // Объявляем структуру заголовка bmp-файла
    struct bmp_file_header header;
    // Читаем из файла заголовок целиком
    size_t have_read = fread(&header, sizeof(header), 1, file);

    // Во время чтения что-то пошло не так?
    if (ferror(file))
        return IMAGE_LOAD_READ_FAIL; // Возвращаем код о том, что чтение не удалось

    // Нам дали файл, который не начинается на специальное число?
    if (header.type != BMP_MAGIC_NUMBER)
        return IMAGE_LOAD_TYPE_UNSUPPORTED; // Вероятно, перед нами не BMP. Возвращаем код об этом

    // Количество бит на пиксель отличается от того, которое было в задании?
    if (header.bits_per_pixel != 24)
        return IMAGE_LOAD_BPP_NOT_SUPPORTED; // Сообщаем о том, что это количество бит на пиксель не поддерживается

    // Файл содержит пиксели в сжатом виде?
    if (header.compression_method != 0)
        return IMAGE_LOAD_COMPRESSION_NOT_SUPPORTED; // Сообщаем о том, что сжатие не поддерживается

    // Узнаём, где начинаются пиксельные данные
    uint32_t pixel_data_start = header.data_start_addr;

    // Узнаём, сколько должна занимать одна строка пикселей
    uint32_t width_bytes = header.width*3;

    // В BMP размер одной строки пикселей всегда округляется вверх так, чтобы делиться на 4.
    // Эта переменная содержит количество "лишних" байтов в конце каждой строки пикселей,
    // которые были добавлены ради округления
    uint32_t width_bytes_padding = (4 - width_bytes%4)%4;
    
    // Перемещаем указатель туда, где должны начинаться пиксельные данные
    fseek(file, pixel_data_start, SEEK_SET);

    // Создаём изображение, в которое будем читать пиксели
    image* result = image_create(header.width, header.height);

    // Выделяем место под буфер пикселей для одной строки в bmp-специфичном формате
    struct bmp_pixel_bgr* bmp_pixel_data = (struct bmp_pixel_bgr*)malloc(sizeof(struct bmp_pixel_bgr) * width_bytes);

    // Двигаемся по строкам
    for (uint32_t pixel_row_index = 0; pixel_row_index < header.height; pixel_row_index++) {
        // Читаем пиксели в строке
        fread(bmp_pixel_data, width_bytes, 1, file);

        // Цикл записи пикселей в строке
        for (uint32_t j = 0; j < image_get_width(result); j++) {
            // Получаем bmp-специфичный пиксель
            struct bmp_pixel_bgr bmp_specific_pixel = bmp_pixel_data[j];

            // Превращаем bmp-специфичный пиксель в общий для всех форматов пиксель
            pixel generic_pixel = (pixel) {
                bmp_pixel_data[j].r,
                bmp_pixel_data[j].g,
                bmp_pixel_data[j].b,
                255
            };

            // Вычисляем, куда надо записать этот пиксель
            // В bmp-изображениях строки перевёрнуты (первая строка - это самая нижняя),
            // поэтому Y координата в этой точке перевёрнута
            point dest_point = (point) {
                j,
                image_get_height(result) - pixel_row_index - 1
            };

            // Теперь устанавливаем нужный пиксель в нужное место
            image_set_pixel(result, dest_point, generic_pixel);
        }

        // Ради округления после каждой строки в BMP есть несколько лишних байт
        // Немного сдвигаем указатель файла вперёд, чтобы сразу
        // перейти к началу следующей строки
        fseek(file, width_bytes_padding, SEEK_CUR);
    }

    // Мы занимали память под буфер bmp-пикселей, теперь освобождаем
    free(bmp_pixel_data);
    // Закрываем открытый файл
    fclose(file);

    // Записываем по двойному указателю указатель на новое изображение
    *dst = result;

    // Всё прошло хорошо, возвращаем соответствующий код
    return IMAGE_LOAD_OK;
}

image_save_status image_save_to_bmp(char* bmp_file_name, image* src) {
    // Открываем файл для бинарной (b) записи (w)
    FILE* file = fopen(bmp_file_name, "wb");

    // Файл не открылся?
    if (file == NULL) {
        // Это потому что у нас прав нет?
        if (errno == EACCES)
            return IMAGE_SAVE_NO_ACCESS; // Тогда возвращаем соответствующий код
        return IMAGE_SAVE_OPEN_FAIL; // Общий код для ошибок открытия файла для записи
    }

    // Вычисляем, сколько нужно байт для записи одной строчки пикселей
    uint32_t width_bytes = image_get_width(src)*3;
    
    // Так как в BMP ширина всех строк должна округляться вверх до 4 байт,
    // корректируем итоговый размер с учетом этого округления
    uint32_t width_bytes_padded = width_bytes % 4 == 0 ? width_bytes : width_bytes - width_bytes%4 + 4;

    // Создаём заголовое
    struct bmp_file_header header;

    // Вычисляем итоговый размер файла (заголовок + ширина одной строки * количество строк)
    uint32_t file_size = sizeof(header) + width_bytes_padded * image_get_height(src);

    header.type = BMP_MAGIC_NUMBER; // В начале BMP-файла всегда должно быть это число
    header.file_size = file_size; // Размер файла
    header.reserved = 0;    // Зарезервированное поле, должно быть 0

    // Смещение файла, с которое начинаются пиксельные данные.
    // Так как мы пишем пиксели сразу после заголовка, указываем смещение,
    // равное размеру этого заголовка.
    header.data_start_addr = sizeof(header);
    header.header_size = 40;    // Размер этого заголовка
    header.width = image_get_width(src);    // Ширина картинки в пикселях
    header.height = image_get_height(src);  // Высота картинки в пикселях
    header.color_planes_num = 1;    // В соответствии с описанием BMP, всегда равен 1
    header.bits_per_pixel = 24;     // Сколько нужно бит, чтобы закодировать 1 пиксель (8 бит * 3 пикселя)
    header.compression_method = 0;  // 0 = нет компрессии
    header.image_size = 0;          // Размер картинки, лень было считать
    header.x_pixels_per_meter = 1;  // Не знаю зачем это, ставлю 1
    header.y_pixels_per_meter = 1;  // Не знаю зачем это, ставлю 1
    header.colors_num = 256*256*256;// Количество цветов (2^8 цветов * 3 канала)
    header.important_colors_num = 0;// Количество важных цветов, 0 = все цвета важны

    // Записываем весь заголовок в файл
    fwrite(&header, sizeof(header), 1, file);

    // При записи что-то пошло не так?
    if (ferror(file))
        return IMAGE_SAVE_WRITE_FAIL; // Возвращаем соответствующий код

    // Выделяем место под буфер bmp-специфичных пикселей
    struct bmp_pixel_bgr* pixel_data = (struct bmp_pixel_bgr*)malloc(width_bytes_padded);

    // Цикл итерации по строкам
    for (uint32_t row = 0; row < image_get_height(src); row++) {
        
        // Цикл итерации по пикселям в строке
        for (uint32_t pixelIdx = 0; pixelIdx < image_get_width(src); pixelIdx++) {
            // Получаем пиксель из изображения.
            // Так как в bmp строки перевёрнуты, читаем строки снизу вверх
            pixel px = image_get_pixel(src, (point) { pixelIdx, image_get_height(src) - row - 1 });
            // Записываем пиксели в буфер в bmp-специфичном формате
            pixel_data[pixelIdx] = (struct bmp_pixel_bgr) {
                px.b, px.g, px.r
            };
        }
        // Пишем буфер bmp-строки в файл
        fwrite(pixel_data, width_bytes_padded, 1, file);

        // При записи что-то пошло не так?
        if (ferror(file))
            return IMAGE_SAVE_WRITE_FAIL; // Возвращаем соответствующий код
    }

    // Освобождаем выделенный буфер
    free(pixel_data);

    // Закрываем открытый файл
    fclose(file);

    // Запись прошла успешно, возвращаем соответствующий код
    return IMAGE_SAVE_OK;
}