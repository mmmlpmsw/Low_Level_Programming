#ifndef _IMAGE_IO
#define _IMAGE_IO

#include "image.h"

/**
 * Этот файл содержит определения, необходимые
 * для загрузки и сохранения изображений в различных форматах
 * (в нашем случае только в bmp).
**/

/**
 * Коды ошибок, которые могут возникнуть при загрузке изображения.
**/
typedef enum image_load_status {
    IMAGE_LOAD_OK = 0,
    IMAGE_LOAD_INVALID_FORMAT,
    IMAGE_LOAD_FILE_NOT_EXIST,
    IMAGE_LOAD_READ_FAIL,
    IMAGE_LOAD_TYPE_UNSUPPORTED,
    IMAGE_LOAD_BPP_NOT_SUPPORTED,
    IMAGE_LOAD_COMPRESSION_NOT_SUPPORTED
} image_load_status;

/**
 * Коды ошибок, которые могут возникнуть при записи изображения.
**/
typedef enum image_save_status {
    IMAGE_SAVE_OK = 0,
    IMAGE_SAVE_NO_ACCESS,
    IMAGE_SAVE_OPEN_FAIL,
    IMAGE_SAVE_WRITE_FAIL
} image_save_status;

/**
 * Читает bmp-файл в указанную ссылку на ссылку на изображение.
 * Непосредственный указатель (*dst, имеющий тип image*) будет заменен указателем на созданное изображение.
 * Изображение после использования следует освободить с помощью image_destroy(...).
 * 
 * Возвращает код, описывающий результат чтения.
 * Непосредственный указатель на изображение устанавливается только если чтение прошло успешно.
**/
image_load_status image_read_from_bmp(char* bmp_file_name, image** dst);

/**
 * Записывает изображение в файл в формате bmp.
 * Возвращает код, описывающий результат записи.
**/
image_save_status image_save_to_bmp(char* bmp_file_name, image* src);

#endif