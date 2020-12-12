#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "image.h"
#include "image_io.h"
#include "image_utils.h"

void print_help(char*);
void print_load_err(image_load_status status);
void print_save_err(image_save_status status);

int main(int arg_num, char** args) {
    if (arg_num < 4) {
        puts("Пожалуйста, укажите 3 файла");
        print_help(args[0]);
        return 0;
    }

    char* src_file = args[1];
    char* dst_file_c = args[2];
    char* dst_file_asm = args[3];

    image* image;
    struct image* img_asm;

    image_load_status load_status = image_read_from_bmp(src_file, &image);
    /*image_load_status load_status_asm = */image_read_from_bmp(src_file, &img_asm);

    if (load_status == IMAGE_LOAD_OK) {
        struct rusage r;
        struct timeval start;
        struct timeval end;
        getrusage(RUSAGE_SELF, &r);
        start = r.ru_utime;
        image_make_sepia_c(image);

        getrusage(RUSAGE_SELF, &r);
        end = r.ru_utime;
        long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
        printf( "Время выполнения sepia (с): %ld\n", res);
        printf( "end.tv_sec: %ld\n", end.tv_sec);
        printf( "start.tv_sec: %ld\n", start.tv_sec);
        printf( "end.tv_usec: %ld\n", end.tv_usec);
        printf( "start.tv_usec: %ld\n", start.tv_usec);
        image_save_status save_status = image_save_to_bmp(dst_file_c, image);

        getrusage(RUSAGE_SELF, &r);
        start = r.ru_utime;
        image_make_sepia_asm(img_asm);
        end = r.ru_utime;
        res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
        printf( "Время выполнения sepia (sse): %ld\n", res);
        printf( "end.tv_sec: %ld\n", end.tv_sec);
        printf( "start.tv_sec: %ld\n", start.tv_sec);
        printf( "end.tv_usec: %ld\n", end.tv_usec);
        printf( "start.tv_usec: %ld\n", start.tv_usec);
        image_save_status save_status_asm = image_save_to_bmp(dst_file_asm, img_asm);


        if (save_status == IMAGE_SAVE_OK && save_status_asm == IMAGE_SAVE_OK) {
            puts("Я с радостью готов Вам доложить, что изображение, ранее загруженное из файла файловой системы данного персонального компьютера в формате BMP, было успешно повёрнуто и сохранено в файловую систему в виде нового файла в формате BMP, а также было проведено освобождение памяти, ранее занимаемое служебными объектами, создаваемыми в процессе обработки изображения, и подготовка программы к успешному завершению своей работы и возврату кода 0. Успехов Вам в Вашем, как говорится, нелёгком деле, и хорошего вечера! После вывода данного сообщения программа успешно завершит свою работу и вернёт управление пользователю, чтобы он мог выполнить следующие команды, непременно ждущие момента, когда их запустят и когда они смогут выполнить какую-нибудь полезную работу для пользователя, приблизив его к своей цели, ради которой он открыл этот терминал и начал набирать команды.");
            // puts("Segmentation fault");
        } else {
            print_save_err(save_status);
        }
    } else {
        print_load_err(load_status);
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

void print_help(char* cmd_name) {
    printf(
        "Использование: %s <src> <dst> <degrees>\n"
        "src        Файл, который надо повернуть\n"
        "dst_c        Файл, в который надо записать результат - c\n"
        "dst_asm    Файл, в который надо записать результат - asm\n",
        cmd_name
    );
}