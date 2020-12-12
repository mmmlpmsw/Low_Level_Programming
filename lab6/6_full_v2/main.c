#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вставляем все наши библиотеки
#include "image.h"
#include "image_io.h"
#include "image_utils.h"

void print_help(char*);
void print_load_err(image_load_status status);
void print_save_err(image_save_status status);

int main(int arg_num, char** args) {
    // Указаны ли хотя бы имена двух файлов (первый аргумент - название программы)?
    if (arg_num < 3) {
        puts("Пожалуйста, укажите 2 файла");
        print_help(args[0]);
        return 0;
    }

    // Получаем имя файла-источника
    char* src_file = args[1];

    // Получаем имя файла-приёмника
    char* dst_file = args[2];

    // Поворот по умолчанию
    double degrees = 90;

    // Может, пользователь указал поворот?
    if (arg_num >= 4) {
        // Получаем слово третьего аргумента
        char* degrees_str = args[3];

        // Превращаем это в double
        degrees = strtod(degrees_str, NULL); // Не помню зачем нужен 2 аргумент, почитай маны
    }

    // Создаём нулевой указатель на изображение
    image* image;

    // Загружаем картинку из файла, ссылка на изображение запишется в image
    image_load_status load_status = image_read_from_bmp(src_file, &image);

    // Удалось прочитать?
    if (load_status == IMAGE_LOAD_OK) {
        // Поворачиваем картинку
        printf("Поворачиваем на %.2f граудсов...\n", degrees);
        image_rotate(image, degrees);

        // Сохраняем картинку
        image_save_status save_status = image_save_to_bmp(dst_file, image);

        // Удалось записать?
        if (save_status == IMAGE_SAVE_OK) {
            puts("Я с радостью готов Вам доложить, что изображение, ранее загруженное из файла файловой системы данного персонального компьютера в формате BMP, было успешно повёрнуто и сохранено в файловую систему в виде нового файла в формате BMP, а также было проведено освобождение памяти, ранее занимаемое служебными объектами, создаваемыми в процессе обработки изображения, и подготовка программы к успешному завершению своей работы и возврату кода 0. Успехов Вам в Вашем, как говорится, нелёгком деле, и хорошего вечера! После вывода данного сообщения программа успешно завершит свою работу и вернёт управление пользователю, чтобы он мог выполнить следующие команды, непременно ждущие момента, когда их запустят и когда они смогут выполнить какую-нибудь полезную работу для пользователя, приблизив его к своей цели, ради которой он открыл этот терминал и начал набирать команды.");
            // puts("Segmentation fault");
        } else { // При записи что-то пошло не так
            print_save_err(save_status); // Вызываем функцию, чтобы нагромождать main
        }
    } else { // Что-то пошло не так
        print_load_err(load_status); // Вызываем функцию, чтобы нагромождать main
    }

    return 0;
}

void print_load_err(image_load_status status) {
    if (status == IMAGE_LOAD_BPP_NOT_SUPPORTED)
        puts("Такое количество бит на пиксель не поддерживается");
    else if (status == IMAGE_LOAD_COMPRESSION_NOT_SUPPORTED)
        puts("Сжатые BMP не поддерживаются");
    else if (status == IMAGE_LOAD_FILE_NOT_EXIST)
        puts("Указанный файл не существует");
    else if (status == IMAGE_LOAD_TYPE_UNSUPPORTED)
        puts("Файл в неверном формате");
    else if (status == IMAGE_LOAD_READ_FAIL)
        puts("Ошибка чтения файла");
    else
        puts("аааааааааааааааааааааааааааааааааааааааааа");
}

void print_save_err(image_save_status status) {
    if (status == IMAGE_SAVE_NO_ACCESS)
        puts("Нет доступа для записи в файл");
    else if (status == IMAGE_SAVE_OPEN_FAIL)
        puts("Не удалось открыть файл для записи");
    else if (status == IMAGE_SAVE_WRITE_FAIL)
        puts("Не удалось записать в файл");
    else
        puts("аааааааааааааааааааааааааааааааааааааааааа");
}

/**
 * Выводит подсказку по использованию программы для пользователя.
 * 
 * @param cmd_name Название команды этой программы.
**/
void print_help(char* cmd_name) {
    printf(
        "Использование: %s <src> <dst> <degrees>\n"
        "src        Файл, который надо повернуть\n"
        "dst        Файл, в который надо записать результат\n"
        "degrees    Поворот (по умолчанию 90)\n",
        cmd_name
    );
}