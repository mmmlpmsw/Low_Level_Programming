#include "./linked_list.h"
#include "./higher_order_func.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

void print_space(int64_t);
void print_newline(int64_t);
int64_t square(int64_t);
int64_t cube(int64_t);
int64_t sum(int64_t x, int64_t a);
int64_t timesThree(int64_t x);
struct LinkedList *init(void);
struct LinkedList *iterate_test(void);
void save_load_test(struct LinkedList **list);
void file_bin_test(struct LinkedList **list);
void basic_test(struct LinkedList *list);

enum status {
    Ok, Serialize_error, Deserialize_error, Save_error, Loading_error
};

int main() {
    struct LinkedList *list = init();
    basic_test(list);
    struct LinkedList *iter = iterate_test();
    save_load_test(&iter);
    file_bin_test(&iter);
    list_free(list);
    list_free(iter);
    return Ok;
}

struct LinkedList *init(void) {
    int64_t digit;
    struct LinkedList *list;
    printf("Write digits separated by a space >>> \n");
    if (scanf("%" PRId64 "", &digit) != EOF) {
        list = list_create(digit);
        while (scanf("%" PRId64 "", &digit) != EOF) {
            list_add_front(&list, digit);
        }
    }
    return list;
}

void save_load_test(struct LinkedList **iter) {
    puts("Process saving in file...");
    if (!save(*iter, "aaaa")) {
        puts("Save error.");
        exit(Save_error);
    }
    puts("Save completed.");
    list_free(*iter);
    *iter = NULL;

    puts("Process loading from file...");
    if (!load(iter, "aaaa")) {
        puts("Load error.");
        exit(Loading_error);
    }
    printf("Load is completed, your list is:\n ");
    foreach(*iter, print_space);
    puts("");

}

void file_bin_test(struct LinkedList **iter) {
    puts("Process serialization...");
    if (!serialize(*iter, "./serialization.bin")) {
        puts("Serialization error");
        exit(Serialize_error);
    }
    puts("Serialization complete.");

    list_free(*iter);
    *iter = NULL;

    puts("Process deserialization...");
    if (!deserialize(iter, "./serialization.bin")) {
        puts("Deserialization error.");
        exit(Deserialize_error);
    }
    printf("Deserialization complete, your list is: \n");
    foreach(*iter, print_space);
    puts("");
}

struct LinkedList *iterate_test(void) {
    struct LinkedList *iter = iterate(1, 10, timesThree);
    puts("Iteration completed: ");
    foreach(iter, print_space);
    puts("");
    return iter;
}

void basic_test(struct LinkedList *list) {
    int64_t second_list = list_get_at(list, 2);
    size_t len = list_length(list);
    struct LinkedList *second_node = list_node_at(list, 2);
    int64_t sum = list_sum(list);
    //check length and sum
    printf("length = %lu, sum = %"PRId64"\n", len, sum);
    if ((second_list != 0) && (second_node != NULL)) {
        printf("list_get(2) = %"PRId64", list_node_at(2) = %"PRId64"\n",
               second_list,
               second_node->value);
    } else {
        puts("Error in list_node_at or list_get: element does not exist.");
    }

    //check foreach
    puts("Foreach with spaces");
    foreach(list, print_space);
    puts("\nForeach with new line");
    foreach(list, print_newline);

    //check map
    struct LinkedList *cubes_map = map(list, cube);
    puts("Map cubes is: ");
    foreach(cubes_map, print_space);
    puts("");
    list_free(cubes_map);

    //check map_mut
    map_mut(&list, imaxabs);
    puts("Map mut abs is: ");
    foreach(list, print_space);
    puts("");

    //check foldl
    int64_t su = foldl(0, list, square);
    printf("foldl sum is: %"PRId64"\n", su);
}

void print_space(int64_t i) { printf("%" PRId64 " ", i); }
void print_newline(int64_t i) { printf("%" PRId64 "\n", i); }
int64_t square(int64_t x) { return x * x; }
int64_t cube(int64_t x) { return x * x * x; }
int64_t sum(int64_t x, int64_t a) { return x + a; }
int64_t timesThree(int64_t x) { return 3 * x; }
int64_t multiply(int64_t x, int64_t a) { return x * a; }